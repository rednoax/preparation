#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv, char **envp)
{
	int i;
	for (i = 0; argv[i] != NULL; i++) {
		printf("%d: [%s]\n", i, argv[i]);
	}
#if 10
	printf("VVVVARVVV pid %d,ppid %d\n", getpid(), getppid());
	sleep(30);
	for (i = 0; envp[i] != NULL; i++) {
		printf("%d: %s\n", i, envp[i]);
	}
#endif
	return 0;
}
/*
if no export in setting PATH before launch make, make can work!
to present in a simple test:
$ PATH=$PATH:~/arm-2013.11/bin
then ./dump.out's env will show changed PATH like the above
*/
