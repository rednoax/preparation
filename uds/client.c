#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#define CS_OPEN "/tmp/server.socket"
#define MAXLINE 4096

int cli_conn(const char *name)
{
	int fd = -1, rval, err;
	socklen_t len;
	struct sockaddr_un un, sun;
	un.sun_path[0] = 0;
	if (strlen(name) >= sizeof(un.sun_path)) {
		errno = ENAMETOOLONG;
		rval = -1;
		goto errout;
	}
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		rval = -1;
		goto errout;
	}
#if 1	
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path, "/tmp/%05d", getpid());
	printf("socket:%s\n", un.sun_path);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	unlink(un.sun_path);
	if (bind(fd, (struct sockaddr*)&un, len) == -1) {
		rval = -2;
		un.sun_path[0] = 0;
		goto errout;
	}
#endif	
	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	if ((connect(fd, (struct sockaddr*)&sun, len)) == -1) {
		rval = -4;
		goto errout;
	}
	return fd;
errout:
	err = errno;
	if (fd != -1)
		close(fd);
	if (un.sun_path[0])
		unlink(un.sun_path);
	errno = err;
	return rval;
}

void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char buf[MAXLINE];
	vsnprintf(buf, MAXLINE - 1, fmt, ap);
	if (errnoflag)
		snprintf(buf + strlen(buf), MAXLINE - 1 - strlen(buf), ": %s", strerror(errno));
	strcat(buf, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
}

void err_ret(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

int main(int argc, char **argv)
{
	char line[MAXLINE];
	int csfd, send_nr, nr;
	//offsetof(struct sockaddr_un, sun_path) is 2
	if ((csfd = cli_conn(CS_OPEN))< 0) {
		err_ret("cli_conn error");
		return -1;
	}
	while (fgets(line, MAXLINE, stdin) != NULL) {
		send_nr = strlen(line);// + 1;
		if ((nr = write(csfd, line, send_nr) != send_nr)) {
			err_ret("write error %d != %d", nr, send_nr);
			return -1;
		}
		nr = read(csfd, line, sizeof(line));
		write(STDOUT_FILENO, line, nr);
	}
	return 0;
}