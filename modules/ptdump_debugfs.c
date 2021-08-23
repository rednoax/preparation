// SPDX-License-Identifier: GPL-2.0
#include <linux/debugfs.h>
#include <linux/seq_file.h>

//#include <asm/ptdump.h>
#include "ptdump.h"

static int ptdump_show(struct seq_file *m, void *v)
{
	struct ptdump_info *info = m->private;

	ptdump_walk_pgd(m, info);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(ptdump);

static int cptdump_show(struct seq_file *m, void *v)
{
	struct ptdump_info *info = m->private;
	cptdump_walk_pgd(m, info);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(cptdump);

static struct dentry *ptdump_debug_root;
void __init ptdump_debugfs_register(struct ptdump_info *info, const char *name)
{
	ptdump_debug_root = debugfs_create_dir("ptdump", NULL);
	debugfs_create_file(name, 0444, ptdump_debug_root, info, &ptdump_fops);
	debugfs_create_file("cPT", 0444, ptdump_debug_root, info, &cptdump_fops);
}

void ptdump_debugfs_unregister()
{
	debugfs_remove(ptdump_debug_root);
}
