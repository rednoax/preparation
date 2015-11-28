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
	(spinlock_t){//not necessary, the typecast can be removed because here the assignment is static
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

struct list_head {
	struct list_head *next, *prev;
};
#define LIST_HEAD_INIT(name) {&name, &name}
/*
no problem for the following 3
*/
struct list_head list1 = LIST_HEAD_INIT(list1);//{&list1, &list1};
struct list_head list2 = (struct list_head)LIST_HEAD_INIT(list2);//typecast is not necessary since it is static assignment here!
struct list_head list3 = {&list3};//untouched fields are 0 according the sequnce of defination
struct list_head list4 = {.prev = &list4};//untouched fields are 0

int main(int argc, char **argv)
{
	struct list_head *p;
	dump_seqlock(&xtime_lock);
	dump_seqlock(&xtime_lock2);
	p = &list3;
	printf("%p %p\n", p->prev, p->next);
	p = &list4;
	printf("%p %p\n", p->prev, p->next);
	/*
$ gcc -Wall seqlock.c
seqlock.c: In function 'main':
seqlock.c:55:30: error: expected expression before '{' token
 #define LIST_HEAD_INIT(name) {&name, &name}
                              ^
seqlock.c:83:7: note: in expansion of macro 'LIST_HEAD_INIT'
  *p = LIST_HEAD_INIT(list4);
       ^
	*/
	//typecast is necessary, error otherwise like above
	*p = (struct list_head)LIST_HEAD_INIT(list4);
	printf("%p %p\n", p->prev, p->next);
	return 0;
}
