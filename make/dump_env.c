#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv, char **envp)
{
	char **p = envp;
	const char str[] = "myenv";
	for (; *p; p++) {
		if (!strncmp(*p, str, strlen(str)))
			printf("%s\n", *p);
	}
	printf("pid %d, ppid %d\n", getpid(), getppid());
	if (argc == 1)
		system("./dump_env.out dummy");
	return 0;
}