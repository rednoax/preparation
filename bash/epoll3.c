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

/*
./a.out |tee 1.log
[1604539354.386559]4806:{<--parent begin to block in epoll_wait
[1604539354.386586]4808:close wpipe then start sleep 1s<---child close pipe
[1604539354.386634]4806:1: ev 10, err 0 }75 us(0 ms)<--parent epoll_wait return at once after child close pipe as poll-ed pipe becomes avaiable due to EPOLLHUP
[1604539354.386637]4806:{
[1604539354.386638]4806:1: ev 10, err 0 }1 us(0 ms)<---and epoll_wait will continuely repeatly return 1 availabel fd that is EPOLLHUP as the poll-ed pipe is level triggered
......
[1604539355.264139]4806:{
[1604539355.264140]4806:1: ev 10, err 0 }1 us(0 ms)
[1604539355.264141]4806:{
[1604539355.264141]4806:1: ev 10, err 0 }0 us(0 ms)
[1604539355.264142]4806:{
[1604539355.387414]4808:child exits
[1604539355.387564]4806:<==SIGCHLD: 17<--parent rxed SIGCHLD after child process exit()
[1604539355.714269]4806:{
[1604539355.714280]4806:1: ev 10, err 4 }11 us(0 ms)<---note errno is 4 as write(not epoll_wait) is interrupted by signal SIGCHLD, and available fd is still 1 as poll-pipe is level triggered
[1604539355.714281]4806:{
[1604539355.714282]4806:1: ev 10, err 4 }1 us(0 ms)<-epoll_wait interrupted by irq seems never been seen
------
A call to epoll_wait() will block until either:
   • a file descriptor delivers an event;
   • the call is interrupted by a signal handler; or
   • the timeout expires.
SO ONE TIME'S epoll_wait return CAN MEET THE FORMER 2 AT THE SAME TIME:
there is fd event and the call is interrupted by a signal handler. The example of the above show
EPOLLHUP(0x10) in fd is available all the time as level triggered and errno becomes 4(signal handler
interrupts parent)
------
epoll_wait return value:
When successful, epoll_wait() returns the number of file descriptors ready for the requested I/O, or zero if no file descriptor became
ready during the requested timeout milliseconds.
When an error occurs, epoll_wait() returns -1 and errno is set appropriately.<--ie when signal hanlder interrupts
epoll_wait(),...
------
EINTR  The call was interrupted by a signal handler before either (1) any of the requested events occurred or (2) the timeout expired; see signal(7).
*/
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
		struct epoll_event ev = { 0 };
		int ret;
		ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;//1|0x2000|(1<<31)
//level triggered fd will make epoll_wait return 1 repeatly after pipe is closed by child process
		ev.data.fd = pipefd[0];
		close(pipefd[1]);
		ret = epoll_ctl(poll_fd, EPOLL_CTL_ADD, pipefd[0], &ev);
		if (ret == -1) {
			_printf("***epoll add -1: %d\n", errno);
			goto EXIT;
		}
		for (;;) {
			static struct epoll_event events[10];
			int nfds, evt;
			uloop_gettime(&tv1);
			_printf("{\n");
			//errno = 0xa;
			nfds = epoll_wait(poll_fd, events, sizeof(events), -1);
			uloop_gettime(&tv2);
			evt = events[0].events;
#if 10
			_printf("%d: ev %x, err %d}%d us(%d ms) %d\n", nfds, evt, errno,
				tv_diff(&tv2, &tv1), tv_diff(&tv2, &tv1)/1000);
#else
			if (_err != errno || _nfds != nfds || _evt != events[0].events) {
				char buf[4096];
				int ret;
				ret = sprintf(buf, "%d=>%d %d=>%d %d=>%d\n", _nfds, nfds, _evt, events[0].events, _err, errno);
				write(STDOUT_FILENO, buf, ret);
				_err = errno, _nfds = nfds, _evt = events[0].events;
			}
#endif
		}
	} else {
		int t = 1;
		close(pipefd[0]);//r
		uloop_gettime(&tv1);
#if 10
		__printf("close wpipe then start sleep %ds\n", t);
		close(pipefd[1]);//w
#endif
		sleep(t);
		//write(pipefd[1], "1", 1);
		__printf("child exits\n");
		exit(-1);
	}
EXIT:
	close(poll_fd);
	return 0;
}
