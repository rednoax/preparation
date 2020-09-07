#include <string.h>
#include <stdio.h>

int main()
{
	char *t = strtok("/sbin/ubusd", " ");
	int i = 0;
	printf("%d, %s\n", i++, t);
	while (t) {
		t = strtok(NULL, " ");
		printf("%d, %s\n", i++, t);
	}
	return 0;
}