/*                                                     
 * based on 
 * 1. ldd3 hello.c
 * 2. linux-3.5.3/kernel/debug/debug_core.c
 * 3. linux-3.5.3/kernel/trace/trace.c
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/sysrq.h>//to register a basic function in sysrq table
#include <linux/debugfs.h>
#include <linux/rbtree.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

#define SYSRQ_HARDIRQ_TRIGGER_CHAR 'x'

MODULE_LICENSE("Dual BSD/GPL");

static void sysrq_handle_dbg(int key)
{
	printk("%s:%d\n", __func__, __LINE__);
}

static struct sysrq_key_op sysrq_dbg_op = {
	.handler	= sysrq_handle_dbg,
	.help_msg = "Hardirq;)(x)",
	.action_msg = "Hardirq:)",
};

static struct dentry *d_debug;

struct cfs_rq {
	struct rb_root tasks_timeline;// = RB_ROOT;
	struct list_head tasks;
	spinlock_t lock;
};

struct __sched_entity {
	struct rb_node run_node;
	struct list_head group_node;
	unsigned long val;
};

struct cfs_rq *cfs_rq;

struct dentry *debug_create_file(const char *name,
				umode_t mode,
				struct dentry *parent,
				void *data,
				const struct file_operations *fops)
{
	struct dentry *ret;
	ret = debugfs_create_file(name, mode, parent, data, fops);
	if (!ret)
		printk("Could not create debugfs '%s' entry\n", name);
	return ret;
}			

struct dentry *debug_init_dentry(void)
{
	if (d_debug)
		return d_debug;
	if (!debugfs_initialized())
		return NULL;
	d_debug = debugfs_create_dir("debug", NULL);
	if (!d_debug) {
		printk("Could not create debugfs directory 'debug'\n");
		return NULL;
	}
	return d_debug;
}

int debug_open_generic(struct inode *inode, struct file *filep)
{
	//if...
	filep->private_data = inode->i_private;
	return 0;
}

static ssize_t
debug_rb_read(struct file *filep, char __user *ubuf,
			size_t cnt, loff_t *ppos)
{
	int nr = 0;
	struct cfs_rq *cfs_rq = filep->private_data;
	struct rb_node *node = rb_first(&cfs_rq->tasks_timeline);
	spin_lock(&cfs_rq->lock);
	for (; node; node = rb_next(node)) {
		struct __sched_entity *se = rb_entry(node, struct __sched_entity, run_node);
		printk("%lu ", se->val);
		nr++;
	}
	printk("\nTotal %d\n", nr);
	spin_unlock(&cfs_rq->lock);
	return 0;//FIXME?
}		

static ssize_t
debug_rb_insert(struct file *filp, const char __user *ubuf, 
				size_t cnt, loff_t *ppos)
{
	struct cfs_rq *cfs_rq = filp->private_data;
	unsigned long val;
	int ret;
	struct rb_node **link = &cfs_rq->tasks_timeline.rb_node;
	struct rb_node *parent = NULL;
	struct __sched_entity *entry, *se;
	
	ret = kstrtoul_from_user(ubuf, cnt, 10, &val);
	if (ret)
		return ret;
	se = kmalloc(sizeof(*se), GFP_KERNEL);
	if (!se)
		return -ENOMEM;
	se->val = val;
	INIT_LIST_HEAD(&se->group_node);
	spin_lock(&cfs_rq->lock);
	while (*link) {
		parent = *link;
		entry = rb_entry(parent, struct __sched_entity, run_node);
		if (val < entry->val)
			link = &parent->rb_left;
		else
			link = &parent->rb_right;
	}
	rb_link_node(&se->run_node, parent, link);
	rb_insert_color(&se->run_node, &cfs_rq->tasks_timeline);
	spin_unlock(&cfs_rq->lock);		
	*ppos += cnt;
	return cnt;
}

//borrowed from rbtree.txt
struct __sched_entity *my_search(struct rb_root *root, unsigned long val)
{
	struct rb_node *node = root->rb_node;
	while (node) {
		struct __sched_entity *se = rb_entry(node, struct __sched_entity, run_node);
		if (val < se->val)
			node = node->rb_left;
		else if (val > se->val)
			node = node->rb_right;
		else
			return se;	
	}
	return NULL;
}

static ssize_t
debug_rb_erase(struct file *filp, const char __user *ubuf,
			size_t cnt, loff_t *ppos)
{
	struct cfs_rq *cfs_rq = filp->private_data;
	unsigned long val;
	int ret;
	struct __sched_entity *se;
	struct rb_root *root = &cfs_rq->tasks_timeline;
	
	ret = kstrtoul_from_user(ubuf, cnt, 10, &val);
	if (ret)
		return ret;
	spin_lock(&cfs_rq->lock);
	se = my_search(root, val);
	if (se) {
		rb_erase(&se->run_node, root);
		kfree(se);
	} else
		printk("%lu not exist!\n", val);
	spin_unlock(&cfs_rq->lock);
	*ppos += cnt;
	return cnt;
}

static irqreturn_t fake_irq_handler(int irq, void *dev_id)
{
	static int nr = 0;
	printk("%s %d\n", __func__, nr++);
	return IRQ_HANDLED;
}

static ssize_t
debug_request_irq_write(struct file *filp, const char __user *ubuf, 
					size_t cnt, loff_t *ppos)
{
	int ret;
	unsigned long val;
	ret = kstrtoul_from_user(ubuf, cnt, 10, &val);
	if (ret)
		return ret;
	ret = request_irq(val, fake_irq_handler, 0, "fake_device", NULL);
	printk("request_irq %ld %d\n", val, ret);
	*ppos += cnt;
	return cnt;
}				

static ssize_t
debug_free_irq_write(struct file *filp, const char __user *ubuf, 
					size_t cnt, loff_t *ppos)
{
	int ret;
	unsigned long val;
	ret = kstrtoul_from_user(ubuf, cnt, 10, &val);
	if (ret)
		return ret;
	free_irq(val, NULL);
	*ppos += cnt;
	return cnt;
}				

struct tasklet_struct free_irq_tasklet;

static void free_tasklet(unsigned long data)
{
	free_irq(data, NULL);
}

static ssize_t
debug_tasklet_write(struct file *filp, const char __user *ubuf, 
					size_t cnt, loff_t *ppos)
{
	int ret;
	unsigned long val;
	ret = kstrtoul_from_user(ubuf, cnt, 10, &val);
	if (ret)
		return ret;
	tasklet_init(&free_irq_tasklet, free_tasklet, val);
	tasklet_schedule(&free_irq_tasklet);
	//free_irq(val, NULL);
	*ppos += cnt;
	return cnt;
}

static const struct file_operations debug_rb_insert_fops = {
	.open	= debug_open_generic,
	.read	= debug_rb_read,
	.write	= debug_rb_insert,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_rb_erase_fops = {
	.open	= debug_open_generic,
	.read	= debug_rb_read,
	.write	= debug_rb_erase,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_request_irq_fops = {
	.open	= debug_open_generic,
	.write	= debug_request_irq_write,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_free_irq_fops = {
	.open	= debug_open_generic,
	.write	= debug_free_irq_write,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_tasklet_fops = {
	.open	= debug_open_generic,
	.write	= debug_tasklet_write,
	.llseek	= generic_file_llseek,
};

static int debug_init(void)
{
	struct dentry *d_debug;
	int ret;
	/*
	register_sysrq_key compares the desired index(if valid)'s org (struct sysrq_key_op*) with NULL, and assigned it with your (sysrq_key_op*) iff
	its org value is NULL!
	return:
	0  - real ok
	   - false ok, your index is NOT valid([0,9] || [a,z]) and the assignment
         will not occur.
	     this case seems a bug!!!
	-1 - index is valid but its org value is not NULL! And the assignment will
         not occur.
	*/
	ret = register_sysrq_key(SYSRQ_HARDIRQ_TRIGGER_CHAR, &sysrq_dbg_op);
	if (!ret){
		printk("register '%c'\n", SYSRQ_HARDIRQ_TRIGGER_CHAR);
		d_debug = debug_init_dentry();
		if (!d_debug || !(cfs_rq = kmalloc(sizeof(*cfs_rq), GFP_KERNEL))) {
			unregister_sysrq_key(SYSRQ_HARDIRQ_TRIGGER_CHAR, &sysrq_dbg_op);
			ret = -1;
			goto EXIT;
		}
		cfs_rq->tasks_timeline = RB_ROOT;
		INIT_LIST_HEAD(&cfs_rq->tasks);
		spin_lock_init(&cfs_rq->lock);
		debug_create_file("rb_link", 0644, d_debug, cfs_rq, &debug_rb_insert_fops);
		debug_create_file("rb_erase", 0644, d_debug, cfs_rq, &debug_rb_erase_fops);
		debug_create_file("request_irq", 0220, d_debug, cfs_rq, &debug_request_irq_fops);
		debug_create_file("free_irq", 0220, d_debug, cfs_rq, &debug_free_irq_fops);
		debug_create_file("tasklet", 0220, d_debug, cfs_rq, &debug_tasklet_fops);
	}
EXIT:	
	return ret;
}

//borrowed from ubi
static void tree_destroy(struct rb_root* root)
{
	struct rb_node *rb;
	struct __sched_entity *se;
	
	rb = root->rb_node;
	while (rb) {
		if (rb->rb_left)
			rb = rb->rb_left;
		else if (rb->rb_right)
			rb = rb->rb_right;
		else {
			se = rb_entry(rb, struct __sched_entity, run_node);
			rb = rb_parent(rb);
			if (rb) {
				if (rb->rb_left == &se->run_node)
					rb->rb_left = NULL;
				else
					rb->rb_right = NULL;
			}
			kfree(se);
		}
	}
}

static void debug_exit(void)
{
	printk(KERN_ALERT "un-register '%c'\n", SYSRQ_HARDIRQ_TRIGGER_CHAR);
	/*
	unregister_sysrq_key return: remove the key op from key iff it is currently
	registered in that slot.
	0  - real ok:index valid and its org value is the argumented sysrq_key_op
	-1 - index is valid but its org value is not your argument (sysrq_key_op*)!
		 And the unregister assignment to NULL will not occur.
	   - your index is NOT valid([0,9] || [a,z]) and the unregistered assignment
         to NULL will not occur.	
	*/
	unregister_sysrq_key(SYSRQ_HARDIRQ_TRIGGER_CHAR, &sysrq_dbg_op);
	/*
	filesystem/debugfs.txt
	If this function is passed a pointer for the dentry corresponding to the
	top-level directory, the entire hierarchy below that directory will be
	removed.	
	*/
	debugfs_remove_recursive(d_debug);
	tree_destroy(&cfs_rq->tasks_timeline);
	kfree(cfs_rq);
}

module_init(debug_init);
module_exit(debug_exit);
