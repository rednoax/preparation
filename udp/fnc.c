#include <stdio.h>
#include <string.h>//memset()
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>//int getaddrinfo()
#include <netdb.h>//getaddrinfo(), gai_strerror()
#include <arpa/inet.h>//inet_ntop()
#include <errno.h>//for global var errno
#include <err.h>//err() errx()
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
	else
		warnx("getnameinfo:%s", gai_strerror(herr));
	fprintf(stderr, "%s on %s:%s\n", msg, host, port);
}

int local_listen(const char *host, const char *port, struct addrinfo hints)
{
	int s = -1, ret = 0, x = 1;
	int error;
	struct addrinfo *res, *res0;
	hints.ai_flags |= AI_PASSIVE;//allow node name to be NULL
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
		//ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(x));
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
	} else {
	}
	return 0;
}

