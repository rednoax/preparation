#include <stdio.h>

struct lockdep_map {//these 3 members always exist in lockdep_map
	void *key;
	void *class_cache;
	const char *name;
};
typedef struct {} raw_spinlock_t;//struct can be empty, i.e. no member at all!

typedef struct{
	raw_spinlock_t raw_lock;//in real spinlock, raw_lock is always there but it can have no member
	struct lockdep_map dep_map;//in real spinlock, there will be no ".dep_map" when !CONFIG_DEBUG_LOCK_ALLOC
}spinlock_t;

typedef struct{
	unsigned sequence;
	spinlock_t lock;
}seqlock_t;

seqlock_t xtime_lock = {//the member not stated here will be 0!
	10,//during assignment sequence, use ',' rather than ';'
	(spinlock_t){//absolutely necessary
		.raw_lock = {},//empty assignment for empty structure
		.dep_map = {.name = "xtime_lock"},
	},//this comma can be ignored!
};

#define SPIN_DEP_MAP_INIT(lockname) .dep_map = { .name = #lockname }

#define __RAW_SPIN_LOCK_UNLOCKED {}
	
#define __SPIN_LOCK_UNLOCKED(lockname) \
	(spinlock_t) { .raw_lock = __RAW_SPIN_LOCK_UNLOCKED,  \
			SPIN_DEP_MAP_INIT(lockname) }

#define __SEQLOCK_UNLOCKED(lockname) \
	{ 0, __SPIN_LOCK_UNLOCKED(lockname) }

#define DEFINE_SEQLOCK(x) \
	seqlock_t x = __SEQLOCK_UNLOCKED(x)
	
DEFINE_SEQLOCK(xtime_lock2);

void dump_seqlock(seqlock_t *pseq)
{
	//seqlock_t *pseq = &xtime_lock;
	spinlock_t *pspin = &pseq->lock;
	struct lockdep_map *pmap = &pspin->dep_map;
	printf("%d %p %p %s\n", pseq->sequence, pmap->key, pmap->class_cache, pmap->name);
}
int main()
{
	dump_seqlock(&xtime_lock);
	dump_seqlock(&xtime_lock2);
	return 0;
}