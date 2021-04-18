#define _GNU_SOURCE//cater for 'warning: implicit declaration of function ‘accept4’'
#include <stdio.h>
#include <string.h>//memset()
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>//int getaddrinfo()
#include <netdb.h>//getaddrinfo(), gai_strerror()
#include <arpa/inet.h>//inet_ntop()
#include <errno.h>//for global var errno
#include <err.h>//err() errx()
#include <poll.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <limits.h>

#define BUFSIZE 16384
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
int bflag, kflag, lflag, uflag, qflag = -1;
int nflag;//Do not do any DNS or service lookups on any specified addresses, hostnames or ports.
char *pflag, *sflag;
int timeout = -1;
int family = AF_UNSPEC;
char *portlist[1];
enum {
	POLL_STDIN = 0,
	POLL_NETOUT,
	POLL_NETIN,
	POLL_STDOUT,
};

enum {
	CONNECTION_SUCCESS,
	CONNECTION_FAILED,
	CONNECTION_TIMEOUT
};
void usage(int ret)
{
	fprintf(stderr,
		"usage: nc [-klu]\n"
		"\t  [destination] [port]\n");
	if (ret)
		exit(1);//`man 3 exit` show <stdlib.h> should be included.
}

void show_addrinfo(const struct addrinfo *res)
{
	char buf[INET6_ADDRSTRLEN];
	void *addr = NULL;
	const char *s;
	in_port_t port;
	sa_family_t fm = -1;
	printf("flags:");
	if (res->ai_flags == 0) {
		printf(" 0");
	} else {
		if (res->ai_flags & AI_PASSIVE)
			printf(" passive");
		if (res->ai_flags & AI_CANONNAME)
			printf(" canon");
		if (res->ai_flags & AI_NUMERICHOST)
			printf(" numhost");
		if (res->ai_flags & AI_NUMERICSERV)
			printf(" numserv");
		if (res->ai_flags & AI_V4MAPPED)
			printf(" v4mapped");
		if (res->ai_flags & AI_ALL)
			printf(" all");
	}
	printf("\nfamily:");
	switch (res->ai_family) {
	case AF_INET:
		printf(" inet");
		break;
	case AF_INET6:
		printf(" inet6");
		break;
	case AF_UNIX:
		printf(" unix");
		break;
	case AF_UNSPEC:
		printf(" unspecified");
		break;
	default:
		printf(" unknown");
	}
	printf("\ntype:");
	switch (res->ai_socktype) {
	case SOCK_STREAM:
		printf(" stream");
		break;
	case SOCK_DGRAM:
		printf(" datagram");
		break;
	case SOCK_SEQPACKET:
		printf(" seqpacket");
		break;
	case SOCK_RAW:
		printf(" raw");
		break;
	default:
		printf(" unknow (%d)", res->ai_socktype);
	}
	printf("\nprotocol:");
	switch (res->ai_protocol) {
	case 0:
		printf(" default");
		break;
	case IPPROTO_TCP:
		printf(" tcp");
		break;
	case IPPROTO_UDP:
		printf(" udp");
		break;
	case IPPROTO_RAW:
		printf(" raw");
		break;
	default:
		printf(" unknown (%d)", res->ai_protocol);
		break;
	}
 	printf("\ncanon name: %s\t", res->ai_canonname);
	//generic sockaddr* needs to be interpreted to be a sockaddr_XX* based on its family
	if (res->ai_family == AF_INET) {
		struct sockaddr_in *sinp = (struct sockaddr_in*)res->ai_addr;
		fm = sinp->sin_family;
		addr = &sinp->sin_addr;
		port = ntohs(sinp->sin_port);
	} else if (res->ai_family == AF_INET6) {
		struct sockaddr_in6 *sinp = (struct sockaddr_in6*)res->ai_addr;
		fm = sinp->sin6_family;
		addr = &sinp->sin6_addr;
		port = ntohs(sinp->sin6_port);
	}
	s = inet_ntop(res->ai_family, addr, buf, sizeof(buf));
	if (s == buf)
		printf("%s:%d\t assert %d==%d\n", buf, port, fm, res->ai_family);
	else if (s == NULL)//when error, s == 0
		err(1, "inet_ntop error");
}

void set_common_sockopts(int s, struct sockaddr *sa)
{
	int x = 1;
	if (bflag) {
		if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &x, sizeof(x)) == -1)//if err, the return value needs not to be reserved as err() 's err info is strerror(errno) whose arg is global var!
			err(1, "set SO_REUSEADDR error");
	}
}
/*eg:
$ ./a.out -nl 1080
flags: passive numhost
family: inet
type: stream
protocol: tcp
canon name: (null)      0.0.0.0:1080
Listeing on 0.0.0.0:1080<--

comparing with:

$ ./a.out -l 1080
flags: passive
family: inet
type: stream
protocol: tcp
canon name: (null)      0.0.0.0:1080
a.out: getnameinfo:Temporary failure in name resolution: Success<--
Listeing on :
*/
void report_sock(const char *msg, const struct sockaddr *sa, socklen_t salen, char *path)
{
	char host[NI_MAXHOST] = {0}, port[NI_MAXSERV] = {0};
	int herr, flags = NI_NUMERICSERV;//Return the numeric form of the host address instead of the name to avoid the inefficiency of unnecessarily searching /etc/services.
	//getnameinfo is the converse of getaddrinfo()
	if (nflag)
		flags |= NI_NUMERICHOST;//Return the numeric form of the host address instead of the name to avoid a possibly time-consuming call to the DNS server
	herr = getnameinfo(sa, salen, host, sizeof(host), port, sizeof(port), flags);
	if (herr == EAI_SYSTEM)//system error returned in @errno
		warn("getnameinfo");
	else if (herr)
		warnx("getnameinfo:%s", gai_strerror(herr));
	fprintf(stderr, "%s on %s:%s\n", msg, host, port);
}

int local_listen(const char *host, const char *port, struct addrinfo hints)
{
	int s = -1, ret = 0, x = 1;
	int error;
	struct addrinfo *res, *res0;
	hints.ai_flags |= AI_PASSIVE;//allow @host name to be NULL(if so then ip member of POed res0->ai_addr below will be INADDR_ANY or IN6ADDR_ANY_INIT, according to the domain), if @host!=NULL,AI_PASSIVE has no effect
	if (host == NULL && hints.ai_family == AF_UNSPEC)
		hints.ai_family = AF_INET;
	if ((error = getaddrinfo(host, port, &hints, &res0)))
//1.The errx() and warnx() functions do not append an error message.
//2.The err(), verr(), warn(), and vwarn() functions append an error message obtained from strerror(3) based on the global variable errno
//3.perror, strerror should not be used when getaddrinfo fails. gai_strerror() should be used.
		errx(1, "getaddrinfo: %s", gai_strerror(error));
/*
`./a.out -l lucia 1080` return 127.0.0.1 if there is "127.0.0.1 lucia" in /etc/hosts
`./a.out -l repo.or.cz ssh` can return both ipv4 & ipv6 address by dns, why the port is 22, see below
$ ./a.out -l repo.or.cz ssh
flags: passive
family: inet
type: stream<--
protocol: tcp
canon name: (null)      195.113.20.142:22
flags: passive
family: inet6<--ipv6
type: stream<--not datagram, both ipv4 & ipv6 is stream
protocol: tcp
canon name: (null)      2001:718:1e03:801::8e:22
`./a.out -l ssh` return 0.0.0.0 22 as /etc/services include entry "ssh 22/tcp"
$ ./a.out -ln baidu.com 1080<- show '-n' usage:'Do not do any DNS or service lookups on any specified addresses, hostnames or ports.'
a.out: getaddrinfo: Name or service not known
*/
	for (res = res0; res; res = res->ai_next) {
		show_addrinfo(res);
		//if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol) < 0))//compareing like '<' has precedentce over '=', bind(0,) cause 'Socket operation on non-socket'
		if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
			continue;
		//s = -1;//to test err after setsockopt fails
#if 0/*
1.if launch server=>client connect=>server quit by ^c:
$ netstat -nap|grep 1080
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
tcp 	   0	  0 127.0.0.1:1080			127.0.0.1:58428 		FIN_WAIT2	-  <--local port 1080
tcp 	   0	  0 127.0.0.1:58428 		127.0.0.1:1080			CLOSE_WAIT	4744/./a.out
rednoah@lucia:~/preparation/udp$ ./a.out -ln 1080<--even `nc -l 1080` run here will fail with 'nc: Address already in use'
flags: passive numhost
family: inet
type: stream
protocol: tcp
canon name: (null)		0.0.0.0:1080	 assert 2==2
a.out: bind err: Address already in use<--
a.out: setup server error
2.if run the same sequence as the above for standard nc:
$ netstat -nap|grep 1080
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
tcp        0      0 127.0.0.1:1080          127.0.0.1:58472         TIME_WAIT   -<---not FIN_WAIT2,why?
rednoah@lucia:~$ nc -l 1080<--ok
*/
		ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(x));
#endif
/*
The  err()	and  warn() family of functions display a formatted error message on the standard error output.  In all cases, the last
component of the program name, a colon character, and a space are output.  If the fmt argument is not NULL, the printf(3)-like for‐
matted error message is output.  The output is terminated by a newline character.

The err(), verr(), warn(), and vwarn() functions append an error message obtained from strerror(3) based on the global variable er‐
rno, preceded by another colon and space unless the fmt argument is NULL.

The errx() and warnx() functions do not append an error message.
*/
		if (ret == -1)
			err(1, NULL);//if s==-1:"a.out: Bad file descriptor"
			//err(1, "test");//if s==-1:"a.out: test: Bad file descriptor"
			//errx(-1, "test");//if s==-1:"a.out: test";`echo $?` is 255
			//err(1, strerror(errno));//if s==-1:"a.out: Bad file descriptor: Bad file descriptor"
			//perror("test");//if s==-1:"test: Bad file descriptor"
			//warn("test");//if s==-1:"a.out: test: Bad file descriptor"<--warn() is superset of perror():1."filename:" added.2.perror cannot support c fmt string but warn can
			//warnx("test");//if s==-1:"a.out: test"
		set_common_sockopts(s, res->ai_addr);
		if (bind(s, res->ai_addr, res->ai_addrlen) == 0)//for udp, `netstat -nap` can see 'udp 0 0 0.0.0.0:1080 0.0.0.0:* 4171/./a.out' after this call return successfully, but tcp cannot!
			break;//errx(1, "bind err");
		warn("bind err");//eg port<1024
		close(s);
		s = -1;
	}
	if (!uflag && s != -1) {//if bind err, don't listen
		if (listen(s, 1) == -1)//for tcp, `netstat -nap` can see 'tcp 0 0 0.0.0.0:1080 0.0.0.0:* LISTEN 4201/./a.out' after listen return ok
			err(1, "listen err");
	}
	if (s != -1) {
		struct sockaddr addr;
		socklen_t len = sizeof(addr);
		if (getsockname(s, &addr, &len) == -1)
			err(1, "getsockname");
		report_sock(uflag? "Bound": "Listeing", &addr, len, NULL);
	}
	freeaddrinfo(res0);
	return s;
}
void my_poll(int s)
{
	struct pollfd fds[3] = {
		[0] = {
			.fd = s,
			.events = POLLIN,
		},
		[1] = {
			.fd = STDIN_FILENO,
			.events = POLLIN,
		},
		[2] = {
			.fd = -1,
		}
	};
	int r;
	while (1) {
		r = poll(fds, ARRAY_SIZE(fds), -1);
		while (r > 0) {
			if (fds[0].revents & POLLIN) {//server
				struct sockaddr sa;
				struct sockaddr_in *sap = (struct sockaddr_in*)&sa;
				socklen_t len = sizeof(sa);
				int c = accept(fds[0].fd, &sa, &len);
				if (c > 0) {
					char peer[INET_ADDRSTRLEN];
					fds[2].fd = c;
					fds[2].events = POLLIN | POLLHUP;
					printf("peer %s:%d\n", inet_ntop(AF_INET, &sap->sin_addr, peer, INET_ADDRSTRLEN),
						ntohs(sap->sin_port));
				}
				r--;
			}
			if (fds[1].revents & POLLIN){//stdin
				int ret;
				char buf[4096];
				ret = read(fds[1].fd, buf, sizeof(buf));
				if (ret > 0 && fds[2].fd >= 0)
					send(fds[2].fd, buf, ret, 0);
				r--;
			}
			if (fds[2].revents & (POLLIN | POLLHUP)) {//client
				char buf[4096];
				int ret = recv(fds[2].fd, buf, sizeof(buf), 0);
				if (ret >= 0)
					write(STDIN_FILENO, buf, ret);
				if (fds[2].revents & POLLHUP) {
					close(fds[2].fd);
					fds[2].fd = -1;
					printf("POLLHUP detected\n");
				}
				r--;
			}
		}
	}
}

void my_poll2(int fd)
{
	int r;
	struct pollfd fds[2] = {
		[0] = {
			.fd = fd,
			.events = POLLIN | POLLHUP,
		},
		[1] = {
			.fd = STDIN_FILENO,
			.events = POLLIN,
		},
	};
	while (1) {
		char buf[4096];
		int ret;
		r = poll(fds, ARRAY_SIZE(fds), -1);
		if (r > 0) {
			if (fds[0].revents & (POLLIN | POLLHUP)) {
				ret = recv(fds[0].fd, buf, sizeof(buf), 0);
				if (ret > 0)
					write(STDIN_FILENO, buf, ret);
				if (fds[0].revents & POLLHUP) {
					ret = sprintf(buf, "server POLLHUP");
					write(STDOUT_FILENO, buf, ret);
					close(fds[0].fd);
					fds[0].fd = -1;
				}
				r--;
			}
			if (fds[1].revents & POLLIN) {
				ret = read(STDIN_FILENO, buf, sizeof(buf));
				if (ret > 0 && fds[0].fd >= 0) {
					send(fds[0].fd, buf, ret, 0);
				}
				r--;
			}
		}
	}
}
void connect_server(const char *host, const char *port, struct addrinfo hints)
{
	int ret, s;
	struct addrinfo *res, *res0;
	hints.ai_flags |= AI_PASSIVE;
	if (nflag)
		hints.ai_flags |= AI_NUMERICHOST;
	if ((ret = getaddrinfo(host, port, &hints, &res)))
		errx(1, "getaddrinfo %s", gai_strerror(ret));
	for (res0 = res; res0; res0 = res0->ai_next) {
		show_addrinfo(res0);
		if ((s = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol)) < 0)
			err(1, "socket");
		if (connect(s, res0->ai_addr, res0->ai_addrlen) == -1)
			err(1, "connect");
		my_poll2(s);
	}
	freeaddrinfo(res);
}
/*
if s is tcp, poll return POLLHUP at once as connection has not been established yet;
if s is udp, socket can be blocked in poll even connect() has not been called;
connect() for udp is just specified a fixed peer ip+port for later rx+tx, no connection
actually as udp is connectionless.
Besides, whether s is O_NONBLOCK will not affect poll()'s blocking and poll() returned .revents.
If there is POLLHUP in .revents, it means the tcp socket connection has not been established.
If the socket has been set O_NONBLOCK or not has no relation to POLLHUP!
*/
void testpoll(int s)
{
	while (1) {
		;
		int ret;
#if 0
		char buf[1];
		int ret = read(s, buf, 1);
#else
		struct pollfd pfd[1] = {
			[0] = {
				.fd = s,
				.events = POLLIN,
			}
		};
		ret = poll(pfd, ARRAY_SIZE(pfd), -1);
		printf("%d: %x\n", ret, pfd[0].revents);
		if (pfd[0].revents & POLLHUP)
			exit(1);
#endif
	}
}

static int connect_with_timeout(int fd, const struct sockaddr *sa, socklen_t salen, int ctimeout)
{
	struct timeval tv, *tvp = NULL;
	int ret, org_flags;
	if (ctimeout > 0) {
		tv.tv_sec = ctimeout / 1000;
		tv.tv_usec = ctimeout % 1000 * 1000;
	}
	if ((org_flags = fcntl(fd, F_GETFL)) == -1)
		err(1, "F_GETFL error");
	if ((ret = fcntl(fd, F_SETFL, org_flags | O_NONBLOCK)) == -1)
		err(1, "F_SETFL error");
	if ((ret = connect(fd, sa, salen)) == 0)
		goto OUT;
	if (ret == -1 && errno == EINPROGRESS) {
		fd_set set;
		FD_ZERO(&set);
		FD_SET(fd, &set);
		do {
			ret = select(fd + 1, NULL, &set, NULL, tvp);
		} while(ret == -1 && errno == EINTR);//then @set is not touched so select can be restart again w/o reset @set
		if (ret == 0)
			return CONNECTION_TIMEOUT;
		if (ret == 1 && FD_ISSET(fd, &set))
			ret = CONNECTION_SUCCESS;
	}
OUT:
	fcntl(fd, F_SETFL, org_flags);
	return ret;
}
/*comment A(the following a.out test is done with udp and testpoll() enabled, ie -u and no connect() called):
1.standard nc:no matter if '-u' is added to 'nc -lu ip port', netstat will see udp ip:port binding
as long as it runs. The reason is bind() call exists when the cmd runs.
to make it happen in client here:
./a.out -u -s 10.0.0.1 10.0.0.1 1080, then netstat can see sth like(port is not specified so getaddrinfo()
PO port 0, when port 0 is used in bind(), a random port is used)
udp        0      0 10.0.0.1:51439          0.0.0.0:*                           14686/./a.out
other test:
./a.out -u -p 1024 10.0.0.1 1080, netstat
udp        0      0 0.0.0.0:1024            0.0.0.0:*<-- so unspecified -s should not be used?

./a.out -u -s 10.0.0.1 -p 0 10.0.0.1 1080, netstat:
udp        0      0 10.0.0.1:44478          0.0.0.0:*                           14778/./a.out
-p 0 actually specify use random port.

./a.out -u 10.0.0.1 1080, netstat show no entry.
conclusion:after client called bind() and it is ok, netstat will show binded ip and port at once.
It is the bind() that determin if there is entry in netstat output. If no bind(), there is no related entry in netstat output
*/
int//here hints.ai_family is AF_UNSPEC, but getaddrinfo() can return AF_INET in .ai_family
remote_connect(const char *host, const char *port, struct addrinfo hints)
{
	int s = -1, error, save_errno;
	struct addrinfo *res, *res0;
//getaddrinfo("127.0.0.1", "1080",) and hints.ai_flags w/t AI_NUMERICHOST set:res->ai_addr will hold
//BE of 127.0.0.1 & 1080
	if ((error = getaddrinfo(host, port, &hints, &res0)))
		err(1, "getaddrinfo for host \"%s\" port %s: %s", host, port, gai_strerror(error));
	for (res = res0; res; res = res->ai_next) {
/*Starting with kernel 2.6.27, Linux provides a second use for the socket()'s 2nd @type argument,
by allowing two nonstandard flags to be ORed with the socket type....The SOCK_NONBLOCK flag causes the kernel
to set the O_NONBLOCK flag on the underlying open file description, so that future
I/O operations on the socket will be nonblocking. This saves additional calls
to fcntl() to achieve the same result. fcntl example is in eintr.c*/
		show_addrinfo(res);
		if ((s = socket(res->ai_family, res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol)) == -1)
			err(1, "socket error");
/*connect() block until SYN/ACK got, then connect() enqueues the final ACK Message and returns.
if SYN/ACK is never rxed, then connect() call eventually times out(controlled by os, eg 20s)*/
		if (sflag || pflag) {
			struct addrinfo ahints[1], *ares;
			memset(ahints, 0, sizeof(ahints));
			ahints->ai_family = res->ai_family;
			ahints->ai_flags = AI_PASSIVE;//?
			if (uflag) {
				ahints->ai_socktype = SOCK_DGRAM;
				ahints->ai_protocol = IPPROTO_UDP;
			} else {
				ahints->ai_socktype = SOCK_STREAM;
				ahints->ai_protocol = IPPROTO_TCP;
			}
			if ((error = getaddrinfo(sflag, pflag, ahints, &ares)))
				err(1, "getaddrinfo %s %s %s", sflag, pflag, gai_strerror(error));
			printf("%s: %s:%s\n", __func__, sflag, pflag);
			show_addrinfo(ares);
			if (bind(s, ares->ai_addr, ares->ai_addrlen) == -1)//comment A
				err(1, "bind err");
			freeaddrinfo(ares);
			//testpoll(s);
		}
		set_common_sockopts(s, res->ai_addr);
		if ((error = connect_with_timeout(s, res->ai_addr, res->ai_addrlen, timeout)) == CONNECTION_SUCCESS)
			break;
		warn("connect to %s:%p (%s) err %d", host, port, uflag? "udp": "tcp", error);
		save_errno = errno;
		close(s);
		errno = save_errno;
		s = -1;
	}
	freeaddrinfo(res0);
	return s;
}

void quit()
{
	exit(0);
}

ssize_t fillbuf(int fd, char *buf, size_t *bufpos)
{
	ssize_t n;
	n = read(fd, buf + *bufpos, BUFSIZE - *bufpos);
	if (n == -1 && (errno == EAGAIN || errno == EINTR))
		n = -2;
	if (n <= 0)
		return n;
	*bufpos += n;
	return n;
}

ssize_t drainbuf(int fd, char *buf, size_t *bufpos, int oneline)
{
	ssize_t n;
	n = write(fd, buf, *bufpos);
	if (n == -1 && (errno == EAGAIN || errno == EINTR))
		n = -2;
	if (n <= 0)
		return n;
	if (*bufpos - n > 0)
		memmove(buf, buf + n, *bufpos - n);//memmove can handle overlap
	*bufpos -= n;
	return n;
}

void readwrite(int net_fd)
{
	char netinbuf[BUFSIZE], stdinbuf[BUFSIZE];
	size_t netinbufpos, stdinbufpos;//size_t:unsigned long, ssize_t:long
	netinbufpos = stdinbufpos = 0;
	int n, num_fds;
	ssize_t ret;//ssize_t can be seen as 'long' on 32/64bit arch
	struct pollfd pfd[4] = {
		[POLL_STDIN] = {
			.fd = STDIN_FILENO,
			.events = POLLIN,
		},
		[POLL_NETOUT] = {//[1] & [2] can be combined together by combining their .events
			.fd = net_fd,//don't set POLLOUT as it will be returned continuously in .revents until closed locally
			.events = 0,
		},
		[POLL_NETIN] = {
			.fd = net_fd,
			.events = POLLIN,
		},
		[POLL_STDOUT] = {
			.fd = STDOUT_FILENO,
			.events = 0,
		},
	};
	while (1) {
		if (pfd[POLL_STDIN].fd == -1 && pfd[POLL_NETIN].fd == -1 &&
			stdinbufpos == 0 && netinbufpos == 0) {
			if (qflag <= 0)
				return;
			goto delay_exit;
		}
		if (pfd[POLL_NETOUT].fd == -1 && pfd[POLL_STDOUT].fd == -1) {
			if (qflag <= 0)
				return;
			goto delay_exit;
		}
		if (lflag && pfd[POLL_NETIN].fd == -1 && stdinbufpos == 0 && netinbufpos == 0) {
			if (qflag <= 0) {
				printf("**here\n");
				return;
			}
delay_exit:
			close(net_fd);
			signal(SIGALRM, quit);
			alarm(qflag);//note arg typeof is unsigned in
		}
		num_fds = poll(pfd, 4, timeout);
		//printf("%s:%d %d %d\n", __func__, __LINE__, net_fd, num_fds);
		if (num_fds == -1)
			err(1, "polling error");
		if (num_fds == 0)
			return;
/*
1.POLLNVAL:the file has not been open() eg close(); here in theory either POLL_NETIN or POLL_NETOUT's .fd==-1, but not both.
So if poll() a close()-ed fd, POLLNVAL returned in .revents;
2.POLLERR can happens after rx [RST], then send() before actually is rejected by peer. see README 2 and 2.f
*/
		for (n = 0; n < ARRAY_SIZE(pfd); n++) {
			printf("%x:%d %x\n", n, pfd[n].fd, pfd[n].revents);
			if (pfd[n].revents & (POLLERR | POLLNVAL))//a common way to make poll() ignore the .fd(the next poll() returned .revent==0).If .revents bit not handled, eg no read() after POLLIN set,then the next poll() return at once with unhandled bits set
				pfd[n].fd = -1;
		}
		if (pfd[POLL_STDIN].events & POLLIN &&
			pfd[POLL_STDIN].revents & POLLHUP &&
			!(pfd[POLL_STDIN].revents & POLLIN))
			pfd[POLL_STDIN].fd = -1;
		if (pfd[POLL_NETIN].events & POLLIN &&
			pfd[POLL_NETIN].revents & POLLHUP &&
			!(pfd[POLL_NETIN].revents & POLLIN))
			pfd[POLL_NETIN].fd = -1;
/*POLLHUP means both r&w of a socket has been shutdown(r can be shutdown localy or by peer's SHUT_WR):
read() return 0 and write() will:
1. return error if there is SIGPIPE handler
2. killed by SIGPIPE if no SIGPIPE handler
*/
		if (pfd[POLL_NETOUT].revents & POLLHUP)
			pfd[POLL_NETOUT].fd = -1;
		if (pfd[POLL_STDOUT].revents & POLLHUP)
			pfd[POLL_STDOUT].fd = -1;
		if (pfd[POLL_NETOUT].fd == -1)
			pfd[POLL_STDIN].fd = -1;
		if (pfd[POLL_STDOUT].fd == -1) {
			if (pfd[POLL_NETIN].fd != -1)
				shutdown(pfd[POLL_NETIN].fd, SHUT_RD);
			pfd[POLL_NETIN].fd = -1;//why not full close since it is not used any more?
		}
		if (pfd[POLL_STDIN].revents & POLLIN && stdinbufpos < BUFSIZE) {
			ret = fillbuf(pfd[POLL_STDIN].fd, stdinbuf, &stdinbufpos);
			if (ret == 0 || ret == -1)
				pfd[POLL_STDIN].fd = -1;
			if (stdinbufpos == BUFSIZE)
				pfd[POLL_STDIN].events = 0;
			if (stdinbufpos > 0)
				pfd[POLL_NETOUT].events = POLLOUT;
		}
		if (pfd[POLL_NETOUT].revents & POLLOUT && stdinbufpos > 0) {
			ret = drainbuf(pfd[POLL_NETOUT].fd, stdinbuf, &stdinbufpos, 0);
			if (ret == 0 || ret == -1)
				pfd[POLL_NETOUT].fd = -1;
			if (stdinbufpos == 0)
				pfd[POLL_NETOUT].events = 0;
			if (stdinbufpos < BUFSIZE)
				pfd[POLL_STDIN].events = POLLIN;
		}
		if (pfd[POLL_NETIN].revents & POLLIN && netinbufpos < BUFSIZE) {
			ret = fillbuf(pfd[POLL_NETIN].fd, netinbuf, &netinbufpos);
			if (ret == -1)
				pfd[POLL_NETIN].fd = -1;
			if (ret == 0) {
				shutdown(pfd[POLL_NETIN].fd, SHUT_RD);
				pfd[POLL_NETIN].fd = -1;
			}
			if (netinbufpos == BUFSIZE)
				pfd[POLL_NETIN].events = 0;
			if (netinbuf > 0)
				pfd[POLL_STDOUT].events = POLLOUT;
		}
		if (pfd[POLL_STDOUT].revents & POLLOUT && netinbufpos > 0) {
			ret = drainbuf(pfd[POLL_STDOUT].fd, netinbuf, &netinbufpos, 0);
			if (ret == 0 || ret == -1)
				pfd[POLL_STDOUT].fd = -1;
			if (netinbufpos == 0)
				pfd[POLL_STDOUT].events = 0;
			if (netinbufpos < BUFSIZE)
				pfd[POLL_NETIN].events = POLLIN;
		}
		if (pfd[POLL_STDIN].fd == -1 && stdinbufpos == 0) {
			pfd[POLL_NETOUT].fd = -1;
		}
		if (pfd[POLL_NETIN].fd == -1 && netinbufpos == 0)
			pfd[POLL_STDOUT].fd = -1;
	}
}
//** rather than *:either use it as array to iterator(* cann't iterate) or derefer it as left-value
void build_ports(char **p)
{
	struct servent *sv;
	short port;
	const char *errmsg;//if named 'err', then err(...) error: called object ‘err’ is not a function or function pointer
/*getservbyname(): obsolete now.check /etc/service to get port# from server name. If a matching
record is found in /etc/services, a statically allocated (servent) is returned, NULL otherwise;
*/
	sv = getservbyname(*p, uflag? "udp": "tcp");
	if (sv) {
		if (asprintf(&portlist[0], "%d", ntohs(sv->s_port) < 0))//return -1 when mem allocation fails.
			err(1, "asprintf");
	} else {
/*
0.'-lbsd' must be used for link
1.no need to init @err, err==0 on ok and !=0 on error
2.on err (not in [1,65535]) 0 is returned in @port but @port is not checked against 0
  since *p can be "0"
3.manual doesn't mention the free of @errmsg string
*/
		port = strtonum(*p, 1, 65535, &errmsg);
		if (errmsg != NULL)
			err(1, "strtonum err:%s", errmsg);
		if (asprintf(portlist, "%d", port) < 0)
			err(1, "asprintf");
	}
}

int main(int argc, char *argv[])
{
	int ch, s = -1;
	char *host = NULL, **uport = NULL;
	union {
		struct sockaddr_storage storage;//The struct sockaddr_storage type is guaranteed to be large enough to hold the largest supported address on your system.This keeps our code robust in the case that we change it from IPv4 to IPv6
		struct sockaddr_un forunix;
	} cliaddr;
	socklen_t len;
	struct addrinfo hints;
/*A more serious issue to consider is what happens when send() is called on a socket that
has already received an RST message from its peer. On Unix-based systems, the default is
to send a SIGPIPE signal to your program. If you don't handle this signal, the operating
system will terminate your program.
It is therefore essential for TCP servers to either handle or disable the SIGPIPE signal.
Failure to handle this case means that a rude client could kill your server.
Signals are complicated. If you're already using signals in your program, you may want to
handle SIGPIPE. Otherwise, I recommend you just disable it by setting the SIGPIPE
handler to SIG_IGN.*/
	signal(SIGPIPE, SIG_IGN);
	while ((ch = getopt(argc, argv, "bklnup:s:")) != -1) {
		switch (ch) {
		case 'b':
			bflag = 1;
			break;
		case 'k':
			kflag = 1;
			break;
		case 'l':
			lflag = 1;
			break;
		case 'n':
			nflag = 1;
			break;
		case 'u':
			uflag = 1;
			break;
		case 'p':
			pflag = optarg;
			break;
		case 's':
			sflag = optarg;
			break;
		default:
			usage(1);
		}
	}
	argc -= optind;
	argv += optind;
	if (argc == 0 && lflag) {
		uport = &pflag;
		host = sflag;
	} else if (argc == 1 && !sflag && !pflag && lflag) {
		uport = argv;
	} else if (argc == 2) {
		if (lflag && (pflag || sflag))
			usage(1);//conflict
		host = argv[0];
		uport = argv + 1;
	} else
		usage(1);

	if (!uport || !*uport) {//!,* are of the same precedent and the associativity is right-to-left, so no need for !(*port)
		errx(1, "missing port number");//no '\n' here since errx will add it
	}
	if (!lflag && kflag)
//		errx(1, "-l must be used with -k");
		errx(1, "must use -l with -k");
/*
preparing for getaddrinfo() since nc can be called with a host name like:
~/src/mini2440/qemu/mini2440$ git pull<--run git pull in mini2440 qemu to ask nc to do socks5 proxy
+ nc -X 5 -x 192.168.1.254:1080 repo.or.cz 9418<--~/.nc specified by core.gitproxy=/home/rednoah/.nc is run
*/
	memset(&hints, 0, sizeof(struct addrinfo));//the fields except ai_{family,flags,protocol,socktype} must be cleared!
	hints.ai_family = family;
	if (uflag) {
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
	} else {
		hints.ai_socktype = SOCK_STREAM;//`./a.out -l 1080` emits "a.out: getaddrinfo: ai_socktype not supported"
		hints.ai_protocol = IPPROTO_TCP;
	}
	if (nflag)
		hints.ai_flags = AI_NUMERICHOST;//getaddrinfo's 1st arg host is forcely interpreted as a numeric address string even it is like 'baidu.com'
	if (lflag) {
		s = local_listen(host, *uport, hints);
		if (s < 0)
			errx(1, "setup server error");
		for (;;) {
			if (uflag && kflag) {
			} else if (uflag && !kflag) {
			} else {
				int connfd;
				len = sizeof(cliaddr);
/*no matter what the last argument of accept4() is, accept4() will block till client connects.
a.if set SOCK_NONBLOCK, the @connfd will be NONBLOCK so the read() return -1 directly & errno==EAGAIN
as long as client has no data sent:
a.out: readwrite -1, errno 11: Resource temporarily unavailable
then `netstat -nap|grep -n 1080`:
20:tcp        0      0 127.0.0.1:1080          127.0.0.1:60376         FIN_WAIT2   -
23:tcp        0      0 127.0.0.1:60376         127.0.0.1:1080          CLOSE_WAIT  4756/./a.out
1.server did active close(by a.out exit) but the client has not launch close(); so server part stays at "FIN_WAIT2",
which will last for a while before disappearing.
2.client did passive close. If it dosen't exit(eg by ^+c), the following lasts all the time even server
part's FIN_WAIT2 has gone:
23:tcp        0      0 127.0.0.1:60376         127.0.0.1:1080          CLOSE_WAIT  4756/./a.out
kill client via ^+c, then the above line disappears at once.
b. if unset SOCK_NONBLOCK, the @connfd's read() will block until client sends data. Then server's
read() get the data and exit. netstat will be:
11457764:tcp        0      0 127.0.0.1:1080          127.0.0.1:60390         FIN_WAIT2   -
11457772:tcp        0      0 127.0.0.1:60390         127.0.0.1:1080          CLOSE_WAIT  4817/./a.out
these 2 line's state has been explained in a.
kill client via ^+c, then netstat becomes:
11524865:tcp        0      0 127.0.0.1:1080          127.0.0.1:60392         TIME_WAIT
The change of server part's FIN_WAIT2 to TIME_WAIT is caused by server getting FIN from client.
FIN of client is caused by its close() calling. Note the TIME_WAIT lasts for a while before disappearing.
*/
				connfd = accept4(s, (struct sockaddr*)&cliaddr, &len, 0);//If last arg is 0, then accept4() is the same as accept().
				if (connfd == -1)
					err(1, "accept4");
				report_sock("SYN from", (struct sockaddr*)&cliaddr, len, family == AF_UNIX ? host : NULL);
				readwrite(connfd);
				close(connfd);
			}
			if (!kflag) {
				if (s != -1)
					close(s);
				break;
			}
		}
	} else if (family == AF_UNIX) {
	} else {
		build_ports(uport);
		s = remote_connect(host, portlist[0], hints);
		if (s == -1)
			err(1, "connect error");
		readwrite(s);
	}
	if (s != -1)
		close(s);
	return 0;
}

