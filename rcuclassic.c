#include <stdio.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
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
#else	
	.cpumask = {
		[0 ... 1] = -1UL,
		[2] = 0UL,
	},
#endif
};

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
	return 0;
}