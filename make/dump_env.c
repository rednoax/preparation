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
	printf("dump:pid %d, ppid %d\n", getpid(), getppid());
	if (argc == 1) {
		printf("call system\n");
		system("./dump_env.out dummy");
	} else if (argc == 2 && !strcmp("dummy", argv[1])) {
		char buf[64];
		snprintf(buf, sizeof(buf), "pstree -aps %d", getpid());
/*
                  `-make,8013 -f module.mk myenv2=2
                      `-sh,8014 -c ./dump_env.out dummy>out.out
                          `-dump_env.out,8015 dummy
                              `-sh,8016 -c pstree -aps 8015
                                  `-pstree,8017 -aps 8015
so the system hiarachycal
	./dump_env.out who calls system(./dump_env.out dummy")
		|-> child process "sh" by fork+exec("sh")
			|->child process to exec "pstree"
*/
		system(buf);
	}
	return 0;
}