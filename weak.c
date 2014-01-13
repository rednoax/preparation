/*
gcc -Wall weak.c weak_func.c will overlay local weak function
gcc -Wall weak.c will use local
*/
/*
6.30 Declaring Attributes of Functions
The keyword __attribute__ allows you to specify special attributes when makinga 
declaration. This keyword is followed by an attribute specification inside 
doubleparentheses. You may also specify attributes with '__' preceding and 
following each keyword. This allows you to use them in header files without 
being concerned about a possible macro ofthe same name. 
For example, you may use __noreturn__ instead of noreturn.
*/
#include <stdio.h>
//or __attribute__((__weaked__))
void __attribute__((weak)) arch_dup_task_struct()
{
	printf("weak\n");
}
int main()
{
	arch_dup_task_struct();
	return 0;
}
