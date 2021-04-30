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
const char all0Mac[6] = {0};
const char qemuMac[6] = {0x52, 0x54, 0x00};

struct EthernetHeader {
	char DestMac[6];
	char SrcMac[6];
	short FrameType;
} __attribute__((packed));

struct arp {//apply to both arp request and arp reply
	struct EthernetHeader ethHeader;
	short HardType;
	short ProtType;
	char HardSize;
	char ProtSize;
	short opcode;
	char SenderMac[6];
	unsigned int SenderIp;//changed to unsigned since >>
	char TargetMac[6];
	unsigned int TargetIp;
} __attribute__((packed));//42B

#define DestMac ethHeader.DestMac
#define SrcMac ethHeader.SrcMac
#define FrameType ethHeader.FrameType


void dump(const char *buf, int len)
{
	int i;
	printf("%d bytes\n", len);
	for (i = 0; i < len; i++) {
		printf("%02x ", buf[i] & 0xff);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n");
	fflush(stdout);
}

int isBroadcast(const char *buf)
{
	int i, r = 1;
	for (i = 0; i < 6; i++) {
		if ((buf[i] & 0xff) != 0xff) {//precedence: != is one more higher than &
			r = 0;
			break;
		}
	}
	return r;
}

int arp_request(void *buf, int r)
{
	const struct arp *req = buf;
	int ret = 0;
	if (r == sizeof(struct arp) && isBroadcast(req->DestMac)
		&& req->FrameType == htons(0x0806)/*apply to both arp request & reply; 0x0800:ipv4*/
		&& req->HardType == htons(0x1)/*Ethernet*/
		&& req->ProtType == htons(0x0800)/*IPV4, which means arp request asks .HardType hw address corresponding to .ProtType, ie ethernet address of ipv4 here*/
		&& req->HardSize == 6 && req->ProtSize == 4//For an arp request/reply for an IP address on an Ethernet, mac address is 6B and ip address is 4B
		&& req->opcode == htons(0x1)//1:arp request 2:arp reply;opcode is necessary since arp request/reply share the same .FrameType 0x0806
		&& !memcmp(req->SenderMac, req->SrcMac, sizeof(req->SenderMac))
		&& !memcmp(req->TargetMac, all0Mac, sizeof(req->TargetMac))) {
		ret = 1 ;
	}
	return ret;//precedence:+- >><<
}

struct arp* arp_reply(void *buf)
{
	const struct arp *req = buf;
	struct arp *reply = req + 1;
	memcpy(reply->DestMac, req->SrcMac, sizeof(reply->DestMac));
	memcpy(reply->SrcMac, qemuMac, sizeof(reply->SrcMac));
	reply->SrcMac[5] = (req->TargetIp >> 24) & 0xff;
	reply->FrameType = req->FrameType;
	reply->HardType = req->HardType;
	reply->ProtType = req->ProtType;
	reply->HardSize = req->HardSize;
	reply->ProtSize = req->ProtSize;
	reply->opcode = htons(0x2);//1:arp request 2:arp reply
	memcpy(reply->SenderMac, reply->SrcMac, sizeof(reply->SenderMac));
	reply->SenderIp = req->TargetIp;
	memcpy(reply->TargetMac, req->SenderMac, sizeof(reply->TargetMac));
	reply->TargetIp = req->SenderIp;
	return reply;
}
/*
If no reading for tap0, ie this program is not run. Wireshark can't capture
anything even there is incoming sending like `ping 10.0.0.2`.
*/
void watch(int fd)
{
	int i, r, w;
	struct pollfd fds[] = {
		[0] = {
			.fd = fd,
			.events = POLLIN,
		},
	};
	//int cfd = open("data.cap", O_CREAT | O_TRUNC | O_RDWR, 0644);//droped as wireshark/tcpdump can't recognize such binary format.
	while ((r = poll(fds, ARRAY_SIZE(fds), -1)) > 0) {
		for (i = 0; i < ARRAY_SIZE(fds); i++) {
			printf("%d: %x\n", fds[i].fd, fds[i].revents);
		}
		if (fds[0].revents & POLLIN) {
			r = read(fds[0].fd, buf, sizeof(buf));
			/*w = write(cfd, buf, r);
			warn("%d == %d?", r, w);
			fsync(cfd);*/
			dump(buf, r);
			if (arp_request(buf, r))
				fds[0].events |= POLLOUT;
		}
		if (fds[0].revents & POLLOUT) {
			struct arp *reply = arp_reply(buf);
			fds[0].events &= ~POLLOUT;
			r = write(fds[0].fd, reply, sizeof(struct arp));
			/*printf("w %dB\n", r);
			dump(reply, r);*/
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

int main(int argc, char *argv[])
{
	int c, tap_fd;
	char if_name[IFNAMSIZ] = {0};
	const char* remote_ip = NULL;
	int cliserv = -1;
	int port = 1080, flags = IFF_TAP;//2layer ethernet frame default, rather than 3layer IP packet
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
			port = atoi(optarg);
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
	}
	else if (cliserv < 0) {
		my_err("Must specify client or server mode!\n");
		usage();
	} else if (cliserv == CLIENT && !remote_ip) {
		my_err("Must specify server address!\n");
		usage();
	}
#endif
	if ((tap_fd = tun_alloc(if_name, flags | IFF_NO_PI)) < 0) {
		my_err("Error connecting to %s interface %s\n", flags & IFF_TUN? "TUN": "TAP", if_name);
		exit(1);
	}
	do_debug("connected to %s\n", if_name);
/*
test when commentA enabled:
0.If no any tapX in `ifconfig -a`, `sudo ./a.out` w/t '-i' will generate 'tap0' only during this
program's running. Once the program exit(), 'tap0' disappeared.(When the program closes the file
descriptor, the network device and all corresponding routes will disappear.)
1. If the 'tap0' has existed(`sudo tunctl -u rednoah -t tap0`, no need to ifconfig tap0 ip) before
`sudo ./a.out -i tap0`, tun_alloc() will not create new tap device and ioctl of 'tap0' return 0(ok).
In other word the program will not create new tapX device but use the specified existed tap0 directly.
2. If the tap0 has not been setup like 1, then `sudo ./a.out ` will create tap1, which only exists
during this program's running.
*/
	//sleep(10); exit(1);//commentA
	watch(tap_fd);
	return 0;
}
