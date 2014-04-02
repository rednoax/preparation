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

int main()
{
	/*
	error: expected identifier or ¡®(¡¯ before ¡®[¡¯ token
	struct thread_info*[4] var;
	*/
	struct thread_info *array[4];
	typeof(array) array2;
	typeof(array) array3[2];
	typeof(struct thread_info* [9]) array4;
	__typeof__(struct thread_info [9]) array5;

	printf("%d-%d-%d-%d-%d\n", sizeof array, sizeof array2, \
		sizeof array3, sizeof array4, sizeof array5);
	printf("%d-%d-%d\n", sizeof infos, sizeof info, sizeof info2);
	smp_processor_id() = 1;
	printf("%ld-%ld\n", smp_processor_id(), get_cpu());
	/*
	lvalue required as left operand of assignment
	so u cannot "get_cpu() = 2";
	*/
	/*
	error: lvalue required as left operand of assignment
	so "raw_smp_processor_id() = 2;" is error
	*/
	valid_lvalue(array) = 3;
	printf("%ld-%ld-%ld-%ld\n", 
		smp_processor_id(), get_cpu(), raw_processor_id(), raw_smp_processor_id(array));
	return 0;
}
