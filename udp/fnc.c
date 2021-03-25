#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>//int getaddrinfo()
#include <netdb.h>//getaddrinfo(), gai_strerror()
#include <arpa/inet.h>//inet_ntop()
int kflag, lflag, nflag, uflag;

int family = AF_UNSPEC;

void usage(int ret)
{
	fprintf(stderr,
		"usage: nc [-klu]\n"
		"\t  [destination] [port]");
	if (ret)
		exit(1);//`man 3 exit` show <stdlib.h> should be included.
}

int local_listen(const char *host, const char *port, struct addrinfo hints)
{
	int error;
	struct addrinfo *res, *res0;
	hints.ai_flags |= AI_PASSIVE;//allow what?
	if (host == NULL && hints.ai_family == AF_UNSPEC)
		hints.ai_family = AF_INET;
	if ((error = getaddrinfo(host, port, hints, &res0)))
//1.The errx() and warnx() functions do not append an error message.
//2.The err(), verr(), warn(), and vwarn() functions append an error message obtained from strerror(3) based on the global variable errno
//3.perror, strerror should not be used when getaddrinfo fails. gai_strerror() should be used.
		errx(1, "getaddrinfo: %s", gai_strerror(error));
	for (res = res0; res; res = res->ai_next) {
		printf("%x:%x:%x:%x:%s\n", res->ai_flags, res->ai_family,
			res->ai_socktype, res->ai_protocol, res->ai_canonname);
		if (res->ai_family == AF_INET) {
			char buf[INET_ADDRSTRLEN];
			struct sockaddr_in *sinp = (struct sockaddr_in*)res->ai_addr;
			const char * s = inet_ntop(AF_INET, &sinp->sin_addr, buf, sizeof(buf));
			if (s) {
				printf("%p == %p?\n", s, buf);
				printf("%s:%d\n", buf, ntohs(sinp->sin_port));
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int ch, s = -1;
	char *host, **uport;
	struct addrinfo hints;
	host = uport = NULL;
	while ((ch = getopt(argc, argv, "klnu")) != -1) {
		switch (ch) {
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
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_TCP;
	}
	if (nflag)
		hints.ai_flags = AI_NUMERICHOST;
	if (lflag) {
		s = local_listen(host, *uport, hints);
	} else {
	}
	return 0;
}

