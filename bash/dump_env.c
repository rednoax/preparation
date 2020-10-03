#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv, char **envp)
{
	char **p = envp;
	const char str[] = "myenv";
	const char *s = str;
	int c = 0;
	if (argc > 1)
		s = argv[1];
	for (; *p; p++) {
		if (!strncmp(*p, s, strlen(s))) {
			printf("%s\n", *p);
			c++;
		}
	}
	if (!c) {
		printf("%s not found\t\t\t", s);
	}
	printf("dump:pid %d, ppid %d\n", getpid(), getppid());
#if 0
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
#endif
	return 0;
}