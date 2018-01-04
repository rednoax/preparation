#include <sys/types.h>
#include <sys/socket.h>/*protocol family for communication*/
#include <sys/stat.h>
#include <sys/un.h>
#include <syslog.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <stdarg.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <stddef.h>
#include <time.h>
#define CS_OPEN "/tmp/server.socket"
int log_to_stderr = 1;
#define MAXLINE 4096
#define QLEN 50

void log_doit(int errnoflag, int error, int priority, const char *fmt, va_list ap)
{
	char buf[MAXLINE];
	vsnprintf(buf, MAXLINE - 1, fmt, ap);//to hold trailing '\n'
	if (errnoflag)
		snprintf(buf + strlen(buf), MAXLINE - 1 - strlen(buf), ": %s", strerror(error));
	strcat(buf, "\n");
	if (log_to_stderr) {
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	} else
		syslog(priority, "%s", buf);
}

void log_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(1, errno, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

void log_msg(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, 0, LOG_ERR/*LOG_INFO???*/, fmt, ap);
	va_end(ap);
}

void log_quit(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, 0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

int serv_listen(const char *name)
{
	int fd, len, err, rval;
	struct sockaddr_un un;
	if (strlen(name) >= sizeof(un.sun_path)) {
		errno = ENAMETOOLONG;
		return -1;
	}

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
		return -2;
	unlink(name);
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name) + 1;
	/*
	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
	it is normally necessary to assign a local address using bind before a
	SOCK_STREAM socket may receive connections.
	addrlen specifies the size, in bytes, of the address structure pointed by addr
	man 7 unix:
	Various systems calls (for example, bind(2), connect(2), and sendto(2)) take a sockaddr_un argument as input.  Some  other  system  calls  (for
	example, getsockname(2), getpeername(2), recvfrom(2), and accept(2)) return an argument of this type.

	Three types of address are distinguished in the sockaddr_un structure:

	pathname:  a  UNIX domain socket can be bound to a null-terminated filesystem pathname using bind(2).  When the address of a pathname socket
	is returned (by one of the system calls noted above), its length is
	offsetof(struct sockaddr_un, sun_path) + strlen(sun_path) + 1
	and sun_path contains the null-terminated pathname.
	On Linux, the above offsetof() expression equates to the same value as  sizeof(sa_family_t).

	unnamed: A stream socket that has not been bound to a pathname using bind(2) has no name.  Likewise, the  two  sockets  created  by  socket‐
	pair(2)  are  unnamed.   When  the  address  of an unnamed socket is returned, its length is sizeof(sa_family_t), and sun_path should not be
	inspected.

	...
	When binding a socket to a pathname, a few rules should be observed for maximum portability and ease of coding:
	*  The pathname in sun_path should be null-terminated.

	*  The length of the pathname, including the terminating null byte, should not exceed the size of sun_path.

	*  The addrlen argument that describes the enclosing sockaddr_un structure should have a value of at least:

		  offsetof(struct sockaddr_un, sun_path)+strlen(addr.sun_path)+1

	  or, more simply, addrlen can be specified as sizeof(struct sockaddr_un).(see man 2 bind's EXAMPLE part)

	...
	Various system calls (accept(2), recvfrom(2), getsockname(2), getpeername(2)) return socket address structures.  When applied  to  UNIX  domain
	sockets,  the  value-result addrlen argument supplied to the call should be initialized as above.  Upon return, the argument is set to indicate
	the actual size of the address structure.  The caller should check the value returned in this argument: if the output value exceeds  the  input
	value, then there is no guarantee that a null terminator is present in sun_path.  (See BUGS.)
	*/
	if (bind(fd, (struct sockaddr*)&un, len) == -1) {
		rval = -3;
		goto errout;
	}
	if (listen(fd, QLEN) == -1) {
		rval = -4;
		goto errout;
	}
	return fd;
errout:
	err = errno;
	close(fd);
	errno = err;
	return rval;
}

int serv_accept(int listenfd, uid_t *uidptr)
{
	int err, clifd = -1, rval;
	socklen_t len;
	struct sockaddr_un un;
	struct stat statbuf;
	uid_t uid = -1;
	char *name = NULL;
	if ((name = malloc(sizeof(un.sun_path))) == NULL) {//.sun_path is 108B
		rval = -1;
		goto errout;
	}
	/*
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
	@addr:no need to initialize, pure output
	caller must initialize addrlen to contain the size (in bytes) of
	the structure pointed to by addr; on return it will contain the
	actual size of the peer address.
	*/
	len = sizeof(un);
	/*
	when client don't use bind:
	1. un.sun_path[0] is not always '\0'. It can be full of rubbish.
	2. len returned by accept is always 2
	when client use bind
	1. un.sun_path is a '\0' teminated string
	2. len is (2 + strlen(.sun_path) + 1)
	for more, see unix(7)
	*/
	if ((clifd = accept(listenfd, (struct sockaddr*)&un, &len)) == -1) {
		rval = -2;
		goto errout;
	}
	/*
	else
		log_msg("new client %d, %dB:%d,%s", clifd, len, un.sun_family, un.sun_path);//len includes trailing 1B '\0' if client use bind
	*/
	len -= offsetof(struct sockaddr_un, sun_path);
#if 0	
	memcpy(name, un.sun_path, len);
	name[len] = 0;
#else
	strncpy(name, un.sun_path, len);
	name[len] = 0;//in case len is 0
	log_msg("new client %d, %dB:%d[%s]", clifd, len + offsetof(struct sockaddr_un, sun_path), un.sun_family, name);
#endif
	if (name[0]) {
		if (stat(name, &statbuf) == -1) {
			rval = -3;
			goto errout;
		}
		/*
		else
			log_msg("client uid %d", statbuf.st_uid);
		*/
		if (!S_ISSOCK(statbuf.st_mode)) {
			rval = -4;
			goto errout;
		}
		uid = statbuf.st_uid;
		unlink(name);//FIXME?
	}
	if (uidptr)
		uidptr[0] = uid;
	free(name);
	return clifd;
errout:
	err = errno;
	if (clifd != -1)
		close(clifd);
	if (name)
		free(name);
	errno = err;
	log_msg("***%d\n", rval);
	return rval;
}
typedef struct{
	int fd;
	uid_t uid;
}Client;

Client *client = NULL;
int client_size = 0;
#define NALLOC 10
void client_alloc(void)
{
	int i;
	if (!client)
		client = malloc(NALLOC * sizeof(Client));
	else
		client = realloc(client, (client_size + NALLOC) * sizeof(Client));
	if (!client)
		log_sys("can't alloc for client array");
	
	for (i = client_size; i < client_size + NALLOC; i++)
		client[i].fd = -1;
	client_size += NALLOC;
}

int client_add(int fd, uid_t uid)
{
	int i;
	if (!client)
		client_alloc();
again:
	for (i = 0; i < client_size; i++) {
		if (client[i].fd == -1) {
			client[i].fd = fd;
			client[i].uid = uid;
			return i;
		}
	}
	client_alloc();
	goto again;
}

void client_del(int fd)
{
	int i;
	for (i = 0; i < client_size; i++) {
		int *fdp = &client[i].fd;
		if (*fdp == fd) {
			*fdp = -1;
			log_msg("remove client %d", fd);
			return;
		}
	}
	log_quit("***can't find client entry for fd %d", fd);
}
void handle_request(char *buf, int nread, int clifd, uid_t uid)
{
	time_t tm;
	char *ts;
	time(&tm);
	ts = ctime(&tm);
	buf[nread] = 0;
	log_msg("[%d %d]%d:%s", uid, clifd, nread, buf);
	strcat(buf, ts);
	write(clifd, buf, strlen(buf));//FIXME:error handle
}

char buf[MAXLINE];
int main(int argc, char *argv[])
{
	int i, n, maxfd, maxi, listenfd, clifd, nread;
	uid_t uid;
	fd_set rset, allset;
	FD_ZERO(&allset);
	if ((listenfd = serv_listen(CS_OPEN)) < 0)
		log_sys("serv_listen error");
	FD_SET(listenfd, &allset);
	maxfd = listenfd;
	maxi = -1;

	for (;;) {
		rset = allset;
		n = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (n == -1)//or < 0
			log_sys("select error");
		else
			log_msg("Date available %d", n);//0 is nearly impossible so the >0 and ==0 are combined!
		if (FD_ISSET(listenfd, &rset)) {
			if ((clifd = serv_accept(listenfd, &uid)) < 0)
				log_sys("serv_accept error: %d", clifd);
			i = client_add(clifd, uid);
			FD_SET(clifd, &allset);
			if (clifd > maxfd)
				maxfd = clifd;
			if (i > maxi)
				maxi = i;
			log_msg("new connection: uid %d, fd %d", uid, clifd, maxi);
			continue;
		}
		for (i = 0; i <= maxi; i++) {
			if ((clifd = client[i].fd) < 0)
				continue;
			if (FD_ISSET(clifd, &rset)) {
				nread = read(clifd, buf, sizeof(buf));
				//printf("[%d]:%d\n", clifd, nread);
				if (nread < 0)
					log_sys("read error on fd %d", clifd);
				else if (nread == 0) {//client shutdown!?
					log_msg("closed: uid %d, fd %d", client[i].uid, client[i].fd);
					client_del(clifd);
					FD_CLR(clifd, &allset);
					close(clifd);
				} else
					handle_request(buf, nread, clifd, client[i].uid);
			}
		}
		
	}
	
		
	return 0;
}