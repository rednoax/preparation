// SPDX-License-Identifier: GPL-2.0
#include <linux/debugfs.h>
#include <linux/seq_file.h>

//#include <asm/ptdump.h>
#include "ptdump.h"
#include <linux/pid.h>

static void get_ttbcp15(struct seq_file *m)
{
	unsigned int ttbcr, ttbr[2];
	asm volatile(
		"mrc p15, 0, %0, c2, c0, 2\n\t"
		"mrc p15, 0, %1, c2, c0, 0\n\t"
		"mrc p15, 0, %2, c2, c0, 1\n\t"
		:"=r"(ttbcr), "=r"(ttbr[0]), "=r"(ttbr[1]));
	pt_dump_seq_printf(m, "TTBCR %x, TTBR0/1 %08x %08x\n", ttbcr, ttbr[0], ttbr[1]);
}
static int ptdump_show(struct seq_file *m, void *v)
{
	struct ptdump_info *info = m->private;
	struct task_struct *p = &init_task;
	int pid = info->pid;
	if (pid > 0) {//find_task_vpid/find_task_by_pid_ns can't be used as they are not exported
		if (!(p = pid_task(find_vpid(pid), PIDTYPE_PID)))//from stackoverflow, or use 'struct pid *find_get_pid(pid_t nr)'
			p = current;
	}
	info->mm = p->mm;
	pt_dump_seq_printf(m, "PID %d, task_t* %x, mm %x ", p->pid, p, p->mm);
	get_ttbcp15(m);
	ptdump_walk_pgd(m, info);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(ptdump);

static int pid_get(void *data, u64 *val)
{
	*val = *(u32*)data;
	return 0;
}

static int pid_set(void *data, u64 val)//arch/arm/mach-omap2/pm-debug.c
{
	u32 *ppid = data;
	*ppid = val;
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(pid_fops, pid_get, pid_set, "%llu\n");

static struct dentry *ptdump_debug_root;
void __init ptdump_debugfs_register(struct ptdump_info *info, const char *name)
{
	ptdump_debug_root = debugfs_create_dir("ptdump", NULL);
	debugfs_create_file(name, 0444, ptdump_debug_root, info, &ptdump_fops);
	debugfs_create_file("pid", 0666, ptdump_debug_root, &info->pid, &pid_fops);
}

void ptdump_debugfs_unregister()
{
	debugfs_remove(ptdump_debug_root);
}
