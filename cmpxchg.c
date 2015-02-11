#include <stdio.h>
static inline unsigned long __xchg(unsigned long x, volatile void *ptr, int size)
{
	return 0;
}
/*
* if parentheses enclosing "typeof(*(ptr))" is ignored like the following:
#define xchg(ptr,x) typeof(*(ptr))__xchg((unsigned long)(x),ptr,sizeof(*(ptr)))
* gcc will report error like the following:
cmpxchg.c: In function 'main':
cmpxchg.c:7:21: error: expected expression before 'typeof'
 #define xchg(ptr,x) typeof(*(ptr))__xchg((unsigned long)(x),ptr,sizeof(*(ptr)))
                     ^
cmpxchg.c:13:12: note: in expansion of macro 'xchg'
  int ret = xchg(&debug_locks, 0);
            ^
*/
#define xchg(ptr,x) (typeof(*(ptr)))__xchg((unsigned long)(x),ptr,sizeof(*(ptr)))

int debug_locks = 1;

int main()
{
	int ret = xchg(&debug_locks, 0);
	printf("%d\n", ret);
	return 0;
}