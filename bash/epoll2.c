#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

void uloop_sigchld(int signo)
{
	printf("<==SIGCHLD: %d\n", signo);
}

void uloop_gettime(struct timeval *tv)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
}

static int tv_diff(struct timeval *t1, struct timeval *t2)
{
	return
		((t1->tv_sec - t2->tv_sec) * 1000000 +
		(t1->tv_usec - t2->tv_usec));// / 1000;
}
/*
./a.out >1.log
*/
int main()
{
	int poll_fd, pid, pipefd[2];
	struct sigaction act = {
		.sa_handler = uloop_sigchld,
		.sa_flags = 0,
	};
	poll_fd = epoll_create(32);
	if (poll_fd == -1) {
		printf("***epoll_create -1: %d\n", errno);
		exit(-1);
	}
	sigaction(SIGCHLD, &act, NULL);
	if (pipe(pipefd) == -1) {
		printf("***pipe -1: %d\n", errno);
		goto EXIT;
	}
	pid = fork();
	if (pid < 0) {
		printf("***fork %d: %d\n", pid, errno);
		goto EXIT;
	}
	if (pid) {
		struct epoll_event ev = { 0 };
		int ret;
		ev.events = EPOLLIN | EPOLLRDHUP;// | EPOLLET;//1|0x2000|(1<<31)
//level triggered fd will make epoll_wait return repeatly once pipe is closed by child process
		ev.data.fd = pipefd[0];
		close(pipefd[1]);
		ret = epoll_ctl(poll_fd, EPOLL_CTL_ADD, pipefd[0], &ev);
		if (ret == -1) {
			printf("***epoll add -1: %d\n", errno);
			goto EXIT;
		}
		for (;;) {
			static struct epoll_event events[10];
			int nfds, evt;
			static int loops = 1;
			(void)loops;
			struct timeval tv1, tv2;
			uloop_gettime(&tv1);
			printf("[%ld.%04ld]{\n", tv1.tv_sec, tv1.tv_usec);
			nfds = epoll_wait(poll_fd, events, 10, -1);
			evt = events[0].events;
			events[0].events = 0xa5;
			uloop_gettime(&tv2);
			printf("[%ld.%04ld]%d: ev %x, errno %d }%d us(%d ms)\n", tv2.tv_sec, tv2.tv_usec, nfds, evt, errno,
				tv_diff(&tv2, &tv1), tv_diff(&tv2, &tv1)/1000);
#if 0
			if (nfds == 1 && (evt & EPOLLHUP) && loops <= 2) {
				ret = epoll_ctl(poll_fd, EPOLL_CTL_MOD, pipefd[0], &ev);
				if (ret == -1) {
					printf("***epoll add -1: %d\n", errno);
					goto EXIT;
				} else
					printf("MOD %d\n", loops++);
			}
#endif
		}
	} else {
		int t = 1;
		close(pipefd[0]);//r
		printf("[%d]close wpipe then start sleep %ds\n", getpid(), t);
		close(pipefd[1]);//w
		sleep(t);
		printf("[%d]child exits\n", getpid());
		exit(-1);
	}
EXIT:
	close(poll_fd);
	return 0;
}
