#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <arpa/inet.h>
#include <stddef.h>//offsetof
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
int debug = 1;
enum {
	SERVER, CLIENT
};

int tun_alloc(char *if_name, int flags)//if_name will be overwritten with the real device name after ioctl returns ok
{
	struct ifreq ifr;
	const char *dev = "/dev/net/tun";
	int fd = open(dev, O_RDWR);
	int ret;
	if (fd < 0)
		err(1, "open %s error", dev);
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = flags;
	strncpy(ifr.ifr_name, if_name, IFNAMSIZ - 1);
	if ((ret = ioctl(fd, TUNSETIFF, &ifr)) < 0) {//ok:>=0 err:-1
		close(fd);
		err(1, "ioctl %s error %d", dev, errno);//if no sudo, it ends up here with 'Operation not permitted'.If `tunctl -u rednoah -t tap0`:TUNSETIFF: Operation not permitted
	} else
		printf("ioctl '%s'=>'%s': %d\n", if_name, ifr.ifr_name, ret);
	strncpy(if_name, ifr.ifr_name, IFNAMSIZ - 1);
	return fd;
}

char buf[1<<20];

void dump(const void *_buf, int len)
{
	int i;
	const char *buf = _buf;
	printf("%d bytes\n", len);
	for (i = 0; i < len; i++) {
		printf("%02x ", buf[i] & 0xff);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n");
	fflush(stdout);
}

/*
If no reading for tap0, ie this program is not run. Wireshark can't capture
anything even there is incoming sending like `ping 10.0.0.2`.
*/
void relay(int s, int tap)
{
	int i, r, w;
	(void)w;
	struct pollfd fds[] = {
		[0] = {
			.fd = s,
			.events = POLLIN,
		},
		[1] = {
			.fd = tap,
			.events = POLLIN,
		},
	};
	printf("%d %d\n", s, tap);
	while ((r = poll(fds, ARRAY_SIZE(fds), -1)) > 0) {
		for (i = 0; i < ARRAY_SIZE(fds); i++) {
			printf("%d: %d %x\n", i, fds[i].fd, fds[i].revents);
			if (fds[i].revents & (POLLNVAL | POLLHUP))
				return;
		}
		if (fds[0].revents & POLLIN) {
			r = read(fds[0].fd, buf, sizeof(buf));
			if (r == 0)//peer close() or SHUT_WR
				return;
			w = write(fds[1].fd, buf, r);
		}
		if (fds[1].revents & POLLIN) {
			r = read(fds[1].fd, buf, sizeof(buf));
			w = write(fds[0].fd, buf, r);
		}
	}
	err(1, "poll() error %d", r);//-1 or 0(timeout)
}

void do_debug(const char *fmt, ...)
{
	va_list argp;
	if (debug) {
		va_start(argp, fmt);
		vfprintf(stderr, fmt, argp);
		va_end(argp);
	}
}

void my_err(const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
}

void usage()
{
	fprintf(stderr, "Usage:\n"
		"-i <ifacename> [-s|-c <serverIP>] [-p <port>] [-u|-a] [-d]\n"
		"-h\n\n");
	exit(1);
}
/*
vm1@192.168.193.129(NAT)
$ sudo tunctl -t tap0 -u rednoah
Set 'tap0' persistent and owned by uid 1000
$ sudo ifconfig tap0 10.0.0.1
$ sudo ./a.out -i tap0 -c 192.168.1.147
==
$ sudo ./a.out -i tap0 -c 10.0.0.2<--don't use a wrong ip address
ioctl 'tap0'=>'tap0': 0
connected to tap0
a.out: connect(): No route to host
==
vm2@192.168.1.147(BRIDGED):
$sudo tunctl -u rednoah -t tap0
$sudo ifconfig tap0 10.0.0.2
$sudo ./a.out -i tap0 -s<==then ssh 10.0.0.1 at vm2 or ssh 10.0.0.2 at vm1 can work at the same time.
*/
int main(int argc, char *argv[])
{
	int c, tap_fd, s;
	char if_name[IFNAMSIZ] = {0};
	const char* remote_ip = NULL;
	int cliserv = -1;
	int port = 1080, flags = IFF_TAP;//2layer ethernet frame default, rather than 3layer IP packet
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr = {
			htonl(INADDR_ANY)
		},
		.sin_port = htons(port),
	};
	while ((c = getopt(argc, argv, "i:sc:p:uahd")) != -1) {
		switch (c) {
		case 'd':
			debug = 1;
			break;
		case 'h':
			usage();
			break;
		case 'i':
			strncpy(if_name, optarg, IFNAMSIZ - 1);
			break;
		case 's':
			cliserv = SERVER;
			break;
		case 'c':
			cliserv = CLIENT;
			remote_ip = optarg;
			break;
		case 'p':
			addr.sin_port = htons(atoi(optarg));
			break;
		case 'u':
			flags = IFF_TUN;
			break;
		case 'a':
			flags = IFF_TAP;
			break;
		default:
			my_err("%c not supported\n", optopt);
			usage();
		}
	}
	argc -= optind;
	argv += optind;
	if (argc > 0) {
		my_err("Too many options!\n");
		usage();
	}
#if 0
	if (*if_name == '\0') {
		my_err("Must specify interface name!\n");
		usage();
	} else
#endif
	if (cliserv < 0) {
		my_err("Must specify client or server mode!\n");
		usage();
	} else if (cliserv == CLIENT && !remote_ip) {
		my_err("Must specify server address!\n");
		usage();
	}
	if ((tap_fd = tun_alloc(if_name, flags | IFF_NO_PI)) < 0) {
		my_err("Error connecting to %s interface %s\n", flags & IFF_TUN? "TUN": "TAP", if_name);
		exit(1);
	}
	do_debug("connected to %s\n", if_name);
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1)
		err(1, "socket err");
	if (cliserv == SERVER) {
		char peer[INET_ADDRSTRLEN];
		unsigned int len = sizeof(struct sockaddr_in), x = 1;
		if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(x)) == -1)
			err(1, "setsockopt");
		if (bind(s, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1)
			err(1, "bind err");
		if (listen(s, 0) == -1)
			err(1, "listen err");
		c = accept(s, (struct sockaddr*)&addr, &len);
		fprintf(stderr, "accept peer %s:%d\n", inet_ntop(addr.sin_family, &addr.sin_addr, peer, sizeof(peer)),
			ntohs(addr.sin_port));
		if (c == -1)
			err(1, "accept");
		relay(c, tap_fd);
		close(c);
	} else {
		inet_pton(addr.sin_family, remote_ip, &addr.sin_addr);
		if (connect(s, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1)
			err(1, "connect()");
		relay(s, tap_fd);
	}
	close(s);
	close(tap_fd);
	return 0;
}
