#include <stdio.h>
struct irq_domain {
	int i;
	int linear_revmap[];
};
int main()
{
	printf("%lx\n", sizeof(struct irq_domain));//4
	return 0;
}
