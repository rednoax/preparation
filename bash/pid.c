#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void getName(int pid)
{
	static char name[64];
	int ret;
	snprintf(name, sizeof(name), "/proc/%d/cmdline", pid);
	int fd = open(name, O_RDONLY);
	if (fd >= 0) {
		ret = read(fd, name, sizeof(name) - 1);
		if (ret > 0)
			name[ret] = 0;
		close(fd);
	}
	printf("%d:%s\n", pid, name);
}

int main(int argc, char **argv)
{
	pid_t pid;
	int ret = 0xa5;
	if (argc > 1)
		getName(atoi(argv[1]));
	pid = waitpid(-1, &ret, WNOHANG);
	printf("waitpid:%d %x\n", pid, ret);//return -1 if no any child belonging to parent
	ret = fork();
	if (!ret) {
		exit(0xAA);
	} else {
		sleep(1);
		pid = waitpid(-1, &ret, WNOHANG);
		printf("waitpid:%d %x %x\n", pid, ret, WIFEXITED(ret));
	}
	ret = fork();
	if (!ret) {
		sleep(10);
	} else {
		ret = 0x99;
		pid = waitpid(-1, &ret, 0);//WNOHANG);
		printf("waitpid:%d %x\n", pid, ret);//return 0 at once if no child has exited, and ret not touched
	}
	return 0;
}
