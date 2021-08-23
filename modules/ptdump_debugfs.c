// SPDX-License-Identifier: GPL-2.0
#include <linux/debugfs.h>
#include <linux/seq_file.h>

//#include <asm/ptdump.h>
#include "ptdump.h"
#include <linux/sched.h>

static int ptdump_show(struct seq_file *m, void *v)
{
	struct ptdump_info *info = m->private;
	struct task_struct *p = &init_task;
	if (ptdump_info.pid > 0) {
		if (!(p = find_task_by_vpid(ptdump_info.pid)))
			p = current;
	}
	ptdump_info.mm = p->mm;

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
