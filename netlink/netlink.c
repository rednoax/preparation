#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


static char buf[4096];
//
static const char *_ulog_ident = "netlink";


static void ulog_kmsg(int priority, const char *fmt, va_list ap)
{
#if 0
	FILE *kmsg;

	if ((kmsg = fopen("/dev/kmsg", "r+")) != NULL) {
		fprintf(kmsg, "<%u>", priority);

		if (_ulog_ident)
			fprintf(kmsg, "[%d]%s: ", getpid(), _ulog_ident);

		vfprintf(kmsg, fmt, ap);
		fclose(kmsg);
	}
#else
	static int fd = -1;
	static char kmsg[2048];
	if (fd < 0) {
		fd = open("/dev/kmsg", O_RDWR | O_APPEND);
	}
	if (fd >= 0) {
		int ret = 0;
		if (_ulog_ident)
			ret = snprintf(kmsg, sizeof(kmsg), "<%u>[%d]%s: ", priority, getpid(), _ulog_ident);
		ret += vsnprintf(kmsg + ret, sizeof(kmsg) - ret, fmt, ap);
		if (ret >= sizeof(kmsg)) {
			kmsg[sizeof(kmsg) - 1] = 0;
			ret = sizeof(kmsg);
		}
		write(fd, kmsg, ret);
	}
#endif
}

void ulog(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ulog_kmsg(7, fmt, ap);
	va_end(ap);
}
//
void recv_print()
{
	struct sockaddr_nl nls;
	int nlbufsize = 512 * 1024;
	int fd, len;
	memset(&nls,0,sizeof(struct sockaddr_nl));
	nls.nl_family = AF_NETLINK;
	nls.nl_pid = getpid();
	nls.nl_groups = -1;

	if ((fd = socket(PF_NETLINK, SOCK_DGRAM | SOCK_CLOEXEC, NETLINK_KOBJECT_UEVENT)) == -1) {
		ulog("Failed to open hotplug socket: %s\n", strerror(errno));
		exit(1);
	}
	if (bind(fd, (void *)&nls, sizeof(struct sockaddr_nl))) {
		ulog("Failed to bind hotplug socket: %s\n", strerror(errno));
		exit(1);
	}

	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUFFORCE, &nlbufsize, sizeof(nlbufsize)))
		ulog("Failed to resize receive buffer: %s\n", strerror(errno));
	while ((len = recv(fd, buf, sizeof(buf), 0)) > 0) {
		int i = 0;
		while (i < len) {
			int l = strlen(buf + i) + 1;
			i += l;
			buf[i - 1] = ' ';
		}
		sprintf(&buf[i - 1], " @\n");
		ulog(buf);	
	}
}

int main()
{
	recv_print();
	return 0;
}
