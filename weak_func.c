#include <stdio.h>
void arch_dup_task_struct()
{
	printf("%s:%d\n", __func__, __LINE__);
}
