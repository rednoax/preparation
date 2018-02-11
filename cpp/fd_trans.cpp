#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define ENV_KEY "fd"
int main(int argc, char **argv, char **envp)
{
	int nr, fd;
	char buf[32];
	char *genv[32] = {0};	
	int pid, status = -1;
	char arg[] = "--reinit";
	printf("argc %d, argv[1] '%s', envp[0] %s\n", argc, argv[1], envp[0]);
	if (argc > 1 && !strcmp(argv[1], arg)) {
		int key_len = strcspn(envp[0], "=");
		const char *val = getenv(ENV_KEY);
		fd = atoi(&envp[0][key_len + 1]);
		printf("fd via strcspn is %d, via getenv %ld\n", fd, strtol(val, NULL, 10));
		nr = read(fd, buf, 2);
		buf[nr] = 0;
		printf("PPID %d, PID %d read [%d] %dB after execve: '%s'\n", getppid(), getpid(), fd, nr, buf);
		return fd;
	}
	pid = fork();
	if (pid == 0) {
		fd = open("Makefile", O_RDONLY);
		nr = read(fd, buf, 2);
		buf[nr] = 0;
		printf("PID %d read [%d] %dB before execve: '%s'\n", getpid(), fd, nr, buf);
		asprintf(&genv[0], "%s=%d", ENV_KEY, fd);
		genv[1] = 0;
		fcntl(fd, F_SETFD, 0);//can be removed since close-on-exec is not set by default
		argv[1] = arg;
		argv[2] = NULL;
		printf("execve %s %s %s\n", argv[0], argv[1], genv[0]);
		if (execve(argv[0], argv, genv) < 0 ) {
			perror("execve error:");
			exit(127);
		}
	} else if (pid > 0)
		printf("parent %d, child %d\n", getpid(), pid);
	else
		perror("fork error:");
	wait(&status);
	if (WIFEXITED(status)) {
		fd = WEXITSTATUS(status);
		printf("child %d exit fin 0x%x\n", pid, status);
		nr = read(fd, buf, 2);
		buf[nr] = 0;
		printf("PID %d read [%d] %dB after clild exit: '%s'\n", getpid(), fd, nr, buf);
	}
	free(genv[0]);
	return 0;
}