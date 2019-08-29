#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG_FILE "/sys/kernel/debug/drbg-cavs/builtin"
char buf[1024*1024];
int main(int argc, char *argv[])
{
	int size = atoi(argv[1]);
	int fd = open(DEBUG_FILE, O_RDONLY);
	int ret = 0;
	char *_buf = buf;
/*
The user's read loop will finally call the following func twice:
static ssize_t drbg_builtin_read(struct file *file, char __user *buf,
				 size_t nbytes, loff_t *ppos)
in 1st&2nd calling:
@buf == _buf
@nbytes == size

in 1st calling:
@*ppos ==0, read return 3

in 2nd calling:
*ppos ==ret, read return 0 then loop stops
*/
	while ((ret = read(fd, _buf, size)) != 0) {
		printf("get %d\n", ret);
		_buf += ret;
	}
	close(fd);
	return 0;
}