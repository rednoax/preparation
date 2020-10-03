#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
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
	if (argc > 1)
		getName(atoi(argv[1]));
	return 0;
}
