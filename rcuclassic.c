#include <stdio.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
typedef unsigned long ulong;
struct rcu_ctrlblk {
	long cur, completed, pending;
	ulong gp_start, jiffies_stall;
	int signaled;
	//spinlock_t lock;
	ulong cpumask[3];//DECLARE_BITMAP(CPUMASK, NR_CPUS)
};

struct rcu_ctrlblk rcu_ctrlblk = {
	.cur = -300,
	.completed = -300,
	.pending = -300,
	//.lock = 
#if 0//brace:dakuohao
rcuclassic.c:16:2: warning: braces around scalar initializer [enabled by default]
  .cpumask = {[0 ... 1] = {0UL, 1}},	
#endif
#if 0//either is OK!
	.cpumask = {
		[0] = -1UL,
		[1] = -1UL,
	},
#elif 0	
	.cpumask = {
		[0 ... 1] = -1UL,
		[2] = 0UL,
	},
#else
	.cpumask = {
		-1UL,
	},
#endif
};

#define NR_CPUS 2
#define BITS_TO_LONGS(nr) (((nr) + 8*sizeof(long) - 1)/(8*sizeof(long)))
typedef struct cpumask {
	unsigned long bits[BITS_TO_LONGS(NR_CPUS)];
}cpumask_t;
#define CPU_MASK_ALL (cpumask_t) {{[BITS_TO_LONGS(NR_CPUS)-1] = (1<<NR_CPUS)-1,}}
#if 0
#define CPU_MASK_ALL_PTR (&CPU_MASK_ALL)
#else
#define CPU_MASK_ALL_PTR (&(cpumask_t){.bits = {[0] = ((1<<2)-1)}})
#endif
#define RCU_CTRLBLK (&(struct rcu_ctrlblk){.cur = 1,.cpumask = {[0] = 1,}})
int set_cpus_allowed_ptr(const struct cpumask *newmask, const struct rcu_ctrlblk *blk)
{
	return 0;
}

int main()
{
	int i;
	struct rcu_ctrlblk *p = &rcu_ctrlblk;
	int var[3] = {0, 1, 2};
	for(i = 0; i < ARRAY_SIZE(p->cpumask); i++)
		printf("%d:%lx\n", i, p->cpumask[i]);
	var[0] = var[1] = var[2];
	for(i = 0; i < ARRAY_SIZE(var); i++)
		printf("%d:%x\n", i, var[i]);
	set_cpus_allowed_ptr(CPU_MASK_ALL_PTR, RCU_CTRLBLK);
	return 0;
}