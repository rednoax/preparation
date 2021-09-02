#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <err.h>
#include <errno.h>
#include <signal.h>

int nflag;

void inthandler(int s)
{
	printf("sig %d\n", s);
	fflush(stdout);
}
/*
conclusion:
O_NONBLOCK: read() will not block. If no data, it returns -1 with errno==EAGAIN(11).
Registered signal like ^c cannot 'interrupt' the read.In mass test I never see read()
return -1 with errno==EINTR. Ignored signal like ^\ will have no any effect.
O_NONBLOCK is not set for socket:
read() will block. Registered signal like ^c can interrupt it, then read() return -1 with
errno==EINTR(4). Ignored signal like ^\ will have no any effect.
*/
int main(int argc, char **argv)
{
	int s, r, ch;
	char buf[1024];
	struct sigaction sa = {
		.sa_flags = 0,//SA_RESTART,//once SA_RESTART set, ^C can never see read() return -1 and EINTR==errno
		.sa_handler = inthandler,
	};
	struct sigaction ign = {
		.sa_flags = 0,
		.sa_handler = SIG_IGN,
	};
	while ((ch = getopt(argc, argv, "nr")) != -1) {
		switch(ch) {
		case 'n':
			nflag = 1;
			break;
		case 'r':
			sa.sa_flags |= SA_RESTART;
			break;
		default:
			printf("%c not supported\n", optopt);
		}
	}
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(1080),//htonl(1080) results port 0, stackoverflow:Another option is to specify port 0 to bind(). That will allow you to bind to a specific IP address (in case you have multiple installed) while still binding to a random port.
		.sin_addr.s_addr = INADDR_ANY,
	};
	/*
	in_port_t port = htonl(1080);
	printf("1080 %x\n", ntohs(port));
	*/
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		err(1, "socket");
	if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		err(1, "bind");
	if (nflag) {
		int orig_flags = fcntl(s, F_GETFL, 0);
		if (fcntl(s, F_SETFL, orig_flags | O_NONBLOCK) == -1) {
			close(s);
			err(1, "fcntl NONBLOCK");
		}
	}
#if 0
/*
search 'EINTR' in `man 7 signal`:
If	a  blocked call to one of the following interfaces is interrupted by a signal handler, then the call is automatically restarted
after the signal handler returns if the SA_RESTART flag was used; otherwise the call fails with the error EINTR:
read(2),...
BUT WHY NO EINTR RETURNED IN REAL WORLD?
`man signal`:
BSD improved on this situation, but unfortunately also changed the semantics of the existing signal() interface while doing so.  On
BSD,  when	a  signal handler is invoked, the signal disposition is not reset, and further instances of the signal are blocked from
being delivered while the handler is executing.  Furthermore, certain blocking system calls are automatically restarted  if  inter‐
rupted by a signal handler (see signal(7)).  The BSD semantics are equivalent to calling sigaction(2) with the following flags:
   sa.sa_flags = SA_RESTART;
The situation on Linux is as follows:
* The kernel's signal() system call provides System V semantics.
* By default, in glibc 2 and later, the signal() wrapper function does not invoke the kernel system call.  Instead, it calls sigac‐
 tion(2) using flags that supply BSD semantics.
$ ldd --version
ldd (Ubuntu GLIBC 2.31-0ubuntu9.2) 2.31
*/
	signal(SIGINT, inthandler);
#else
	if (sigaction(SIGINT, &sa, NULL) == -1)
		err(1, "sigaction");
	if (sigaction(SIGQUIT, &ign, NULL) == -1)
		err(1, "SIGQUIT");
#endif
	printf("pid %d\n", getpid());
	for (;;) {
		r = read(s, buf, sizeof(buf));
		if (r >= 0)
			printf("r %d\n", r);
		else {
			if (!nflag)
				warn("**r %d %d", r, errno);
			else {
				if (errno != EAGAIN)
					warn("***r %d %d", r, errno);//cannot be hit!
				else {
					static int cnt = 0;
					if (++cnt == 7000000) {
						warn("*r %d %d hit %d", r, errno, cnt);//
						cnt = 0;
					}
				}
			}
		}
		fflush(stdout);
	}
	return 0;
}

