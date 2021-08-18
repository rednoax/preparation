#include <stdio.h>
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))// + __must_be_array(arr))
typedef unsigned int u32;
typedef u32 pteval_t;
typedef u32 pmdval_t;
struct mem_type {
	pteval_t/*u32*/ prot_pte;//L2-PT's entry's attributs:linux's rather than arm hw's
	pteval_t prot_pte_s2;
	pmdval_t/*u32*/ prot_l1;//L1-PT's entry if it is NOT a section;If 0 then L1 can ONLY use section entry mapping;this field is filled on L1 PT entry directly
	pmdval_t prot_sect;//L1-PT's entry if it IS a section:all hw bits that will be used directly in __map_init_section()
	unsigned int domain;
};
static struct mem_type mem_types[] = {//stale value after build_mem_type_table()
	[0] = {.prot_pte = 0x653, .prot_pte_s2 = 0x0, .prot_l1 = 0x41, .prot_sect = 0x11452, .domain = 0x2}, 
	[1] = {.prot_pte = 0x273, .prot_pte_s2 = 0x0, .prot_l1 = 0x41, .prot_sect = 0x1452, .domain = 0x2}, 
	[2] = {.prot_pte = 0x66f, .prot_pte_s2 = 0x0, .prot_l1 = 0x41, .prot_sect = 0x1045e,    .domain = 0x2}, 
	[3] = {.prot_pte = 0x667, .prot_pte_s2 = 0x0, .prot_l1 = 0x41, .prot_sect = 0x10456, .domain = 0x2}, 
	[4] = {.prot_pte = 0x243,    .prot_pte_s2 = 0x0, .prot_l1 = 0x41, .prot_sect = 0x52, .domain = 0x2}, 
	[5] = {.prot_pte = 0x0, .prot_pte_s2 = 0x0, .prot_l1 = 0x0,    .prot_sect = 0x841e, .domain = 0x0}, 
	[6] = {.prot_pte = 0x0, .prot_pte_s2 = 0x0, .prot_l1 = 0x0, .prot_sect = 0x941a, .domain = 0x0}, 
	[7] = {.prot_pte = 0x4df, .prot_pte_s2 = 0x0, .prot_l1 = 0x61, .prot_sect = 0x0, .domain = 0x3}, 
	[8] = {.prot_pte = 0x5df, .prot_pte_s2 = 0x0,    .prot_l1 = 0x61, .prot_sect = 0x0, .domain = 0x3}, 
	[9] = {.prot_pte = 0x45f, .prot_pte_s2 = 0x0, .prot_l1 = 0x1, .prot_sect = 0x1140e, .domain = 0x0},
	[10] = {.prot_pte = 0x65f, .prot_pte_s2 = 0x0, .prot_l1 = 0x1, .prot_sect = 0x1141e, .domain = 0x0}, 
	[11] = {.prot_pte = 0x0, .prot_pte_s2 = 0x0,    .prot_l1 = 0x0, .prot_sect = 0x940e, .domain = 0x0}, 
	[12] = {.prot_pte = 0x447, .prot_pte_s2 = 0x0, .prot_l1 = 0x1, .prot_sect = 0x10406,    .domain = 0x0}, 
	[13] = {.prot_pte = 0x243, .prot_pte_s2 = 0x0, .prot_l1 = 0x1, .prot_sect = 0x12, .domain = 0x0}, 
	[14] = {.prot_pte = 0x43,    .prot_pte_s2 = 0x0, .prot_l1 = 0x1, .prot_sect = 0x0, .domain = 0x0}, 
	[15] = {.prot_pte = 0x243, .prot_pte_s2 = 0x0, .prot_l1 = 0x1,    .prot_sect = 0x10412, .domain = 0x0}, 
	[16] = {.prot_pte = 0x65f, .prot_pte_s2 = 0x0, .prot_l1 = 0x1, .prot_sect = 0x0, .domain = 0x0}
};

#define __stringify_1(x...)	#x
#define __s(x...)	__stringify_1(x)
const char *index_names[] = {
	__s(MT_DEVICE),// = 0,
	__s(MT_DEVICE_NONSHARED),
	__s(MT_DEVICE_CACHED),
	__s(MT_DEVICE_WC),
	__s(MT_UNCACHED),// = 4,
	__s(MT_CACHECLEAN),
	__s(MT_MINICLEAN),//6 NA for armv7-A
	__s(MT_LOW_VECTORS),
	__s(MT_HIGH_VECTORS),
	__s(MT_MEMORY_RWX),
	__s(MT_MEMORY_RW),//0xa
	__s(MT_ROM),
	__s(MT_MEMORY_RWX_NONCACHED),
	__s(MT_MEMORY_RW_DTCM),
	__s(MT_MEMORY_RWX_ITCM),
	__s(MT_MEMORY_RW_SO),
	__s(MT_MEMORY_DMA_READY),//0x10
};
const char *tex_cb_tre_names[] =  {
	"Strong NCNB  NCNB  InnerS",//strongly-ordered is AL shareable, no matter S in PTE/section is 0 or 1
	"Normal NCNB  NCNB  InnerS",
	"Normal WTnWA WTnWA InnerS",
	"Normal WBnWA WBnWA InnerS",
	"Device NCNB  NCNB  InnerS",
	"Strong NCNB  NCNB  InnerS",
	"***NA",
	"Normal WB+WA WB+WA InnerS",//[7] is what I am interested in!
};
void check()
{
	int i, val;
	struct mem_type *p;
	printf("the following is of index [7]: pte sect");
	for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
		int found = 0;
		p = mem_types + i;
		val = 7<<2;
		if ((p->prot_pte & val) == val) {//[5:2]:L_PTE_MT_XX should be 0111; [11]:Shareable should be 0
			printf("pte:");
			printf("[%d]:%s ", i, p->prot_pte & (1<<10)? "S": "nS");
			found = 1;
		}
		val = (7<<12) | 0xc;
		if ((p->prot_sect & val) == ((1<<12)|0xc)) {//[14:12]:TEX[2:0] should be 0001; [16]:Shareable should be 0
			if (found)
				printf("\t");
			else
				printf("\t\t");
			printf("sect:");
			printf("[%d]:%s\n", i, p->prot_sect & (1<<16)? "S": "nS");
		} else if (found)
			printf("\n");		
	}
	printf("\t\t\t\t      pte(S) sect(S)\n");
	for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
		int j = -1, k = -1, v, s[2] = {-1, -1};
		p = mem_types + i;
		v = p->prot_pte;//linux bits and [10][3:2] are written to arm hw bits also.
		if (v) {
			j = (v >> 2) & 7;//TEX[1]| C | B for pte
			s[0] = (v>>10)&1;
		}
		v = p->prot_sect;
		if (v) {
			k = (((v>>12)&1)<<2)|((v>>2)&3);//TEX[1]| C | B for section
			s[1] = (v>>16)&1;
		}
		if (j != k && (j != -1 && k != -1))
			printf("***1:\n");//an error occurs!
		if (s[0] != s[1] && s[0] != -1 && s[1] != -1)
			printf("***2[%2d]:\n", i);//an error occurs for [15], as its PTE S not set but section S set.It is a false positive as [15] is strongly-ordered, which is AL shared and not affected by PTE/SECTION S bit. If the S bit is 1 or not doesn't matter at all!
		printf("[%2d]%24s: %2d(%2d) %2d(%2d) %s\n", i, index_names[i], j, s[0], k, s[1], tex_cb_tre_names[j > k ? j: k]);
	}
}
int main()
{
	check();
	return 0;
}