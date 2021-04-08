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
#include <signal.h>

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
#if 10/*
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
tcp        0      0 127.0.0.1:1080          127.0.0.1:58472         TIME_WAIT   -<---not FIN_WAIT2, for nc server did close() in readwrite() after getting client's FIN()
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

void debug_poll(int ret, struct pollfd fds[], int size)
{
	int i, r = 0;
	char buf[1024];
	static int _revents[16], _r, _cnt = -1;
	fflush(stdout);
	if (_r != ret) {
		r = sprintf(buf, "poll %d=>%d;", _r, ret);
		_r = ret;
	}
	for (i = 0; i < size; i++) {
		if (_revents[i] != fds[i].revents) {
			r += sprintf(buf + r, "[%d] %x=>%x;", i, _revents[i], fds[i].revents);
			_revents[i] = fds[i].revents;
		}
	}
	if (r > 0) {
		r += sprintf(buf + r, "\t%d -> 1\n", _cnt + 1);
		write(STDOUT_FILENO, buf, r);
		_cnt = 0;
	} else
		_cnt++;
	if (_cnt >= 20000000) {
		r += sprintf(buf + r, "/|\\ %d hits\n", _cnt);
		write(STDOUT_FILENO, buf, r);
		_cnt = -1;
		_r = 0;
		for (i = 0; i < size; i++)
			_revents[i] = 0;//.revent==0 in struct pollfd means no event, ALL POLLXX defined in <poll.h> is >0
	}
}

void pipehandler(int s)
{
	printf("sig %d\n", s);
	fflush(stdout);
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
		debug_poll(r, fds, ARRAY_SIZE(fds));
		while (r > 0) {
			if (fds[0].revents & POLLIN) {//server
				struct sockaddr sa;
				struct sockaddr_in *sap = (struct sockaddr_in*)&sa;
				socklen_t len = sizeof(sa);
				int c = accept(fds[0].fd, &sa, &len);
				if (c > 0) {
					char peer[INET_ADDRSTRLEN];
					fds[2].fd = c;
					fds[2].events = POLLIN | POLLOUT;//no need to set POLLHUP, as long as local fds[2].fd is not closed, POLLOUT will continously returned in .revents even peer(client) `sdw` or `close` to sent a FIN
					printf("peer %s:%d\n", inet_ntop(AF_INET, &sap->sin_addr, peer, INET_ADDRSTRLEN),
						ntohs(sap->sin_port));
					fflush(stdout);
				}
				r--;
			}
			if (fds[1].revents & POLLIN){//stdin
				int ret, flags = 0, c = fds[2].fd;
				char buf[4096];
				ret = read(fds[1].fd, buf, sizeof(buf));
/*
1.c:`sdw`
  s:send() any input from stdin is ok and client can get it normally
2.c:`close`
  s:send() return ok: eg send() 'bye\n' return 4, wireshark can see the packet from s to c, but client return
  immediately one 'RST' packet to server. So poll() return at once and its client socket .revent becomes
  0x1d(POLLHUP|POLLERR|POLLOUT|POLLIN) from old status 0x5(POLLIN|POLLOUT).	NO SIGPIPE in the process.
  BUT, if going on to call send() with one of the following, There is no any packet between c and s can be seen by wireshark.
1)"NOSIG\n": send() return -1, errno is EPIPE(32) and then next poll() return at once 0x15(POLLIN|POLLOUT|POLLHUP) in
  client socket .revent. The error info is:  'a.out: ***send -1, errno 32: Broken pipe'
2)"R_SIG\n": the same as 1. Besides, registered SIGPIPE handler is called before send() return -1
3)"IGN": the same as 1.
4)other ordinary input not among 1~3:program exits directly as it is killed by SIGPIPE.
*/
				if (ret > 0 && c >= 0) {
					int w;
					if (!strncmp(buf, "NOSIG", 5)) {
						flags = MSG_NOSIGNAL;
					} else if (!strncmp(buf, "R_SIG", 5)) {
						struct sigaction sa = {
							.sa_flags = 0,
							.sa_handler = pipehandler,
						};
						if (sigaction(SIGPIPE, &sa, NULL) == -1)
							err(1, "SIGPIPE reg err");
					} else if (!strncmp(buf, "IGN", 3)) {
						struct sigaction sa = {
							.sa_flags = 0,
							.sa_handler = SIG_IGN,
						};
						if (sigaction(SIGPIPE, &sa, NULL) == -1)
							err(1, "SIGPIPE reg err");
					} else if (!strncmp(buf, "close_client", 12)) {//if run it after the above 2-1/2/3, then input anything to lauch a poll() return:fds[2].revents will be 0 from old 0x15
						close(c);
						fds[2].fd = -1;
						goto fin;
					} else if (!strncmp(buf, "sdw", 3)) {
						if (shutdown(c, SHUT_WR) == -1)
							err(1, "SHUT_WR");
						goto fin;
					}
					w = send(c, buf, ret, flags);
					if (w < 0)
						warn("***send %d, errno %d", w, errno);
					else
						warnx("send %d", w);
				}
fin:			r--;
			}
//The field fd contains a file descriptor for an open file. If this field is negative, then the corresponding events field is ignored and the revents field returns zero.
			if (fds[2].revents & (POLLIN | POLLHUP)) {//client, if .fd==-1, revents return 0 so skip .fd check befor .revents check
				char buf[4096];
				int ret = recv(fds[2].fd, buf, sizeof(buf), 0);
/*recv() return:When a stream socket peer has performed an orderly shutdown, the return value will be 0
(the traditional "end-of-file" return).
TODO: when client ^+c:here no POLLHUP but POLLIN then recv return 0
why no POLLHUP? stackoverflow's 'poll-cant-detect-event-when-socket-is-closed-locally' answer doesn't make sense;
TCP/IP illustrated vol1 2nd:
In the FIN_WAIT_2 state, TCP has sent a FIN and the other end has acknowledged<-both close() shutdown(,SH_WR) can send a FIN
it. Unless a half-close is being performed, the TCP must wait for the application
on the other end to recognize that it has received an end-of-file notification and<-so passive close peer get EOF.The tcp connection is just half closed now.so no POLLHUP
close its end of the connection, which causes a FIN to be sent.<-!
*/
				if (ret >= 0)
					write(STDIN_FILENO, buf, ret);
				if (fds[2].revents & POLLIN && !ret) {//& precedes &&
/*when the following close line opened:
if c runs `sdw` or `close`(these 2 have the same effect to send [FIN ACK] except the former can still read
the file descriptor but the latter cannot as fd has been invalid), a simultaneous close is observed by wireshark:
c=>s [FIN ACK] Seq=1 Ack=1
s=>c [FIN ACK] Seq=1 Ack=2<--Ack is last Seq's
c=>s [ACK] Seq=2 Ack=2<--Ack is last Seq's
TODO: this introduced a bug: if peer runs `sdw` to half-close, server finds it here but does a full close.
The half close expected by client becomes a full close for the connection.
*/			
					//close(fds[2].fd);fds[2].fd = -1;//ALA this close line is not done, socket connected with client trigger POLLIN|POLLOUT continuously after peer(client) use `sdw` or `close` to send a FIN
					//printf("EOF\n");
					fflush(stdout);//absolutely necessary, no output otherwise
				}
				r--;
			}
			if (fds[2].revents & POLLOUT)
				r--;
		}
	}
}

void my_poll2(int fd)
{
	int r;
	struct pollfd fds[2] = {
		[0] = {
			.fd = fd,
			.events = POLLIN,// | POLLHUP is ignored in events so remove it!
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
			if (fds[0].fd != -1 && fds[0].revents & (POLLIN | POLLHUP)) {
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
				int s = fds[0].fd;
				ret = read(STDIN_FILENO, buf, sizeof(buf));
				//printf("read %d\n", ret);
				if (ret > 0 && s >= 0) {
					int r;
					if (!strncmp(buf, "sdr", 3)) {//strncmp's n should not be ret as ret contains '\n'
//c=>s no any packet sent from c to s after the read's shutdown
						if ((r = shutdown(s, SHUT_RD)) == -1)
							warn("sdr error");
						else
							printf("sdr %d\n", r);
					} else if (!strncmp(buf, "sdw", 3)) {
/*c=>s [FIN,ACK];
s=>c [ACK]; if server ^c then, POLLHUP is got*/
						if ((r = shutdown(s, SHUT_WR)) == -1)
							warn("sdw error");
						else
							printf("sdw %d\n", r);
					} else if (!strncmp(buf, "close", 5)) {
/*c=>s [FIN,ACK];
s=>c [ACK]; if server ^c then, No ANY POLLHUP can be got by client*/
						r = close(s);
						printf("close %d\n", r);
						fds[0].fd = -1;
					} else
						send(s, buf, ret, 0);
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
int main(int argc, char *argv[])
{
	int ch, s = -1;
	char *host = NULL, **uport = NULL;
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
		my_poll(s);		
	} else {
		connect_server(host, *uport, hints);
	}
	return 0;
}
