#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

//./a.out 2000
int poll_main(int argc, char **argv)
{
	/*
	int poll(struct pollfd *fds, nfds_t nfds, int timeout);
	poll is similar with select: it waits for one of a set of file descriptors to become ready to perform I/O. 
	It will block until either:
	* a file descriptor becomes ready;
	* the call is interrupted by a signal handler; or
	*the timeout expires.
	fds: The set of file descriptors to be monitored that is an array of structures of the following form:
	struct pollfd {
		int fd;
		short events;//requested events;input parameter, a bitmask specifying the events the app is interested in for .fd
		short revents;//returned events, output parameter indicating the events that actually occurred
	};
	timeout:
	<0:	an infinite timeout
	0:	cause poll to rerturn immediately, even no file descriptors are ready.
	>0:	ms
	*/
	struct pollfd fd[1];
	int ret, nr, timeout = -1;
	fd[0].fd = STDIN_FILENO;
	fd[0].events = POLLIN;
	fd[0].revents = 0;
	char buf[32], out_buf[128];
	if (argc >1)
		timeout = atoi(argv[1]);//2000
	printf("timeout %d ms\n", timeout);
	while(1) {
		ret = poll(fd, 1, timeout);
		switch (ret)
		{
		case -1://On error,-1 is returned and errno is set appropriatly
			printf("poll error: %s\n", strerror(errno));
			break;
		case 0:
			printf("poll timeout %d ms return %d\n", timeout, ret);
			break;
		default:
			printf("poll [%d]: revent %d\n", fd[0].fd, fd[0].revents);
			/*
			0:timeout and no file descriptors were ready
			>0:the number of structure pollfd which have nonzero revents fields (in other words, those descriptors with events or errors reported)
			*/
			//input "abc","def" or "abcdefg"(=>abcd, efg\n), or just a enter(\n)
			nr = read(fd[0].fd, buf, 4);
			buf[nr] = 0;
			snprintf(out_buf, sizeof(out_buf), "read %d bytes:[%s]", nr, buf);
			write(STDOUT_FILENO, out_buf, strlen(out_buf));
			//printf("read %d:[%s]", nr, buf);
			break;
		}
	}
	return 0;
}

static int pipefd[2];

void SIGHUP_handler(int signal)
{
	write(pipefd[0], "1", 1);
	printf("catch sig %d, write 1 to pipefd[0]\n", signal);
}

int socketpair_main(int argc, char **argv)
{
	int ret;//, pipefd[2];
	//TODO:close on exec and non block
	int type = SOCK_STREAM;
	struct pollfd fd[3];
	unsigned int i, j, nr, index = 0;
	char buf[1024], out_buf[1024];
	signal(SIGHUP, SIGHUP_handler);
	/*
	int socketpair(int domain, int type, int protocol, int sv[2])
	The descriptors used in referencing the new sockets are returned in sv[0] and sv[1].  The two sockets are indistinguishable.
	*/
	ret = socketpair(AF_UNIX, type, 0, pipefd);
	if (ret == -1) {//on error, -1 is returned and errno is set appropriately.
		printf("socketpair error: %s\n", strerror(errno));
		exit(1);
	}
	printf("pid %d socketpair:%d %d\n", getpid(), pipefd[0], pipefd[1]);
	//full-duplex test
	for (i = 0; i < 2; i++) {
		fd[i].fd = pipefd[i];
		fd[i].events = POLLIN;
		fd[i].revents = 0;
	}
	fd[i].fd = STDIN_FILENO;
	fd[i].events = POLLIN;
	fd[i].revents = 0;
	for(i = 0; i < 6; i++) {//test 4 times
		ret = poll(fd, ARRAY_SIZE(fd), -1);
		switch(ret)
		{
		case -1:
			//when another termnal run "kill -1 pid", poll will return -1 and errno is set EINTR:A signal occurred before any requested event
			printf("poll error: %d %s\n", errno, strerror(errno));//
			break;
		default:
			printf("poll return %d\n", ret);
			for (j = 0; j < ARRAY_SIZE(fd); j++) {
				struct pollfd *fdp = &fd[j];
				printf("[%d] revents:%d\n", fdp->fd, fdp->revents);
				if (fdp->revents & POLLIN) {
					char *p;
					ret = read(fdp->fd, buf, sizeof(buf));
					buf[ret] = 0;
					p = strstr(buf, "\n");
					if (p)
						p[0] = '\0';
					snprintf(out_buf, sizeof(out_buf), "[%d] get %d bytes:[%s]\n", fdp->fd, ret, buf);
					write(STDOUT_FILENO, out_buf, strlen(out_buf));
					if (fdp->fd == STDIN_FILENO) {
						ret = strlen(buf)?:1;
						nr = write(pipefd[index], buf, ret);
						printf("write to pipe[%d]([%d]) %dB\n", index, pipefd[index], nr);
						index = (index + 1) % 2;
					}
				}
			}
			break;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	socketpair_main(argc, argv);
	poll_main(argc, argv);
	return 0;
}