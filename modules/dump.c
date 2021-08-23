// SPDX-License-Identifier: GPL-2.0-only
/*
 * Debug helper to dump the current kernel pagetables of the system
 * so that we can see what the various memory ranges are set to.
 *
 * Derived from x86 implementation:
 * (C) Copyright 2008 Intel Corporation
 *
 * Author: Arjan van de Ven <arjan@linux.intel.com>
 */
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/seq_file.h>

#include <asm/domain.h>
#include <asm/fixmap.h>
#include <asm/memory.h>
//#include <asm/ptdump.h>
#include "ptdump.h"
//
#include <linux/module.h>
//
static struct addr_marker address_markers[] = {
#ifdef CONFIG_KASAN//n
	{ KASAN_SHADOW_START,	"Kasan shadow start"},
	{ KASAN_SHADOW_END,	"Kasan shadow end"},
#endif
	{ MODULES_VADDR,	"Modules" },
	{ PAGE_OFFSET,		"Kernel Mapping" },
	{ 0,			"vmalloc() Area" },//[2]:.start_address:0xc2800000
	{ VMALLOC_END,		"vmalloc() End" },
	{ FIXADDR_START,	"Fixmap Area" },
	{ VECTORS_BASE,	"Vectors" },
	{ VECTORS_BASE + PAGE_SIZE * 2, "Vectors End" },
	{ -1,			NULL },
};

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

struct pg_state {
	struct seq_file *seq;
	const struct addr_marker *marker;//address_markers
	unsigned long start_address;
	unsigned level;//@pg_level[]'s index
	u64 current_prot;//L1SE(section entry)/L2PTE's property value that WILL be checked by @pg_level[4]/[3]:[19:0]/[11:0] of L1SE/L2PTE
	bool check_wx;
	unsigned long wx_pages;
	const char *current_domain;//
};

struct prot_bits {
	u64		mask;
	u64		val;
	const char	*set;
	const char	*clear;
	bool		ro_bit;
	bool		nx_bit;
};

static const struct prot_bits pte_bits[] = {
	{
		.mask	= L_PTE_USER,
		.val	= L_PTE_USER,
		.set	= "USR",
		.clear	= "   ",
	}, {
		.mask	= L_PTE_RDONLY,
		.val	= L_PTE_RDONLY,
		.set	= "ro",
		.clear	= "RW",
		.ro_bit	= true,
	}, {
		.mask	= L_PTE_XN,
		.val	= L_PTE_XN,
		.set	= "NX",
		.clear	= "x ",
		.nx_bit	= true,
	}, {
		.mask	= L_PTE_SHARED,
		.val	= L_PTE_SHARED,
		.set	= "SHD",
		.clear	= "   ",
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_UNCACHED,
		.set	= "SO/UNCACHED",
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_BUFFERABLE,
		.set	= "MEM/BUFFERABLE/WC",//without cache,ie non-cachable
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_WRITETHROUGH,
		.set	= "MEM/CACHED/WT",
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_WRITEBACK,
		.set	= "MEM/CACHED/WBRA",//WBRA:write back,read alloc
#ifndef CONFIG_ARM_LPAE
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_MINICACHE,//NA for armv7-A
		.set	= "MEM/MINICACHE",
#endif
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_WRITEALLOC,
		.set	= "MEM/CACHED/WBWA",
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_DEV_SHARED,
		.set	= "DEV/SHARED",
#ifndef CONFIG_ARM_LPAE
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_DEV_NONSHARED,
		.set	= "DEV/NONSHARED",
#endif
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_DEV_WC,//without cache:noon-cachable
		.set	= "DEV/WC",
	}, {
		.mask	= L_PTE_MT_MASK,
		.val	= L_PTE_MT_DEV_CACHED,
		.set	= "DEV/CACHED",
	},
};

static const struct prot_bits section_bits[] = {
#ifdef CONFIG_ARM_LPAE
	{
		.mask	= PMD_SECT_USER,
		.val	= PMD_SECT_USER,
		.set	= "USR",
	}, {
		.mask	= L_PMD_SECT_RDONLY | PMD_SECT_AP2,
		.val	= L_PMD_SECT_RDONLY | PMD_SECT_AP2,
		.set	= "ro",
		.clear	= "RW",
		.ro_bit	= true,
#elif __LINUX_ARM_ARCH__ >= 6//belows
	{
		.mask	= PMD_SECT_APX | PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val	= PMD_SECT_APX | PMD_SECT_AP_WRITE,
		.set	= "    ro",//PL1 RO,PL0 no access
		.ro_bit	= true,
	}, {
		.mask	= PMD_SECT_APX | PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val	= PMD_SECT_AP_WRITE,
		.set	= "    RW",//PL1 RW,PL0:no access
	}, {
		.mask	= PMD_SECT_APX | PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val	= PMD_SECT_AP_READ,
		.set	= "USR ro",//PL1 RW,PL0:RO
	}, {
		.mask	= PMD_SECT_APX | PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val	= PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.set	= "USR RW",//PL1 RW,PL0:RW
	}, {
		.mask	= PMD_SECT_PXN,
		.val	= PMD_SECT_PXN,//1<<0
		.set	= "PXN",
		.clear	= "px ",
	}, {
		.mask	= PMD_SECT_nG,
		.val	= PMD_SECT_nG,//1<<17
		.set	= "nG",
		.clear	= "G ",
	}, {
		.mask	= PMD_SECT_CACHE_MASK,
		.val	= PMD_SECT_UNCACHED,
		.set	= "SO/UNCACHED",
	}, {
		.mask	= PMD_SECT_CACHE_MASK,
		.val	= PMD_SECT_BUFFERED,
		.set	= "MEM/BUFFERABLE/WC",
	}, {
		.mask	= PMD_SECT_CACHE_MASK,
		.val	= PMD_SECT_WT,
		.set	= "MEM/CACHED/WT",
	}, {
		.mask	= PMD_SECT_CACHE_MASK,
		.val	= PMD_SECT_WB,
		.set	= "MEM/CACHED/WBRA",
	}, {
		.mask	= PMD_SECT_CACHE_MASK,
		.val	= PMD_SECT_WBWA,
		.set	= "MEM/CACHED/WBWA",
#else /* ARMv4/ARMv5  */
	/* These are approximate */
	{
		.mask   = PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val    = 0,
		.set    = "    ro",
		.ro_bit	= true,
	}, {
		.mask   = PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val    = PMD_SECT_AP_WRITE,
		.set    = "    RW",
	}, {
		.mask   = PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val    = PMD_SECT_AP_READ,
		.set    = "USR ro",
	}, {
		.mask   = PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.val    = PMD_SECT_AP_READ | PMD_SECT_AP_WRITE,
		.set    = "USR RW",
#endif
	}, {
		.mask	= PMD_SECT_XN,
		.val	= PMD_SECT_XN,
		.set	= "NX",
		.clear	= "x ",
		.nx_bit	= true,
	}, {
		.mask	= PMD_SECT_S,
		.val	= PMD_SECT_S,//1<<16
		.set	= "SHD",
		.clear	= "   ",
	},
};

struct pg_level {
	const struct prot_bits *bits;
	size_t num;
	u64 mask;//for section/L2PTE:it's some bits'or in the range of [19:0]/[11:0], ie the bits except (the phy address at the MSB); These bits are property of a section/L2PTE
	const struct prot_bits *ro_bit;
	const struct prot_bits *nx_bit;
};

static struct pg_level pg_level[] = {
	{
	}, { /* pgd */
	}, { /* p4d */
	}, { /* pud [3]*/
	}, { /* pmd [4]*/
		.bits	= section_bits,
		.num	= ARRAY_SIZE(section_bits),
	}, { /* pte [5]*/
		.bits	= pte_bits,
		.num	= ARRAY_SIZE(pte_bits),
	},
};

static void dump_prot(struct pg_state *st, const struct prot_bits *bits, size_t num)
{
	unsigned i;

	for (i = 0; i < num; i++, bits++) {
		const char *s;

		if ((st->current_prot & bits->mask) == bits->val)
			s = bits->set;
		else
			s = bits->clear;

		if (s)
			pt_dump_seq_printf(st->seq, " %s", s);
	}
}

static void note_prot_wx(struct pg_state *st, unsigned long addr)
{
	if (!st->check_wx)
		return;
	if ((st->current_prot & pg_level[st->level].ro_bit->mask) ==
				pg_level[st->level].ro_bit->val)
		return;
	if ((st->current_prot & pg_level[st->level].nx_bit->mask) ==
				pg_level[st->level].nx_bit->val)
		return;

	WARN_ONCE(1, "arm/mm: Found insecure W+X mapping at address %pS\n",
			(void *)st->start_address);

	st->wx_pages += (addr - st->start_address) / PAGE_SIZE;
}
/*examples:
i:[0..2048):
addr:1M/2M aligned virt address.Its [31:21] index Should==i
level:@pg_level[]'s index:[4,5]is section,L2PTE; [0,3] are all 0
val:the last level PTE of @addr,If it is 2 level,val is L2 PTE;If it is 1 level, val is L1PTE(possible==0)
1)If @addr's L1PTE is section or Invalid(L1PTE==0), use L1PTE;
If @addr is 2M aligned: (mm_struct*)->pgd[i][0],it is either 0, or a valid SE(section entry)
If @addr is 1M aligned,not 2M aligned: (mm_struct*)->pgd[i][1],it CAN't be 0 but a valid SE(section entry)
2)If @addr's L1PTE points to L2PTE, use L2PTE.These L2 PTE is in [0](256 entries each) or [1](256 entries each) of (a page consisting of [0,3]).
So they are all linux PTE rather than arm hw PTE.
domain:string corresponding to @val's domain fields;val can be a section entry or L1PTE
pointing to a L2PT.All domain fields are [8:5].
--
Once find the record need changing, this func print old in record first then
overlay the record with new start, @val/domain all belongs to and comes from @addr */
static void note_page(struct pg_state *st, unsigned long addr,
		      unsigned int level, u64 val, const char *domain)
{
	static const char units[] = "KMGTPE";
	u64 prot = val & pg_level[level].mask;//[3].mask:0,it can introduce a wrong prot

	if (!st->level) {// 1/2
		st->level = level;
		st->current_prot = prot;
		st->current_domain = domain;
		pt_dump_seq_printf(st->seq, "---[ %s ]---\n", st->marker->name);//address_markers
	} else if (prot != st->current_prot || level != st->level ||
		   domain != st->current_domain ||
		   addr >= st->marker[1].start_address) {//2/2.IF 1 of 4 last arguments changes
		const char *unit = units;
		unsigned long delta;

		if (st->current_prot) {//this if NERVER use @prot argument,which may be a wrong value
			note_prot_wx(st, addr);
			pt_dump_seq_printf(st->seq, "0x%08lx-0x%08lx   ",
				   st->start_address, addr);

			delta = (addr - st->start_address) >> 10;
			while (!(delta & 1023) && unit[1]) {//! higher than &&
				delta >>= 10;
				unit++;
			}
			pt_dump_seq_printf(st->seq, "%9lu%c", delta, *unit);
			if (st->current_domain)
				pt_dump_seq_printf(st->seq, " %s",
							st->current_domain);
			if (pg_level[st->level].bits)
				dump_prot(st, pg_level[st->level].bits, pg_level[st->level].num);
			pt_dump_seq_printf(st->seq, "\n");
		}

		if (addr >= st->marker[1].start_address) {
			st->marker++;
			pt_dump_seq_printf(st->seq, "---[ %s ]---\n",
							st->marker->name);
		}
		st->start_address = addr;
		st->current_prot = prot;
		st->current_domain = domain;
		st->level = level;
	}
}
/*1M memory can be mapped in one level or 2 levels. L1PTE is SE for the former and PTE for the latter.
Besides, the latter need one L2PT that is 4KB ie one page. Compared with the former, an extra 4KB memory
is needed by you can implement more fine memory management: nG/S/AP[2:0]/TEX[0],C,B/xN is only assigned
for 1M memory for the former but can be assigned to 4KB for the latter.Of course, the cost is a 4KB L2PT is consumed!
NOTE domain can only be applied to the whole 1M memory.
--
pmd:(mm_struct*)->pgd+i,i:[0,2048); AL "pmdval_t pgd_t[2]"'s &pgd_t[0],not &pgd_t[1];
start:virt address whose [31:21] index Should==i((mm_struct*)->pgd[2048]'s index that
@pmd belonging to);but NOTE there is no sw checking to enfore the equivality
domain:*pmd is L1PTE and it is AL PTE, not SE; a string corresponding to (*pmd's domain field[8:5])*/
static void walk_pte(struct pg_state *st, pmd_t *pmd, unsigned long start,
		     const char *domain)
{
	pte_t *pte = pte_offset_kernel(pmd, 0);//pte AL a page's beginning address that is 4KB aligned
	unsigned long addr;
	unsigned i;

	for (i = 0; i < PTRS_PER_PTE; i++, pte++) {//[0,512),4KB each; So 2MB totally;at last 512 entries in [0,1] among 1024 entries in [0,3] of a page are iterated
		addr = start + i * PAGE_SIZE;
		note_page(st, addr, 5, pte_val(*pte)/* *pte */, domain);
	}
}

static const char *get_domain_name(pmd_t *pmd)//*pmd can be 1 L1 PTE or section entry, both of their [8:5] are Domain fields
{
#ifndef CONFIG_ARM_LPAE //          & higher than &&
	switch (pmd_present(*pmd) && pmd_val(*pmd) & PMD_DOMAIN_MASK) {//[8:5] of *pmd, ARM hw bit
	case PMD_DOMAIN(DOMAIN_KERNEL):
		return "KERNEL ";
	case PMD_DOMAIN(DOMAIN_USER):
		return "USER   ";
	case PMD_DOMAIN(DOMAIN_IO)://0
		return "IO     ";
	case PMD_DOMAIN(DOMAIN_VECTORS):
		return "VECTORS";
	default:
		return "unknown";
	}
#endif
	return NULL;
}
//The whole func JUST check 2M,no more no less
static void walk_pmd(struct pg_state *st, pud_t *pud, unsigned long start)
{
	pmd_t/*u32*/ *pmd = pmd_offset(pud, 0);//pud<=pgd_t*,eg init_mm.pgd+i,i:[0,2048),@pmd here is AL "pmdval_t pgd_t[2]"'s &pgd_t[0],not &pgd_t[1];@start's [31:21] index Should==i((mm_struct*)->pgd[2048]'s index that @pmd belonging to);but NOTE there is no sw checking to enfore the equivality
	unsigned long addr;
	unsigned i;
	const char *domain;

	for (i = 0; i < PTRS_PER_PMD; i++, pmd++) {//[0,1)
		addr = start + i * PMD_SIZE/*2M*/;//AL addr=start
/*from alloc_init_pmd/alloc_init_pte:CAN ONLY be 3 COMBINATIONs:
a.pmd[0]:points to L2 PT(4KB page's base addr+2KB offset's address),pmd[1]:pmd[0]+1KB;
1)It's possible that all PTE of (L2 PT pointed by pmd[0]) are 0 and all PTE of (L2 PT pointed by pmd[1])
are non zero, ie only 1M memory is mapped, which likes c.The reason why the 1M mem is not mapped as section
is dute to .prot_sect==0, see alloc_init_pmd().
b.pmd[0,1]:all section;
c.pmd[0]:0,pmd[1]:section entry; ie only 1M mempry is mapped
No things like: pmd[0] is L2 PT address and pmd[1] is section, OR VICE VERSA!*/
		domain = get_domain_name(pmd);//Domain field only exists in L1-PTE/SE [8:5], no L2 PTE.
#if 0
		if (pmd_none(*pmd) || pmd_large(*pmd) || !pmd_present(*pmd))//!*pmd||*(pmd)&2/*is section*/||!*pmd
			note_page(st, addr, 4, pmd_val(*pmd), domain);/*&st,kernel_ptdump_info->base_addr+i*2MB,
								3,init_mm.pgd[i][0], domain string of init_mm.pgd[i]); i:[0..2048)*/
		else
			walk_pte(st, pmd, addr, domain);

		if (SECTION_SIZE < PMD_SIZE && pmd_large(pmd[1])) {//1M<2M && pmd[1]&2/*is section*/
			addr += SECTION_SIZE;
			pmd++;
			domain = get_domain_name(pmd);
			note_page(st, addr, 4/*section*/, pmd_val(*pmd)/**pmd*/, domain);
		}
#else
		if (pmd_present(*pmd) && !pmd_large(*pmd))
			walk_pte(st, pmd, addr, domain);
		else {
			note_page(st, addr, 4, pmd_val(*pmd), domain);
			if (SECTION_SIZE < PMD_SIZE) {
				addr += SECTION_SIZE;
				pmd++;
				domain = get_domain_name(pmd);
				note_page(st, addr, 4, pmd_val(*pmd), domain);
			}
		}
#endif
	}
}
//a dummy wrapper to walk_pmd(st,p4d,start)
static void walk_pud(struct pg_state *st, p4d_t *p4d, unsigned long start)
{
	pud_t *pud = pud_offset(p4d, 0);//p4d
	unsigned long addr;
	unsigned i;

	for (i = 0; i < PTRS_PER_PUD; i++, pud++) {//[0,1)
		addr = start + i * PUD_SIZE;//addr=start
		if (!pud_none(*pud)) {//(!0)
			walk_pmd(st, pud, addr);//AL
		} else {
			note_page(st, addr, 3, pud_val(*pud), NULL);
		}
	}
}
/*a dummy wrapper to walk_pud(st,pgd,start)*/
/*NOTE @start DETERMINS @pgd(@pgd is a pmdval_t[2]'s &[0], indexed by @start's [31:21])<--wrong!?*/
static void walk_p4d(struct pg_state *st, pgd_t *pgd, unsigned long start)
{
	p4d_t *p4d = p4d_offset(pgd, 0);//pgd
	unsigned long addr;
	unsigned i;

	for (i = 0; i < PTRS_PER_P4D; i++, p4d++) {//[0,1)
		addr = start + i * P4D_SIZE;//addr=start
		if (!p4d_none(*p4d)) {
			walk_pud(st, p4d, addr);//AL
		} else {
			note_page(st, addr, 2, p4d_val(*p4d), NULL);
		}
	}
}

static void walk_pgd(struct pg_state *st, struct mm_struct *mm,
			unsigned long start)//start SEEM can't be a non-0 value
{
	pgd_t *pgd = pgd_offset(mm, 0UL);//&mm->pgd[0]
	unsigned i;
	unsigned long addr;

	for (i = 0; i < PTRS_PER_PGD; i++, pgd++) {//[0,2048), 2MB mapped each
		addr = start + i * PGDIR_SIZE/*2MB*/;
		if (!pgd_none(*pgd)) {//AL (!0)
			walk_p4d(st, pgd, addr);//AL here
		} else {
			note_page(st, addr, 1, pgd_val(*pgd), NULL);
		}
	}
}
//info:kernel_ptdump_info
void ptdump_walk_pgd(struct seq_file *m, struct ptdump_info *info)
{
	struct pg_state st = {
		.seq = m,
		.marker = info->markers,//address_markers
		.check_wx = false,
		.level = -1,//not 0
	};

	walk_pgd(&st, info->mm, info->base_addr);
	note_page(&st, 0, 0, 0, NULL);
}

static void __init ptdump_initialize(void)
{
	unsigned i, j;

	for (i = 0; i < ARRAY_SIZE(pg_level); i++)
		if (pg_level[i].bits)
			for (j = 0; j < pg_level[i].num; j++) {
				pg_level[i].mask |= pg_level[i].bits[j].mask;
				if (pg_level[i].bits[j].ro_bit)
					pg_level[i].ro_bit = &pg_level[i].bits[j];
				if (pg_level[i].bits[j].nx_bit)
					pg_level[i].nx_bit = &pg_level[i].bits[j];
			}
#ifdef CONFIG_KASAN
	address_markers[4].start_address = VMALLOC_START;
#else
	address_markers[2].start_address = VMALLOC_START;//0xc280 0000
#endif
}

static struct ptdump_info ptdump_info = {
	//.mm = &init_mm,
	.markers = address_markers,
	.base_addr = 0,
};

void ptdump_check_wx(void)
{
	struct pg_state st = {
		.seq = NULL,
		.marker = (struct addr_marker[]) {
			{ 0, NULL},
			{ -1, NULL},
		},
		.check_wx = true,
	};

	walk_pgd(&st, &init_mm, 0);
	note_page(&st, 0, 0, 0, NULL);
	if (st.wx_pages)
		pr_warn("Checked W+X mappings: FAILED, %lu W+X pages found\n",
			st.wx_pages);
	else
		pr_info("Checked W+X mappings: passed, no W+X pages found\n");
}

static int __init ptdump_init(void)
{
	ptdump_initialize();
	ptdump_debugfs_register(&ptdump_info, "pt");
	return 0;
}
//__initcall(ptdump_init);

static void ptdump_exit(void)
{
	ptdump_debugfs_unregister();
}

module_init(ptdump_init);
module_exit(ptdump_exit);
MODULE_LICENSE("GPL");

