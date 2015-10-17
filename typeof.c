/*
* borrowed from include/linux/percpu-defs.h
*/
/*

*/
#include <stdio.h>
#define DEFINE_PER_CPU_SECTION(type, name, sec) \
	__attribute__((section(".data..percpu" sec))) \
	__typeof__(type) name
#define DEFINE_PER_CPU(type, name) \
	DEFINE_PER_CPU_SECTION(type, name, "")
/*
* use arm architecture's thead_info
*/	
/*

*/
#define smp_processor_id() ({current_thread_info()->flags;})//
#define get_cpu() ({preempt_disable();smp_processor_id();})
#define __get_cpu1() ({preempt_disable();current_thread_info()->flags;})
#define __get_cpu2() ({preempt_disable();(&thread_info)->flags;})
#define __get_cpu3() ({preempt_disable();*&current_thread_info()->flags;})
#define __get_cpu4() ({*&current_thread_info()->flags;})
#define __get_cpu5() (*({preempt_disable();&current_thread_info()->flags;}))
/*
error: expected ';' before '}' token, so the following is wrong:
#define raw_processor_id() ({current_thread_info()->flags})//error!
*/
#define raw_processor_id() ({;current_thread_info()->flags;}) //OK!
/*
if no void before x:
#define raw_smp_processor_id(x) ({x;current_thread_info()->flags;})
then:
warning: statement with no effect [-Wunused-value]
*/
#define raw_smp_processor_id(x) ({(void)x;current_thread_info()->flags;}) //OK!
/*
#define valid_lvalue(x) (*{(void)x;&current_thread_info()->flags;})//error!
if valid_lvalue(array) = 3; then "error: expected expression before '{' token"
*/
#define valid_lvalue(x) (*({(void)x;&current_thread_info()->flags;})) //OK!
struct thread_info{
	unsigned long flags;
	int preempt_count;
	char array[16];
};
static DEFINE_PER_CPU(struct thread_info*[16], infos);
static DEFINE_PER_CPU(infos[0], info);//due to typeof
static DEFINE_PER_CPU(infos, info2);
static DEFINE_PER_CPU(struct thread_info, thread_info);
static struct thread_info* current_thread_info()
{
	return &thread_info;
}

void preempt_disable()
{
	thread_info.preempt_count++;
}

#define offsetof(TYPE, MEMBER) ((unsigned long) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define container_of2(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	preempt_disable(); \
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define container_of3(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	printf("%d-%d\n", sizeof(__mptr), sizeof(*__mptr)); \
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define container_of4(ptr, type, member) ({			\
	const typeof( &((type *)0)->member ) __mptr = (ptr);	\
	printf("%d-%d\n", sizeof(__mptr), sizeof(*__mptr)); \
	(type *)( (char *)__mptr - offsetof(type,member) );})

int main()
{
	int *p = &thread_info.preempt_count;
#if 0
//if using "char *pch = thread_info.array;",container_of3/4 will report:
typeof.c: In function ¡®main¡¯:
typeof.c:74:48: warning: initialization from incompatible pointer type [-Wincompatible-pointer-types]
  const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                                                ^
typeof.c:134:2: note: in expansion of macro ¡®container_of3¡¯
  container_of3(pch, struct thread_info, array)->flags = 7;
  ^
typeof.c:79:48: warning: initialization from incompatible pointer type [-Wincompatible-pointer-types]
  const typeof( &((type *)0)->member ) __mptr = (ptr); \
                                                ^
typeof.c:135:2: note: in expansion of macro ¡®container_of4¡¯
  container_of4(pch, struct thread_info, array)->flags = 8;
  ^
#endif
	/*
	error: expected identifier or ¡®(¡¯ before ¡®[¡¯ token
	struct thread_info*[4] var;
	*/
	struct thread_info *array[4];
	typeof(array) array2;
	typeof(array) array3[2];
	typeof(struct thread_info* [9]) array4;
	__typeof__(struct thread_info [9]) array5;
	/*
	the difference between "typeof( ((type*)0)->member )*" and "typeof( &((type*)0)->member )"
	*/
	//if using "= thread_info.array", then "warning: initialization from incompatible pointer type [-Wincompatible-pointer-types]"
	typeof(thread_info.array) *parry = &thread_info.array;
	typeof(&thread_info.array) parry2 = &thread_info.array;//if no 2nd '&', it will report the same error as above
	char *pch = thread_info.array;
	/*
	warning: assignment from incompatible pointer type [-Wincompatible-pointer-types]
	pch = &thread_info.array;
	*/
	/*
	warning: initialization from incompatible pointer type [-Wincompatible-pointer-types]
	char **ppch = &thread_info.array;
	*/
	(void)pch;
	//(void)ppch;

	printf("%d-%d-%d-%d-%d, %d-%d, %d-%d\n", sizeof array, sizeof array2, \
		sizeof array3, sizeof array4, sizeof array5, sizeof(parry), sizeof(*parry), sizeof(parry2), sizeof(*parry2));
	printf("%d-%d-%d\n", sizeof infos, sizeof info, sizeof info2);
	smp_processor_id() = 1;
	printf("%ld-%ld\n", smp_processor_id(), get_cpu());
	/*
	lvalue required as left operand of assignment
	so u cannot use following "get_cpuX() = 2";
	get_cpu() = 2;//error: lvalue required as left operand of assignment
	__get_cpu1() = 2;//error: lvalue required as left operand of assignment
	__get_cpu2() = 2;//error: lvalue required as left operand of assignment
	__get_cpu3() = 2;//error: lvalue required as left operand of assignment
	*/
	__get_cpu4() = 2;//OK
	printf("%ld\n", __get_cpu1());
	__get_cpu5() = 3;//OK
	printf("%ld\n", __get_cpu2());
	(&thread_info)->flags = 4;//OK, but __get_cpu2 will report error!
	printf("%ld\n", __get_cpu3());
	/*
	error: lvalue required as left operand of assignment
	so "raw_smp_processor_id() = 2;" is error
	*/
	valid_lvalue(array) = 3;
	printf("%ld-%ld-%ld-%ld\n", 
		smp_processor_id(), get_cpu(), raw_processor_id(), raw_smp_processor_id(array));

	container_of(p, struct thread_info, preempt_count)->flags = 5;
	printf("%ld\n", __get_cpu4());
	container_of2(p, struct thread_info, preempt_count)->flags = 6;
	printf("%ld\n", __get_cpu5());
	container_of3(parry, struct thread_info, array)->flags = 7;
	container_of4(parry, struct thread_info, array)->flags = 8;
	return 0;
}
