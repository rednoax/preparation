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

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
int bflag, kflag, lflag, uflag;
int nflag;//Do not do any DNS or service lookups on any specified addresses, hostnames or ports.

int family = AF_UNSPEC;

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

void readwrite(int net_fd)
{
	char buf[4096];
	int ret = read(net_fd, buf, sizeof(buf));
/*https://stackoverflow.com/questions/5616092/non-blocking-call-for-reading-descriptor
If data is not available when you call read, then the system call will fail with a return
value of -1 and errno is set to EAGAIN. See the fnctl man pages for more information.*/
	err(1, "%s %d, errno %d", __func__, ret, errno);
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
	while ((ch = getopt(argc, argv, "bklnu")) != -1) {
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
		default:
			usage(1);
		}
	}
	argc -= optind;
	argv += optind;
	if (argc == 1 && lflag) {
		uport = argv;
	} else if (argc == 2) {
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
		connect_server(host, *uport, hints);
	}
	if (s != -1)
		close(s);
	return 0;
}

