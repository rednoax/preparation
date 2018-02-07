/*
copied from:
external/toybox/toy/android/setprop.c
bionic/libc/bionic/system_property.cpp
*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <poll.h>
#include <stddef.h>
#include <unistd.h>

#define PROP_NAME_MAX   32
#define PROP_VALUE_MAX  92
#define PROP_MSG_SETPROP 1

struct prop_msg {
    unsigned cmd;
    char name[PROP_NAME_MAX];
    char value[PROP_VALUE_MAX];	
};

typedef struct prop_msg prop_msg;
static int wait_input = 0;
static const char property_service_socket[] = "./property_service";///dev/property_service";
static int send_prop_msg(const prop_msg *msg)
{
	const int fd = socket(AF_UNIX, SOCK_STREAM/*|SOCK_CLOEXEC*/, 0);
	if (fd == -1) {
		perror("socket error:");
		return -1;
	}
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	snprintf(addr.sun_path, sizeof(addr.sun_path), property_service_socket);
	addr.sun_family = AF_LOCAL;
	socklen_t alen = offsetof(struct sockaddr_un, sun_path) + strlen(addr.sun_path) + 1;
	if (connect(fd, (struct sockaddr*)&addr, alen) < 0) {
		close(fd);
		return -1;
	}
	if (wait_input) {
		int buf[32];
		printf("Now %d waiting for input\n", getpid());
		read(STDIN_FILENO, buf, sizeof(buf));
	}
	const int num_bytes = send(fd, msg, sizeof(prop_msg), 0);//write is also ok, send has flag option
	if (num_bytes < 0)//on error -1 is returned and errno is set appropriately
		perror("send error:");
	else
		printf("send %d B\n", num_bytes);
	int result = -1;
	if (num_bytes == sizeof(prop_msg)) {
		struct pollfd pollfd[1];
		char buf[32];
		int nr;
	/*
	The field events is an input parameter, a bit mask specifying the events the application is interested in for the file descriptor fd.  This
	field may be specified as zero, in which case the only events that can be returned in revents are POLLHUP, POLLERR, and POLLNVAL (see below).

	The field revents is an output parameter, filled by the kernel with the events that actually  occurred. The bits returned in revents can
	include any of those specified in events, or one of the values POLLERR, POLLHUP, or POLLNVAL. (These three bits are meaningless in the events
	field, and will be set in the revents field whenever the corresponding condition is true.)
	POLLHUP
	 Hang up (only returned in revents; ignored in events).  Note that when reading from a channel such as a pipe or a stream  socket,
	 this event merely indicates that the peer closed its end of the channel.  Subsequent reads from the channel will return 0 (end of
	 file) only after all outstanding data in the channel has been consumed.
	*/
		pollfd->fd = fd;
		pollfd->events = 0;
		pollfd->revents = 0;
		nr = poll(pollfd, 1, 250);
		printf("poll %d, revents %x\n", nr, pollfd->revents);
		nr = read(fd, buf, sizeof(buf));
		buf[nr] = 0;
		printf("read [%s]\n", buf);
		result = 0;
	}
	close(fd);
	return result;
}

int __system_property_set(const char *key, const char *value)
{
	prop_msg msg;
	memset(&msg, 0, sizeof(msg));
	msg.cmd = PROP_MSG_SETPROP;
	strcpy(msg.name, key);//, sizeof(msg.name));
	strcpy(msg.value, value);//, sizeof(msg.value));
	return send_prop_msg(&msg);
}

int main(int argc, char **argv)
{
	const char *key = "selinux.reload_policy";
	const char *value = "1";
	int ret;
	while ((ret = getopt(argc, argv, "w")) != -1) {
		switch(ret) {
		case 'w':
			wait_input = 1;
			break;
		default:
			printf("unknow option %c\n", optopt);
			break;
		}

	}
	__system_property_set(key, value);
	return 0;
}