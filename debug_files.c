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
#include <linux/percpu.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <asm/atomic.h>
#include <asm/bitops.h>

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

//
static unsigned long g_count = 0;

static atomic_t g_atomic_count = ATOMIC_INIT(0);
/*
on SMP vmware test, it nerver gets 2000000 if using "inc" file
*/
static ssize_t
debug_inc_read(struct file *filep, char __user *ubuf,
			size_t cnt, loff_t *ppos)
{
	printk("\nnormal count %ld\n", g_count);
	g_count = 0;
	return 0;//FIXME?
}

static ssize_t
debug_inc_write(struct file *filp, const char __user *ubuf,
					size_t cnt, loff_t *ppos)
{
	g_count++;
	return 1;
}

/*
the app uses "atomic_inc" file need not some implementqtion like "test_and_set_inc" and it will get 2000000
*/
static ssize_t
debug_atomic_inc_read(struct file *filep, char __user *ubuf,
			size_t cnt, loff_t *ppos)
{
	printk("\natomic count %d\n", atomic_read(&g_atomic_count));
	atomic_set(&g_atomic_count, 0);
	return 0;//FIXME?
}

static ssize_t
debug_atomic_inc_write(struct file *filp, const char __user *ubuf,
					size_t cnt, loff_t *ppos)
{
	atomic_inc(&g_atomic_count);
	return 0;
}
static ssize_t
debug_test_and_set_inc_read(struct file *filep, char __user *ubuf,
			size_t cnt, loff_t *ppos)
{
	printk("\ntest_and_set count %ld\n", g_count);
	g_count = 0;
	return 0;//FIXME?
}
static unsigned long state;
static ssize_t
debug_test_and_set_inc_write(struct file *filp, const char __user *ubuf,
					size_t cnt, loff_t *ppos)
{
	/*
	only one CPU will enter the critical section, others fail. 
	It is different with spin lock critical section, where all CPUs will finally enter and no one will bypass.
	The app who uses "test_and_set_inc" file will have to take the lower bypass into account if it want get 2000000.
	*/
	if (!test_and_set_bit(0, &state)) {
		g_count++;
		clear_bit(0, &state);
		return 1;
	}
	return 0;
}

static void self_sched_func(unsigned long);
DECLARE_TASKLET(self_sched, self_sched_func, 0);
static int tasklet_count = 0;
static void self_sched_func(unsigned long arg)
{
	tasklet_schedule(&self_sched);
	tasklet_count++;
	printk("%s:%d\n", __func__, tasklet_count);
}

static ssize_t
debug_tasklet_test_read(struct file *filep, char __user *ubuf,
			size_t cnt, loff_t *ppos)
{
	if (!tasklet_count)
		tasklet_schedule(&self_sched);
	return 0;//FIXME?
}

static ssize_t
debug_tasklet_test_write(struct file *filp, const char __user *ubuf,
					size_t cnt, loff_t *ppos)
{
	tasklet_kill(&self_sched);
	tasklet_count = 0;
	*ppos += cnt;
	return cnt;
}

//
struct wq_struct;
struct wk_struct;

typedef void (*wk_func_t)(struct wk_struct*);

#define WK_STRUCT_PENDING 0
#define WK_STRUCT_FLAG_MASK (3UL)
#define WK_STRUCT_WQ_DATA_MASK (~WK_STRUCT_FLAG_MASK)

#define INIT_WK(_work, _func) \
	do{\
		(_work)->data = (atomic_t)ATOMIC_INIT(0);\
		INIT_LIST_HEAD(&(_work)->entry);\
		(_work)->func = (_func);\
	}while(0)

struct wk_struct {
	atomic_t data;
	struct list_head entry;
	wk_func_t func;
};

struct cpu_wq_struct {
	spinlock_t lock;

	struct list_head worklist;
	wait_queue_head_t more_work;
	struct wk_struct *current_work;

	struct wq_struct *wq;
	struct task_struct *thread;
};

struct wq_struct {
	struct cpu_wq_struct *cpu_wq;
	const char *name;
};

static struct wq_struct *wqp = NULL;

#define wk_data_bits(work) ((unsigned long*)(&(work)->data))
#define wk_clear_pending(work) clear_bit(WK_STRUCT_PENDING, wk_data_bits(work))
#define wk_pending(work) test_bit(WK_STRUCT_PENDING, wk_data_bits(work))

static inline
void set_wq_data(struct wk_struct *work, struct cpu_wq_struct *cwq)
{
	unsigned long new;

	BUG_ON(!wk_pending(work));

	new = *wk_data_bits(work) & WK_STRUCT_FLAG_MASK;
	new |= (unsigned long)cwq |(1UL<<WK_STRUCT_PENDING);
	atomic_set(&work->data, new);
}

struct cpu_wq_struct *get_wq_data(struct wk_struct *work)
{
	return (void*)(atomic_read(&work->data) & WK_STRUCT_WQ_DATA_MASK);
}

static void insert_wk(struct cpu_wq_struct *cwq, struct wk_struct *work, struct list_head *head)
{
	set_wq_data(work, cwq);
	//
	//smp_wmb();
	//
	list_add_tail(&work->entry, head);//work struct is FIFO
	wake_up(&cwq->more_work);
}

static void __queue_wk(struct cpu_wq_struct *cwq, struct wk_struct *work)
{
	unsigned long flags;

	spin_lock_irqsave(&cwq->lock, flags);
	insert_wk(cwq, work, &cwq->worklist);
	spin_unlock_irqrestore(&cwq->lock, flags);
}

//return 0 if @work was already on a queue, non-zero otherwise
int queue_wk_on(int cpu, struct wq_struct *wq, struct wk_struct *work)
{
	int ret = 0;
	/*
	WK_STRUCT_PENDING is cleared before running, out of spin_lock critical section. It is safely a bit longer
	than needed.
	*/
	if (!test_and_set_bit(WK_STRUCT_PENDING, wk_data_bits(work))) {
		BUG_ON(!list_empty(&work->entry));
		__queue_wk(per_cpu_ptr(wq->cpu_wq, cpu), work);
		ret = 1;
	}
	return ret;
}

//return 0 if @work was already on a queue, non-zero otherwise
int queue_wk(struct wq_struct *wq, struct wk_struct *work)
{
	int ret;
	int cpu = get_cpu();
	ret = queue_wk_on(cpu, wq, work);
	put_cpu();
	return ret;
}

static void run_wq(struct cpu_wq_struct *cwq)
{
	spin_lock_irq(&cwq->lock);
	while (!list_empty(&cwq->worklist)) {
		struct wk_struct *work = list_entry(cwq->worklist.next, struct wk_struct, entry);
		wk_func_t fn = work->func;
		list_del_init(&work->entry);
		cwq->current_work = work;
		spin_unlock_irq(&cwq->lock);

		BUG_ON(get_wq_data(work) != cwq);
		wk_clear_pending(work);
		fn(work);
		/*
		if (unlikely(in_atomic())) {
		}
		*/
		spin_lock_irq(&cwq->lock);
		cwq->current_work = NULL;
	}
	spin_unlock_irq(&cwq->lock);
}

static int worker_thread(void *__cwq)
{
	struct cpu_wq_struct *cwq = __cwq;
	DEFINE_WAIT(wait);

	for (;;) {
		prepare_to_wait(&cwq->more_work, &wait, TASK_INTERRUPTIBLE);
		/*
		When talking the sequence , "if" is a serial/batch of instructions; but we only care "wake_up" and "schedule", 
		either of them is of task_rq_lock critical section, so there is only 2 possible sequence:
		1. wake_up is done before schedule, and wake_up return 0=>autoremove_wake_function will 
		   NOT list_del_init(&wait->task_list);
		2. schedule is done before wake_up,and wake_up return 1;=?autoremove_wake_function will
		   list_del_init(&wait->task_list);
		*/
		if (!kthread_should_stop() &&
			list_empty(&cwq->worklist))
			schedule();//can be waken up by queue_work or kthread_stop
		finish_wait(&cwq->more_work, &wait);
		if (kthread_should_stop())
			break;
		run_wq(cwq);
	}

	return 0;
}

int create_wq_thread(struct cpu_wq_struct *cwq, int cpu)
{
	struct wq_struct *wq = cwq->wq;
	const char *fmt = "%s/%d";
	struct task_struct *p;
	/*
	kthread_create return:
	1. kernel_thread(return do_fork) return:
	   a. >0:child PID
	   b. <0:-EINVAL, -EPERM, etc

	2. 1.a=>struct task_struct*
	   1.b=>ERR_PTR
	   both of them can be found by IS_ERR, and the value(1.a/1.b) is assigned to (kthread_create_info*)->result
	   by create_kthread, and .result is returned by kthread_create	
	*/
	p = kthread_create(worker_thread, cwq, fmt, wq, cpu);
	if (IS_ERR(p))
		return PTR_ERR(p);
	cwq ->thread = p;//otherwisr .thread is NULL during alloc_percpu
	return 0;
}

void start_wq_thread(struct cpu_wq_struct *cwq, int cpu)
{
	struct task_struct *p = cwq->thread;
	if (p) {
		if (cpu >= 0)
			kthread_bind(p, cpu);
		wake_up_process(p);
	}
}

//the non-initialized member is zeroed since alloc_percpu is kzalloc!
static struct cpu_wq_struct *init_cpu_wq(struct wq_struct *wq, int cpu)
{
	struct cpu_wq_struct *cwq = per_cpu_ptr(wq->cpu_wq, cpu);

	cwq->wq = wq;
	spin_lock_init(&cwq->lock);
	INIT_LIST_HEAD(&cwq->worklist);
	init_waitqueue_head(&cwq->more_work);
	return cwq;
}

struct wq_barrier
{
	struct wk_struct work;
	struct completion done;
};

void wq_barrier_func(struct wk_struct *work)
{
	struct wq_barrier *barr = container_of(work, struct wq_barrier, work);
	complete(&barr->done);
}

static void insert_wq_barrier(struct cpu_wq_struct *cwq, struct wq_barrier *barr, struct list_head *head)
{
	INIT_WK(&barr->work, wq_barrier_func);
	set_bit(WK_STRUCT_PENDING, wk_data_bits(&barr->work));
	init_completion(&barr->done);
	insert_wk(cwq, &barr->work, head);
}

int flush_cpu_wq(struct cpu_wq_struct *cwq)
{
	int active = 0;
	if (current == cwq->thread) {//some work_struct can run flush_cpu_wq itself
		run_wq(cwq);
		active = 1;
	} else {
		struct wq_barrier barr;
		spin_lock_irq(&cwq->lock);
		if (!list_empty(&cwq->worklist) || cwq->current_work) {
			insert_wq_barrier(cwq, &barr, &cwq->worklist);
			active = 1;
		}
		spin_unlock_irq(&cwq->lock);
		if (active)
			wait_for_completion(&barr.done);
	}
	return active;
}

void cleanup_wq_thread(struct cpu_wq_struct *cwq)
{
	struct task_struct *p = cwq->thread;
	if (!p)
		return;
	flush_cpu_wq(cwq);
	kthread_stop(p);
	cwq->thread = NULL;
}

void destroy_wq(struct wq_struct *wq)
{
	int cpu;
	for_each_online_cpu(cpu) {
		cleanup_wq_thread(per_cpu_ptr(wq->cpu_wq, cpu));
	}
	free_percpu(wq->cpu_wq);
	kfree(wq);
}

struct wq_struct *create_wq(const char* name)
{
	struct wq_struct *wq;
	struct cpu_wq_struct *cwq;
	int cpu, err = 0;

	wq = kzalloc(sizeof(*wq), GFP_KERNEL);
	if (!wq)
		return NULL;
	//alloc_percpu is kzalloc on both CONFIG_SMP & !CONFIG_SMP
	wq->cpu_wq = alloc_percpu(struct cpu_wq_struct);
	if (!wq->cpu_wq) {
		kfree(wq);
		return NULL;
	}
	wq->name = name;

	for_each_online_cpu(cpu) {
		cwq = init_cpu_wq(wq, cpu);
		if (err)
			continue;
		err = create_wq_thread(cwq,cpu);
		start_wq_thread(cwq, cpu);
	}
	if (err) {
		destroy_wq(wq);
		wq = NULL;
	}
	return wq;
}

static ssize_t debug_wq_test_read(struct file *filep, char __user *ubuf, size_t cnt, loff_t *ppos)
{
	return 0;
}

static ssize_t debug_wq_test_write(struct file *filep, const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	char buf[64];
	unsigned long val;

	if (cnt >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, ubuf, cnt))
		return -EFAULT;

	buf[cnt] = 0;
	val = simple_strtoul(buf, NULL, 0);
	switch(val)
	{
	case 0://shutdown
		if (wqp) {
			destroy_wq(wqp);
			wqp = NULL;
		}
		break;
	case 1://setup
		if (!wqp)
			wqp = create_wq("wq_test");
		break;
	default:
		break;
	}
	return cnt;
}
//

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

static const struct file_operations debug_inc_fops = {
	.open	= debug_open_generic,
	.read	= debug_inc_read,
	.write	= debug_inc_write,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_atomic_inc_fops = {
	.open	= debug_open_generic,
	.read	= debug_atomic_inc_read,
	.write	= debug_atomic_inc_write,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_test_and_set_inc_fops = {
	.open	= debug_open_generic,
	.read	= debug_test_and_set_inc_read,
	.write	= debug_test_and_set_inc_write,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_tasklet_test_fops = {
	.open	= debug_open_generic,
	.read	= debug_tasklet_test_read,
	.write	= debug_tasklet_test_write,
	.llseek	= generic_file_llseek,
};

static const struct file_operations debug_wq_test_fops = {
	.open	= debug_open_generic,
	.read	= debug_wq_test_read,
	.write	= debug_wq_test_write,
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
		debug_create_file("inc", 0220, d_debug, cfs_rq, &debug_inc_fops);
		debug_create_file("atomic_inc", 0220, d_debug, cfs_rq, &debug_atomic_inc_fops);
		debug_create_file("test_and_set_inc", 0220, d_debug, cfs_rq, &debug_test_and_set_inc_fops);
		debug_create_file("tasklet_test", 0220, d_debug, cfs_rq, &debug_tasklet_test_fops);
		debug_create_file("wq", 0644, d_debug, NULL, &debug_wq_test_fops);
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
