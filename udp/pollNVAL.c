#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int ch, fd = -1;
	int r, nread = 0;
	struct pollfd fds[2] = {
		[0] = {
			.fd = STDIN_FILENO,
			.events = POLLIN,
		},
		[1] = {
			.events = POLLIN,
		}
	};
	while ((ch = getopt(argc, argv, "nf:")) != -1) {
		switch (ch) {
		case 'f':
			fd = atoi(optarg);
			if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
				printf("try fd > 2\n");
				//exit(1);
			}
			break;
		case 'n':
			nread = 1;
			break;
		default:
			exit(1);
		}
	}
	fds[1].fd = fd;
	while(1) {
		r = poll(fds, 2, -1);
		printf("%d: %x %x\n", r, fds[0].revents, fds[1].revents);
		if (fds[1].revents & POLLNVAL)
			fds[1].fd = -1;
		if (fds[1].revents & POLLIN && !nread) {//if no such read() handle, the next poll() return at once with .revents==POLLI
			char buf[1024];
			int r = read(fds[1].fd, buf, sizeof(buf));
			printf("read %d\n", r);
		}
	}
	return 0;
}
/*poll a unopened fd will return POLNVAL in .revents
$ ./a.out -f 3
1: 0 20

poll stderr can also return POLLIN, -n specify no reading after POLLIN, kernel main.c:init_post() sys_open("/dev/console", O_RDWR, 0) then dup(0) twice
./a.out -f 2 -n
try fd > 2 <--warning, but stderr can also be poll()-ed with POLLIN
  <--[ENTER] is inputed
2: 1 1<--both stdin and stderr has returned with POLLIN, then it loops since they're not handled
2: 1 1
...

poll stderr can also return POLLIN, and it can be read to prevent the next poll() return POLLIN
$ ./a.out -f 2
try fd > 2
 <--[ENTER] is inputed
2: 1 1
read 1
*/
