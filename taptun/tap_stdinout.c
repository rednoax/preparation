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

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
int debug = 1;
enum {
	SERVER, CLIENT
};
/*
So this is an attempt to explain what happens when ioctl(TUNSETIFF) is called, and how the kernel differentiates between the request for the allocation of a new interface and the request to connect to an existing interface:
If a non-existent or no interface name is specified, that means the user is requesting the allocation of a new interface. The kernel thus creates an interface using the given name (or picking the next available name if an empty name was given). This works only if done by root.
If the name of an existing interface is specified, that means the user wants to connect to a previously allocated interface. This can be done by a normal user, provided that: the user has appropriate rights on the clone device AND is the owner of the interface (set at creation time), AND the specified mode (tun or tap) matches the mode set at creation time.
*/
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
		fprintf(stderr, "ioctl '%s'=>'%s': %d\n", if_name, ifr.ifr_name, ret);
	strncpy(if_name, ifr.ifr_name, IFNAMSIZ - 1);
	return fd;
}
/*
$ip link show dev tap0
3: tap0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc fq_codel state DOWN mode DEFAULT group default qlen 1000
    link/ether fe:bb:dc:c0:fe:19 brd ff:ff:ff:ff:ff:ff
ethernet MTU is 1500, so the buf should be shrinked to 1500!
*/
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
1.an application use standard socket api rather than ioctl() like tun_alloc() will use tapX's ip directly.
eg ping from 10.0.0.1 will generate ICMP packet with source ip==10.0.0.1;
ssh client from 10.0.0.1 will generate tcp packet with source ip==10.0.0.1;
2.
vm1 192.168.193.129:
$ sudo tunctl -t tap0 -u rednoah
$ sudo ifconfig tap0 10.0.0.1

vm2 192.168.1.147:
$ sudo tunctl -t tap0 -u rednoah
$ sudo ifconfig tap0 10.0.0.2
$ nc -l 192.168.1.147 1080 <~/tmp/fifo | sudo ./a.out -i tap0 >~/tmp/fifo

vm1 ping 10.0.0.2, fails!
PING 10.0.0.2 (10.0.0.2) 56(84) bytes of data.
From 10.0.0.1 icmp_seq=1 Destination Host Unreachable

vm1:
$ sudo ./a.out -i tap0 <fifo |nc 192.168.1.147 1080 >fifo

then launch ping from vm1 in another terminal and ping is ok.

vm1 ssh client connects vm2 tap0 10.0.0.2's ssh server from 10.0.0.1
$ ssh rednoah@10.0.0.2
rednoah@10.0.0.2's password:
Welcome to Ubuntu 20.04.2 LTS (GNU/Linux 5.8.18 x86_64)<--vm2:20.04 vm1:18.04

NOTE:after vm1上的程序退出后, vm2上的a.out会频繁报:POLLHUP for STDIN_FILENO, 原因是?
首先看看vm1的程序没有断开之前 vm2上运行ps aux的结果同时包含nc server和a.out, 注意ps –efj显示前者并非后者父进程
rednoah     4658  0.0  0.0   3260   828 pts/0    S+   03:19   0:00 nc -l 192.168.1.147 1080
root        4659  0.0  0.0  11920  4572 pts/0    S+   03:19   0:00 sudo ./a.out -i tap0
vm1客户端退出后,vm2上的nc server退出了,但是a.out还在, nc server将接收到的网络数据传给a.out的stdin, 对a.out而言
其位于管道的读端, 而nc sever位于管道的写端, 据LPI表63-4:处于pipe or fifo读端的poll()返回POLLHUP说明
Data in pip? no. Write end open? no.
*/
void watch(int fd)
{
	int i, r, w;
	struct pollfd fds[] = {
		[0] = {
			.fd = STDIN_FILENO,
			.events = POLLIN,
		},
		[1] = {
			.fd = fd,
			.events = POLLIN,
		},
	};
	while ((r = poll(fds, ARRAY_SIZE(fds), -1)) > 0) {
		for (i = 0; i < ARRAY_SIZE(fds); i++) {
			fprintf(stderr, "%d: %x\n", fds[i].fd, fds[i].revents);
			if (fds[0].revents & POLLHUP)//write end NOT open and no data in pipe, see comment 2
				return;
		}
		if (fds[0].revents & POLLIN) {
			r = read(fds[0].fd, buf, sizeof(buf));//the input bytes must be >= some limit to avoid "Invalid argument" error
			if (r > 0) {
				w = write(fds[1].fd, buf, r);
				warn("%d => %d", r, w);
				fflush(stdout);
			}
		}
		if (fds[1].revents & POLLIN) {
			r = read(fds[1].fd, buf, sizeof(buf));
			if (r > 0)
				dump(buf, r);//write(STDOUT_FILENO, buf, r);
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
$sudo tunctl -u rednoah -t tap0
$sudo ifconfig tap0 10.0.0.1<--then tap0 is up, no need to `ifconfig tap0 up` once more.
$./a.out -i tap0
$ping -c 1 10.0.0.[2,254]<-- will make ./a.out sends arp reply and ICMP reply.
*/
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
/*
IFF_NO_PI:"pure" IP packets, with no added bytes. Otherwise (if IFF_NO_PI is unset), 4 extra bytes
are added to the beginning of the packet (2 flag bytes and 2 protocol bytes). IFF_NO_PI need not
match between interface creation and reconnection time. Also note that when capturing traffic on
the interface with Wireshark, those 4 bytes are never shown.
*/
	if ((tap_fd = tun_alloc(if_name, flags | IFF_NO_PI)) < 0) {
		my_err("Error connecting to %s interface %s\n", flags & IFF_TUN? "TUN": "TAP", if_name);
		exit(1);
	}
	do_debug("connected to %s\n", if_name);
/*
test when commentA enabled:
0.If no any tapX in `ifconfig -a`, `sudo ./a.out` w/t '-i' will generate 'tap0' only during this
program's running. Once the program exit(), 'tap0' disappeared.(When the program closes the file
descriptor, the network device and all corresponding routes will disappear.).
In other words, tun_alloc("", ...) will ask the kernel to try to allocate the 'next' available
interface of the requested type(eg, tap0, but if that already exists, tap1, and so on); and the
1st arg will PO the actually used name allocated by kernel.
1. If the 'tap0' has existed(`sudo tunctl -u rednoah -t tap0`, no need to ifconfig tap0 ip) before
`./a.out -i tap0`, tun_alloc() will not create new tap device and ioctl of 'tap0' return 0(ok).
In other word the program will not create new tapX device but ATTACH TO the specified existed tap0 directly.
2. If the tap0 has not been setup like 1, then `sudo ./a.out ` will create tap1, which only exists
during this program's running.
3. If attaching to an existing tap0, whose user is equal to the user lauching `./a.out -i tap0`. No root
sudo is needed for `./a.out -i tap0`; If a persistent interface(by tunctl ...) owned by a specific
user, so processes running as that user can successfully attach to it.
4. the MAC address for a tap interface is autogenerated by the kernel at interface creation time;
*/
	//sleep(10); exit(1);//commentA
	watch(tap_fd);
	return 0;
}
