//$ gcc -Wall rewrite-nc.c -lbsd
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
#include <fcntl.h>

#if 10
#define debug(fmt, arg...) printf(fmt, ##arg)
#else
#define debug(fmt, arg...) do{}while(0)
#endif

#define BUFSIZE 16384
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
int bflag, kflag, lflag, uflag, qflag = -1;
int nflag;//Do not do any DNS or service lookups on any specified addresses, hostnames or ports.
char *pflag, *sflag;
int timeout = -1;
int family = AF_UNSPEC;
char *portlist[1], g_fd = -1;
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
	debug("flags:");
	if (res->ai_flags == 0) {
		debug(" 0");
	} else {
		if (res->ai_flags & AI_PASSIVE)
			debug(" passive");
		if (res->ai_flags & AI_CANONNAME)
			debug(" canon");
		if (res->ai_flags & AI_NUMERICHOST)
			debug(" numhost");
		if (res->ai_flags & AI_NUMERICSERV)
			debug(" numserv");
		if (res->ai_flags & AI_V4MAPPED)
			debug(" v4mapped");
		if (res->ai_flags & AI_ALL)
			debug(" all");
	}
	debug("\nfamily:");
	switch (res->ai_family) {
	case AF_INET:
		debug(" inet");
		break;
	case AF_INET6:
		debug(" inet6");
		break;
	case AF_UNIX:
		debug(" unix");
		break;
	case AF_UNSPEC:
		debug(" unspecified");
		break;
	default:
		debug(" unknown");
	}
	debug("\ntype:");
	switch (res->ai_socktype) {
	case SOCK_STREAM:
		debug(" stream");
		break;
	case SOCK_DGRAM:
		debug(" datagram");
		break;
	case SOCK_SEQPACKET:
		debug(" seqpacket");
		break;
	case SOCK_RAW:
		debug(" raw");
		break;
	default:
		debug(" unknow (%d)", res->ai_socktype);
	}
	debug("\nprotocol:");
	switch (res->ai_protocol) {
	case 0:
		debug(" default");
		break;
	case IPPROTO_TCP:
		debug(" tcp");
		break;
	case IPPROTO_UDP:
		debug(" udp");
		break;
	case IPPROTO_RAW:
		debug(" raw");
		break;
	default:
		debug(" unknown (%d)", res->ai_protocol);
		break;
	}
 	debug("\ncanon name: %s\t", res->ai_canonname);
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
		debug("%s:%d\t assert %d==%d\n", buf, port, fm, res->ai_family);
	else if (s == NULL)//when error, s == 0
		err(1, "inet_ntop error");
}

void set_common_sockopts(int s, struct sockaddr *sa)
{
	int x = 1;
	if (bflag) {
/*man 7 socket:
Set or get the broadcast flag.  When enabled, datagram sockets are allowed to send packets to
a broadcast address.  This option has no effect on stream-oriented sockets.*/
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
/*
s:./a.out -lku 127.0.0.1 1080,netstat -nap shows lo:1080 listen rather than eth0:1080 listen
c:./a.out -u 192.168.1.147 1080 will fail, as it uses eth0 ip rather than lo ip;
*/
int local_listen(const char *host, const char *port, struct addrinfo hints)
{
	int s = -1, ret = 0, x = 1;
	int error;
	struct addrinfo *res, *res0;
	hints.ai_flags |= AI_PASSIVE;//allow @host name to be NULL(if so then ip member of POed res0->ai_addr below will be INADDR_ANY or IN6ADDR_ANY_INIT, according to the domain), if @host!=NULL,AI_PASSIVE has no effect
	(void)x;
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
$ ./a.out -l 127.0.0.1 1080<--getaddrinfo() can PO 127.0.0.1:1080 in (addrinfo*)->ai_addr
*/
	debug("%s: %s\n", host, port);
	for (res = res0; res; res = res->ai_next) {
		show_addrinfo(res);
		//if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol) < 0))//compareing like '<' has precedentce over '=', bind(0,) cause 'Socket operation on non-socket'
		if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
			continue;
		//s = -1;//to test err after setsockopt fails
#if 10/*
1.no matter if there is close()(sever exist by ^c then os will close() its socket) at commentB:launch server=>client connect=>server quit by ^c:
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
2.if use standard nc to run the same sequence as the above, plus client ^c at last:
$ netstat -nap|grep 1080
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
tcp        0      0 127.0.0.1:1080          127.0.0.1:58472         TIME_WAIT   -<---not FIN_WAIT2, as client send [FIN,ACK] at last step
rednoah@lucia:~$ nc -l 1080<--ok. if run ./a.out -l 1080:bind err: Address already in use.
SO_REUSEADDR is used for bypass bind() err when TIME_WAIT.Note TIME_WAIT only exists at
active close() part and passive close() part will have no TIME_WAIT state.
*/
		ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(x));//then no bind() err if server rerun when server side's TIME_WAIT still exists. TIME_WAIT only exists in active close part, not passive close part.
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
			//warn("test");//if s==-1:"a.out: test: Bad file descriptor"<--warn() is superset of perror():1."filename:" added.2.perror cannot support c fmt string and variable args but warn can
			//warnx("test");//if s==-1:"a.out: test"
		set_common_sockopts(s, res->ai_addr);
		if (bind(s, res->ai_addr, res->ai_addrlen) == 0)//for udp, `netstat -nap` can see 'udp 0 0 0.0.0.0:1080 0.0.0.0:* 4171/./a.out' after this call return successfully, but tcp cannot!
			break;//errx(1, "bind err");
		warn("bind err");//eg port<1024
		close(s);
		s = -1;
	}
	if (!uflag && s != -1) {//if bind err, don't listen
//listen() is non-blocking. accept() blocks and returns a client<>server socket upon connection.UDP needs not and must not use listen/accept
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
					debug("peer %s:%d\n", inet_ntop(AF_INET, &sap->sin_addr, peer, INET_ADDRSTRLEN),
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
					debug("POLLHUP detected\n");
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
		debug("%d: %x\n", ret, pfd[0].revents);
		if (pfd[0].revents & POLLHUP)
			exit(1);
#endif
	}
}

static int connect_with_timeout(int fd, const struct sockaddr *sa, socklen_t salen, int ctimeout)
{
	struct timeval tv, *tvp = NULL;
	int ret, org_flags;
	socklen_t len;
	if (ctimeout > 0) {
		tv.tv_sec = ctimeout / 1000;
		tv.tv_usec = ctimeout % 1000 * 1000;
		tvp = &tv;
	}
	if ((org_flags = fcntl(fd, F_GETFL)) == -1)//F_GETFD can only be used for FD_CLOEXEC;
		err(1, "F_GETFL error");
	if ((ret = fcntl(fd, F_SETFL, org_flags | O_NONBLOCK)) == -1) {
		warn("F_SETFL error");
		if (connect(fd, sa, salen) == 0)
			return CONNECTION_SUCCESS;
		else
			return CONNECTION_FAILED;
	}
/*
tcp:When the !O_NONBLOCK connect() function is called on the Client side, the first SYN Message is sent,
and the connect() function blocks until the SYN+ACK Message is received from the
Server. After the SYN+ACK Message is received, connect() enqueues the final ACK
Message and returns;
udp:If we call connect() with a SOCK_DGRAM socket, the destination address of all messages we send(in this program
it is readwrite()'s write() call) is set to the address we specified in the connect() call, relieving
us from having to provide the address everytime we transmit message. In addition, we will receive datagrams
only from the address we've specified(in this program it is readwrite()'s read()*/
	ret = connect(fd, sa, salen);
	if (ret == -1 && errno == EINPROGRESS) {
		fd_set set;
		FD_ZERO(&set);
		FD_SET(fd, &set);
		do {
			ret = select(fd + 1, NULL, &set, NULL, tvp);
		} while(ret == -1 && errno == EINTR);//then @set is not touched so select can be restart again w/o reset @set
		if (ret < 0)
			err(1, "select error");
		if (ret == 0)
			return CONNECTION_TIMEOUT;
		if (ret == 1 && FD_ISSET(fd, &set)) {//cannot be AL connect() ok!see below
			debug("connect() ok?\n");
		}
		len = sizeof(ret);
/*On Unixbased systems, select() signals a socket as writable once the connection is established. If
an error has occurred, select() signals the socket as both writable and readable.
However, if the socket has connected successfully and data has arrived from the remote
peer, this also produces both the readable and writable situation. In that case, the
getsockopt() function can be used to determine whether an error has occurred.*/
		if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &ret, &len) == -1)//SO_ERROR:return and clear pending socket error
			err(1, "getsockopt error");
		if (ret != 0) {
			printf("**%d:", ret);
			errno = ret;
		}
	}
	fcntl(fd, F_SETFL, org_flags);
	return ret != 0 ? CONNECTION_FAILED: CONNECTION_SUCCESS;//!= is higher than ?:
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
to fcntl() to achieve the same result. (fcntl example is in eintr.c)
So now we have seen 3 ways to set O_NONBLOCK:
1.specify SOCK_NONBLOCK when socket()
2.fcntl O_NONBLOCK, see connect_with_timeout
3.accept4() specifies SOCK_NONBLOCK so the new client socket becomes O_NONBLOCK. accept4() can
only be applied for server,not client*/
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
			debug("%s: %s:%s\n", __func__, sflag, pflag);
			show_addrinfo(ares);
			if (bind(s, ares->ai_addr, ares->ai_addrlen) == -1)//comment A
				err(1, "bind err");
			freeaddrinfo(ares);
			//testpoll(s);
		}
		set_common_sockopts(s, res->ai_addr);
/*for tcp c/s, no matter if s has -k, the 2nd client can connect() ok after the 1st client has been keeping
occupying server.Wireshark show the 2nd clients three-way handshakes exists and it is right. The data sending
from the 2nd client is rxed by server but app layer has not handle the rx yet as the server is handling the 1st
client. Once the 1st client exits, if server is launched with -k, then server will handle the buffered data
from the 2nd client.
*/
		if ((error = connect_with_timeout(s, res->ai_addr, res->ai_addrlen, timeout)) == CONNECTION_SUCCESS)
			break;
		warn("connect to %s:%s (%s) err %d", host, port, uflag? "udp": "tcp", error);
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
	debug("%s(%d,) %ld, %d\n", __func__, fd, n, errno);
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
	debug("%s(%d,) %ld, %d\n", __func__, fd, n, errno);
	if (n == -1 && (errno == EAGAIN || errno == EINTR))//can be n==-1 && errno==EDESTADDRREQ for udp's server sending when `-lku 1080`
		n = -2;
	if (n <= 0)
		return n;
	if (g_fd > 0) {
		write(g_fd, buf, n);
		fsync(g_fd);
	}
	if (*bufpos - n > 0)
		memmove(buf, buf + n, *bufpos - n);//memmove can handle overlap
	*bufpos -= n;
	return n;
}
/*
1.`nc -l 1080`, input some chars from stdin, then `nc 127.0.0.1 1080`. The former inputed chars are
got in one batch by client. The reason I think is the inputed chars are in buffers of stdin?
2. `./a.out -luk 1080`,`./a.out 10.0.0.1 1080`;<--netstat shows "ESTABLISHED" even it is udp:21777280:udp        0      0 10.0.0.1:54843          10.0.0.1:1080           ESTABLISHED 4958/./a.o
client inputs then server can get, server inputs but
client can't get as server with `-luk` will not call connect() so read/write in readwrite() has not specified sockaddr!
a.server inputs from stdin is ok, then it send it via net socket, the send by write() is -1, errno 89(EDESTADDRRESQ);
then POLL_NETOUT .fd assigned with -1 and stdinbufpos not changed as write() return -1;
errno 89:server's -luk will not use connect() to give socket addr for later rx/tx so some sending function w/t
address eg write() will fail.
b.server inputs from stdin is ok for the 2nd time, this time POLL_STDIN .fd assigned with -1, fillebuf()
will read(-1,... so it return -1,errno==EBADF
c.server inputs from stdin for the 3rd time. poll() will not return as its POLL_STDIN has been ignored
POLL_NETIN=>POLL_STDOUT chain is still ok!
3. `./a.out -luk 1080`,`./a.out 10.0.0.1 1080`;then server ^c to exit; input sth at client, then client exits:
a. clint call write() to send its stdin input, the write() return ok and the written bytes# is returned(==expected)
APUE: If send()(send() has one more flag argument compared with write()) returns success, it doesn't necessarily means that
the process at the other end of the connection receives the data. All we are guaranteed is that when send() succeeds, the data
has been delived to the network drivers w/t error.
because the client has called connect() to server so the rx/tx in readwrite(), ie read()/write(), needs not specify server ip/port.
b. poll() return at once after a's "successful" write():both POLL_NETOUT and POLL_NETIN .revents is POLLERR, then these 2 .fd assigned
with -1, then POLL_STDIN assigned with -1, the next readwrite() main loop exits=>program exits.

4.`./a.out -lu 1080`:launch the 1st client:`./a.out -u 10.0.0.1 1080`, input from stdin to finish connect() to server
then ^c and launch the 2nd client that is the same as the 1st: input anything will make the 2nd exits directly. Then input anything
at server, the serve exits directly.
a. the 2nd client's connect() returns ok as no warn(...) triggered. The connect() for udp is just mark the later rx/tx sockaddr and no
real connection. input anything will make the 2nd exits directly as the write() is ok but poll() return POLL_NETIN & POLL_NETOUT with
.revent==POLLERR immediately after the input. Then sequence is the same as 3.a~b
b. server exits via any input then:its write(0 process cause exits and the process is the same as 3.a~b

5.to make 3.a~b happen more easily:`./a.out -u 127.0.0.10 1080` it first call connect(),then input
sth. It will call drainbuf to write(), which return the expected value successfully but poll() will
immediately return POLLERR for POLL_NETIN and POLL_NETOUT
*/
void readwrite(int net_fd)//can be used for both udp and tcp!
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
	while (1) {//2 chains: NETIN=>STDOUT STDIN=>NETOUT, if one chain's 2 .fd==-1, the other chain can still work
		if (pfd[POLL_STDIN].fd == -1 && pfd[POLL_NETIN].fd == -1 &&
			stdinbufpos == 0 && netinbufpos == 0) {
			debug("Here\n");
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
				debug("**here\n");
				return;
			}
delay_exit:
			close(net_fd);
			signal(SIGALRM, quit);
			alarm(qflag);//note arg typeof is unsigned in
		}
		num_fds = poll(pfd, 4, timeout);
		//debug("%s:%d %d %d\n", __func__, __LINE__, net_fd, num_fds);
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
			if (pfd[n].revents)
				debug("%x:%d %x\n", n, pfd[n].fd, pfd[n].revents);
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
		if (pfd[POLL_NETOUT].fd == -1)//If NETOUT is disabled, STDIN is disabled too
			pfd[POLL_STDIN].fd = -1;
		if (pfd[POLL_STDOUT].fd == -1) {
			if (pfd[POLL_NETIN].fd != -1)
				shutdown(pfd[POLL_NETIN].fd, SHUT_RD);
			pfd[POLL_NETIN].fd = -1;//why not full close since it is not used any more?
		}
		//1/2 chain:POLL_STDIN=>POLL_NETOUT
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
		//2/2 chain:POLL_NETIN=>POLL_STDOUT
		if (pfd[POLL_NETIN].revents & POLLIN && netinbufpos < BUFSIZE) {
			ret = fillbuf(pfd[POLL_NETIN].fd, netinbuf, &netinbufpos);
			if (ret == -1)
				pfd[POLL_NETIN].fd = -1;
			if (ret == 0) {
				int r = shutdown(pfd[POLL_NETIN].fd, SHUT_RD);
				debug("SHUT_RD %d\n", r);
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
void open_rec()
{//don't use "~/tmp/data.txt": "No such file or directory"
	g_fd = open("/home/rednoah/tmp/data.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (g_fd < 0)
		warn("open dump error");
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
			if (uflag && kflag) {//server with '-luk' can get data from any client, eg multiple clients with the same ip but different ports
				readwrite(s);
			} else if (uflag && !kflag) {//server with '-lu' can only get data from one client with specified port&ip as it uses connect() to limit the peer addr&port. Peers with the same ip but different ports can't make server getting its data
				char buf[INET_ADDRSTRLEN];
				struct sockaddr_in *peer = (struct sockaddr_in*)&cliaddr;
				len = sizeof(cliaddr);
				if (recvfrom(s, buf, sizeof(buf), MSG_PEEK, (struct sockaddr*)&cliaddr, &len) == -1)
					break;
				if (connect(s, (struct sockaddr*)&cliaddr, len) == -1)
					break;
				inet_ntop(peer->sin_family, &peer->sin_addr, buf, sizeof(buf));
				debug("peer %d, %s:%d\n", peer->sin_family, buf, ntohs(peer->sin_port));
				readwrite(s);
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
				//open_rec();
				readwrite(connfd);
				//sleep(5);//test sequence:server launched, client lunched, c ^c, then server in CLOSE_WAIT, client in FIN_WAIT2 before the following close()
				close(connfd);//commentB
			}
			if (!kflag) {//`./a.out -lk 1080` can rx/tx with the 2nd client, while `./a.out -l 1080` can't. The later will exit() directly when client first exit(),eg by ^c.
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
		open_rec();
		readwrite(s);
	}
	if (s != -1)
		close(s);
	return 0;
}

