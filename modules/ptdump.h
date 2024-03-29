/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2014 ARM Ltd. */
#ifndef __ASM_PTDUMP_H
#define __ASM_PTDUMP_H

#ifdef CONFIG_ARM_PTDUMP_CORE

#include <linux/mm_types.h>
#include <linux/seq_file.h>

struct addr_marker {
	unsigned long start_address;
	char *name;
};

struct ptdump_info {
	struct mm_struct		*mm;
	const struct addr_marker	*markers;
	unsigned long			base_addr;
	int pid;
};

void ptdump_walk_pgd(struct seq_file *s, struct ptdump_info *info);

#ifdef CONFIG_ARM_PTDUMP_DEBUGFS
void ptdump_debugfs_register(struct ptdump_info *info, const char *name);
void ptdump_debugfs_unregister(void);
#else
static inline void ptdump_debugfs_register(struct ptdump_info *info,
					   const char *name) { }
#endif /* CONFIG_ARM_PTDUMP_DEBUGFS */

void ptdump_check_wx(void);

#endif /* CONFIG_ARM_PTDUMP_CORE */

#ifdef CONFIG_DEBUG_WX
#define debug_checkwx() ptdump_check_wx()
#else
#define debug_checkwx() do { } while (0)
#endif
#define init_mm (*(init_task.mm))

#define pt_dump_seq_printf(m, fmt, args...) \
({                      \
	if (m)					\
		seq_printf(m, fmt, ##args);	\
})

#define pt_dump_seq_puts(m, fmt)    \
({						\
	if (m)					\
		seq_printf(m, fmt);	\
})

#endif /* __ASM_PTDUMP_H */
