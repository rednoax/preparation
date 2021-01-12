#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv, char **envp)
{
	int i;
	for (i = 0; argv[i] != NULL; i++) {
		printf("%d: %s\n", i, argv[i]);
	}
	printf("VVVVARVVV pid %d,ppid %d\n", getpid(), getppid());
	sleep(30);
	for (i = 0; envp[i] != NULL; i++) {
		printf("%d: %s\n", i, envp[i]);
	}
	return 0;
}
