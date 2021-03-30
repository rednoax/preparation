#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <err.h>
#include <arpa/inet.h>
#include <string.h>
int nflag;

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

/*
As with gethostbyname(), getaddrinfo() may need to send a request to a DNS
server, and this request may take some time to complete. The same applies for
getnameinfo(), which we describe in Section 59.10.4.
when getnameinfo() successes, the POed host and port can be numeric,eg:
$ ./a.out baidu.com 80
flags: passive
family: inet
type: stream
protocol: tcp
canon name: (null)      220.181.38.148:80        assert 2==2
        220.181.38.148:80<--
...
sometimes the POed host and port can be presentation string,eg:
$ ./a.out github.com 80
flags: passive
family: inet
type: stream
protocol: tcp
canon name: (null)      13.250.177.223:80        assert 2==2
        gist.github.com:80<--
...
----the following is to bypass time-consuming dns lookup
NI_NUMERICHOST:skip time-consuming dns lookup, return numeric host address instead of name
NI_NUMERICSERV:return the numeric form of the service port number instead of name.
comparing the differences(both flags==NI_NUMERICSERV initially):
$ ./a.out 13.250.177.223 22
flags: passive
family: inet
type: stream
protocol: tcp
canon name: (null)      13.250.177.223:22        assert 2==2
        gist.github.com:22<--the name is got
...
$ ./a.out -n 13.250.177.223 22
flags: passive numhost
family: inet
type: stream
protocol: tcp
canon name: (null)      13.250.177.223:22        assert 2==2
        13.250.177.223:22<--numeric host address rather than name is returned directly
*/
void report_sock(const struct sockaddr *sa, socklen_t salen, char *path)
{
	char host[NI_MAXHOST], port[NI_MAXSERV];
	int herr, flags = NI_NUMERICSERV;/*if flags=0:`./a.out -n 13.250.177.223 ssh`'s getnameinfo():
	$ ./a.out -n 13.250.177.223 ssh
	flags: passive numhost
	family: inet
	type: stream
	protocol: tcp
	canon name: (null)		13.250.177.223:22		 assert 2==2<--getaddrinfo translater ssh to 22 as AI_NUMERICSERV not specified
			13.250.177.223:ssh<--but 22 is translated into 'ssh' as NI_NUMERICSERV specifed for getnameinfo()
$ ./a.out -n 13.250.177.223 22
flags: passive numhost
family: inet
type: stream
protocol: tcp
canon name: (null)      13.250.177.223:22        assert 2==2<--ditto
        13.250.177.223:ssh<--ditto*/
	if (nflag)
		flags |= NI_NUMERICHOST;
	memset(host, 0, sizeof(host));
	memset(port, 0, sizeof(port));
	herr = getnameinfo(sa, salen, host, sizeof(host), port, sizeof(port), flags);
/*
apue: if getaddrinfo fails, we can't use perror or strerror to genrate an error message.
Instead, we need to call gai_strerror to convert the error code returned into an error message.
From test it applies to getnameinfo() too. If using warn() to replace warnx() below, it will
appends strerror(errno). Since getnameinfo()&getaddrinfo() don't use errno for !EAI_SYSTEM,
you may get error like:
a.out: getnameinfo:ai_family not supported: Success<--errno is 0
*/
	if (herr == EAI_SYSTEM)//system error returned in @errno
		warn("getnameinfo ");
	else if (herr)//to avoid gai_strerror(0) occur, which is "Uknown error"
		warnx("getnameinfo:%s", gai_strerror(herr));
	fprintf(stderr, "\t%s:%s\n", host, port);
}


int local_listen(const char *host, const char *port, struct addrinfo *hints)
{
	int s = -1;
	int error;
	struct addrinfo *res, *res0;
	hints->ai_flags |= AI_PASSIVE;//allow node name to be NULL
	if (host == NULL && hints->ai_family == AF_UNSPEC)
		hints->ai_family = AF_INET;
	if ((error = getaddrinfo(host, port, hints, &res0)))
		errx(1, "getaddrinfo: %s", gai_strerror(error));//err should not be used as getaddrinfo do't use errno when it fails
	for (res = res0; res; res = res->ai_next) {
		show_addrinfo(res);
		report_sock(res->ai_addr, res->ai_addrlen, NULL);
	}
	freeaddrinfo(res0);
	return s;
}
/*eg:./a.out baidu.com 80
*/
int main(int argc, char **argv)
{
	char c;
	//int optind;
	char *host = NULL, *port = NULL;
	struct addrinfo hints = {
		.ai_family = AF_UNSPEC,
	};
	while ((c = getopt(argc, argv, "n")) != -1) {
		switch (c) {
		case 'n':
			nflag = 1;
			break;
		default:
			printf("'%c' not supported %c\n", optopt, c);
		}
	}
	argc -= optind;
	argv += optind;
	if (argc == 1) {
		port = argv[0];
	} else if (argc == 2) {
		host = argv[0];
		port = argv[1];
	}
	if (port == NULL)
		errx(1, "[-n] [addr] [port]\n");
/*
$ ./a.out g.cn 1080
flags: passive
family: inet
type: stream
protocol: tcp
canon name: (null)      203.208.50.66:1080       assert 2==2
        203.208.50.66:1080
...
$ ./a.out -n g.cn 1080<-- -n to skip dns lookup
a.out: getaddrinfo: Name or service not known
*/
	if (nflag)
		hints.ai_flags |= AI_NUMERICHOST;
	local_listen(host, port, &hints);
	return 0;
}
