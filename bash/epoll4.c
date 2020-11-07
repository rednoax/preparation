#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

#if 10
//to make the console output sequence really conforms to real program sequence, replace printf with directly write to [1]
#define _printf(fmt, arg...) _emit(fmt, ##arg)
#define __printf(fmt, arg...) _emit(fmt, ##arg)
#else
#define _printf(fmt, arg...) printf(fmt, ##arg)
#define __printf(fmt, arg...) _emit(fmt, ##arg)
#endif

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

int emit(const char *fmt, ...)
{
	static char buf[4*1024*1024];
	static int ret = 0;
	int r;
	struct timeval tv;
	va_list args;
	va_start(args, fmt);
	uloop_gettime(&tv);
	ret += snprintf(buf + ret, sizeof(buf) - ret, "[%ld.%04ld]%d:", tv.tv_sec, tv.tv_usec, getpid());
	ret += vsnprintf(buf + ret, sizeof(buf) - ret, fmt, args);
	if (ret > sizeof(buf) - 512) {
		r = write(STDOUT_FILENO, buf, ret);
		(void)r;
		ret = 0;
	}
	va_end(args);
	return ret;
}


int _emit(const char *fmt, ...)
{
	static char buf[10*1024*1024];
	int r;
	static int ret = 0;
	struct timeval tv;
	va_list args;
	va_start(args, fmt);
	uloop_gettime(&tv);
	ret += snprintf(buf + ret, sizeof(buf) - ret, "[%ld.%04ld]%d:", tv.tv_sec, tv.tv_usec, getpid());
	ret += vsnprintf(buf + ret, sizeof(buf) - ret, fmt, args);
	r = write(STDOUT_FILENO, buf, ret);
	if (r == -1) {
		uloop_gettime(&tv);
		ret += snprintf(buf + ret, sizeof(buf) - ret, "***[%ld.%04ld]%d:%d %d\n", tv.tv_sec, tv.tv_usec, getpid(), r, errno);
		errno = 0;
		write(STDOUT_FILENO, buf, ret);
		ret = 0;
	} else
		ret = 0;
	va_end(args);
	return ret;
}

void uloop_sigchld(int signo)
{
	static int count = 0;
/*
even here I change the errno global var, it will be re-written to 4 when epoll_wait returned
*/
	int org = errno++;
	_printf("<=%d %d %d=>%d\n", signo, count++, org, errno);
}

int hit;
int main(int argc, char **argv)
{
	int poll_fd, pid, pipefd[2], ppid;
	struct timeval tv1, tv2;
	struct sigaction act = {
		.sa_handler = uloop_sigchld,
		.sa_flags = 0,
	};
	ppid = getpid();
	(void)ppid;
	poll_fd = epoll_create(32);
	if (poll_fd == -1) {
		_printf("***epoll_create -1: %d\n", errno);
		exit(-1);
	}
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGINT, &act, NULL);
	if (pipe(pipefd) == -1) {
		_printf("***pipe -1: %d\n", errno);
		goto EXIT;
	}
	pid = fork();
	if (pid < 0) {
		_printf("***fork %d: %d\n", pid, errno);
		goto EXIT;
	}
	if (pid) {
		int ret, fl;
		close(pipefd[1]);
#if 0
		fl = fcntl(pipefd[0], F_GETFL, 0);
		fl |= O_NONBLOCK;
		fcntl(pipefd[0], F_SETFL, fl);
#endif
		for (;;) {
			char buf[8];
			uloop_gettime(&tv1);
			_printf("{\n");
			ret = read(pipefd[0], buf, sizeof(buf));
			uloop_gettime(&tv2);
/*
when O_NONBLOCK is set:
first return -1, errno 11;then return 1 for one time; when child exist, parent's read return 0;
[1604731919.618467]3390:{
[1604731919.821744]3392:child exits
[1604731919.618468]3390:-1: err 11}1 us(0 ms)
[1604731919.822145]3390:<=17 0 11=>12<---parent get signal as child exits
***[1604731920.92596]3390:-1 4
[1604731920.92640]3390:{
[1604731920.92823]3390:0: err 0}183 us(0 ms)<--after child's pipe closed, parent's read will return 0
when O_NONBLOCK is not set:
[1604732752.41128]3432:{
[1604732752.41192]3432:1: err 0}67 us(0 ms)
[1604732752.41196]3432:{
[1604732755.41753]3434:child exits
[1604732755.43657]3432:<=17 0 0=>1
[1604732755.43918]3432:0: err 1}3002721 us(3002 ms)
[1604732755.43940]3432:{
[1604732755.43950]3432:0: err 1}10 us(0 ms)
........
*/
			_printf("%d: err %d}%d us(%d ms)\n", ret, errno,	tv_diff(&tv2, &tv1), tv_diff(&tv2, &tv1)/1000);
		}
	} else {
		int t = 3;
		close(pipefd[0]);//r
#if 0
		__printf("close wpipe then start sleep %ds\n", t);
		close(pipefd[1]);//w
#endif
		write(pipefd[1], "1", 1);
		sleep(t);
		__printf("child exits\n");
		exit(-1);
	}
EXIT:
	close(poll_fd);
	return 0;
}
