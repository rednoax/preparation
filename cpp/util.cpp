#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>//man 7 unix
//#define _GNU_SOURCE
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <poll.h>
#define ANDROID_SOCK_DIR "."///dev"//socket/"
#define PROP_SERVICE_NAME "property_service"
/*
socket <name> <type> <perm> [ <user> [ <group> [ <seclabel> ] ] ]
  Create a unix domain socket named /dev/socket/<name> and pass
  its fd to the launched process.  <type> must be "dgram", "stream" or "seqpacket".
  User and group default to 0.
  'seclabel' is the SELinux security context for the socket.
  It defaults to the service security context, as specified by seclabel or
  computed based on the service executable file security context.

system/core/logd/logd.rc:
socket logdr seqpacket 
*/
/*
mode_t: unsigned short, 0777==0x1ff
uid_t/gid_t: uint
*/
int create_socket(const char *name, int type, mode_t perm, uid_t uid, gid_t gid, const char *socketcon)
{
	struct sockaddr_un addr;
	int fd, ret;

	fd = socket(AF_UNIX, type, 0);
	if (fd < 0) {
		printf("Failed to open socket '%s': %s", name, strerror(errno));
		return -1;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, sizeof(addr.sun_path), ANDROID_SOCK_DIR"/%s", name);
	ret = unlink(addr.sun_path);
	printf("unlink:%d, errno %d\n", ret, errno);
	if (ret != 0 && errno != ENOENT) {//ENOENT==2
		printf("Failed to unlink old socket '%s': %s\n", name, strerror(errno));
		goto out_close;
	}
	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr))) { //strlen(addr.sun_path) + 1 + offsetof(struct sockaddr_un, sun_path)))
		printf("Failed to bind socket '%s': %s\n", name, strerror(errno));
		goto out_unlink;
	}
	/*
	chown will go to the end while lchown will change symbolic link itself
   * chown() changes the ownership of the file specified by pathname, which is dereferenced if it is a symbolic link.

   * fchown() changes the ownership of the file referred to by the open file descriptor fd.

   * lchown() is like chown(), but does not dereference symbolic links.
	*/
	if (lchown(addr.sun_path, uid, gid)) {//On error, -1 is returned
		printf("Failed to lchown socket '%s': %s\n", addr.sun_path, strerror(errno));
		goto out_unlink;
	}
	/*
	int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags);
       The fchmodat() system call operates in exactly the same way as chmod(), except for the differences described here.

       If  the  pathname  given  in  pathname  is  relative, then it is interpreted relative to the directory referred to by the file descriptor dirfd
       (rather than relative to the current working directory of the calling process, as is done by chmod() for a relative pathname).

       If pathname is relative and dirfd is the special value AT_FDCWD, then pathname is interpreted relative to the current working directory of  the
       calling process (like chmod()).

       If pathname is absolute, then dirfd is ignored.

       flags can either be 0, or include the following flag:

       AT_SYMLINK_NOFOLLOW
              If pathname is a symbolic link, do not dereference it: instead operate on the link itself.  This flag is not currently implemented.

	*/
	//addr.sun_path is absolute so AT_FWCWD is ignored. if /dev/socket/name is symlink, change the symlink's mode.
#if 0
	//Failed to fchmodat socket './property_service': 95, Operation not supported
	if (fchmodat(AT_FDCWD, addr.sun_path, perm, AT_SYMLINK_NOFOLLOW)) {//on error, -1 is returned
#else
	if (chmod(addr.sun_path, perm)) {
#endif
		printf("Failed to fchmodat socket '%s': %d, %s\n", addr.sun_path, errno, strerror(errno));
		goto out_unlink;
	}
	printf("create socket '%s'(0x%x) with mode '%o', user '%d', group '%d'\n", addr.sun_path, type, perm, uid, gid);
	return fd;
out_unlink:
	unlink(addr.sun_path);
out_close:
	close(fd);
	return -1;
}

static int epoll_fd = -1;
static int property_set_fd = -1;
#define PROP_NAME_MAX 32
#define PROP_VALUE_MAX 92
struct prop_msg {
    unsigned cmd;
    char name[PROP_NAME_MAX];
    char value[PROP_VALUE_MAX];
};
/*
the client can be set non block or block, which has no relation to whether the server socket is blockable
when server socket is non block, the new accepted client socket is block by default
*/
static int handle_property_set_fd(int cli_block)
{
	prop_msg msg;
	int s;
	int r, ret = 0;
	struct ucred cr;
	struct sockaddr_un addr;
	socklen_t addr_size = sizeof(addr);
	socklen_t cr_size = sizeof(cr);
	struct pollfd ufds[1];
	const int timeout_ms = 2 * 1000;// 2sec
	int nr;
	const char str[] = "bye";
	/*
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	The addrlen argument is a value-result argument: the caller must initialize it to contain the size (in bytes) of the structure pointed to by
	addr; on return it will contain the actual size of the peer address.

	If no pending connections are present on the queue, and the socket is not marked as nonblocking, accept() blocks the caller until a connection
	is  present. If the socket is marked nonblocking and no pending connections are present on the queue, accept() fails with the error EAGAIN or
	EWOULDBLOCK.
	RETURN:
	on success a nonnegative integer is returned, which is a descriptor for the accept socket.
	on error -1 is returned and errno is set appropriately
	*/
	printf("server start accept, client %s\n", cli_block? "Block": "NONBlock");
	s = accept(property_set_fd, (struct sockaddr*)&addr, &addr_size);
	if (s < 0) {
		printf("accept %d failed: %d, %s\n", property_set_fd, errno, strerror(errno));
		ret = -1;
		goto RETURN;
	}
	printf("new client [%d]: addrlen %d\n", s, addr_size);//sizeof(.sun_family)==2
	/*
    int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
	Options may exist at multiple protocol levels; they are always present at the uppermost socket level.    
	When manipulating socket options, the level at which the option resides and the name of the option must be specified. To manipulate options at
	the sockets API level, level is specified as SOL_SOCKET. ...Optlen is a value-result argument, initially containing the size of the buffer pointed
	to by optval, and modified on return to indicate the actual size of the value returned.
	man 7 socket:
	SO_PEERCRED:
	Return  the credentials of the foreign process connected to this socket.  This is possible only for connected AF_UNIX stream sockets and
	AF_UNIX stream and datagram socket pairs created using socketpair(2); see unix(7).  The returned credentials  are  those  that  were  in
	effect  at  the time of the call to connect(2) or socketpair(2).  The argument is a ucred structure; define the _GNU_SOURCE feature test
	macro to obtain the definition of that structure from <sys/socket.h>.  This socket option is read-only.
    */
	if (getsockopt(s, SOL_SOCKET, SO_PEERCRED, &cr, &cr_size) < 0) {//on error, -1 is returned, and errno is set appropriately
		printf("Unable to receive socket option: %s", strerror(errno));
		ret = -1;
		goto CLOSE;
	}
	printf("getsockopt return cred %d B:peer %d, uid %d, gid %d\n", cr_size, cr.pid, cr.uid, cr.gid);
	ufds[0].fd = s;
	ufds[0].events = POLLIN;
	ufds[0].revents = 0;
	nr = poll(ufds, 1, timeout_ms);
	printf("poll %d: ", nr);
	if (nr == 0) {
		printf("timeout(%dms) waiting for pid=%d uid=%d to send property message\n", timeout_ms, cr.pid, cr.uid);
#if 0//go on to test recv with MSG_DONTWAIT
		close(s);
		return -1;
#endif
	} else if (nr == -1) {
		printf("error waiting for uid=%d to send property message: %d %s\n", cr.uid, errno, strerror(errno));
		ret = -1;
		goto CLOSE;
	}
	/*
	#include <sys/socket.h>

	ssize_t recv(int sockfd, void *buf, size_t len, int flags);

	ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
	                struct sockaddr *src_addr, socklen_t *addrlen);

	ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);

	All  three  calls  return  the  length of the message on successful completion.  If a message is too long to fit in the supplied buffer, excess
	bytes may be discarded depending on the type of socket the message is received from.
	If no messages are available at the socket, the receive calls wait for a message to arrive, unless the socket is nonblocking (see fcntl(2)), in
	which case the value -1 is returned and the external variable errno is set to EAGAIN or EWOULDBLOCK.  The receive calls normally return any
	data available, up to the requested amount, rather than waiting for receipt of the full amount requested.
	MSG_DONTWAIT:
		Enables nonblocking operation; if the operation would block, the call fails with the error EAGAIN or EWOULDBLOCK.  This provides similar
		behavior to setting the O_NONBLOCK flag (via the fcntl(2) F_SETFL operation), but differs in that MSG_DONTWAIT  is  a  per-call  option,
		whereas  O_NONBLOCK is a setting on the open file description (see open(2)), which will affect all threads in the calling process and as
		well as other processes that hold file descriptors referring to the same open file description.
	*/
	/*
	similat to read, the buf size can be large or small, where small means smaller than actual input amount and large means bigger than actual input
	amount. And it can return even the really got bytes < requested.
	*///EBADF(9) can be returned if you recv from a socket that has been closed
	r = recv(s, &msg, sizeof(msg), cli_block? 0: MSG_DONTWAIT);//both EAGAIN and EWOULDBLOCK 11
	if (r != sizeof(prop_msg)) {
		//l:long or ulong z:size_t or ssize_t
		printf("mis-match msg size received: %d expected: %zu: %d\n", r, sizeof(prop_msg), errno);
		ret = -1;
		if (r == -1 && !cli_block)
			goto SEND;
		goto CLOSE;
	} else
		printf("cmd %x:[%s][%s]\n", msg.cmd, msg.name, msg.value);
SEND:
	//send even recv return EAGAIN
	r = send(s, str, strlen(str) + 1, 0);
	printf("send to cli %d %dB(\"%s\") then close\n", cr.pid, r, str);
CLOSE:
	close(s);
RETURN:
	return ret;
}

void register_epoll_handler(int fd, int (*fn)(int))
{
	epoll_event ev;
	ev.events = EPOLLIN;//just for read
	ev.data.ptr = (void*)fn;
	/*
	The "event" argument describes the object linked to the file descriptor "fd".  The struct epoll_event is defined as:

	typedef union epoll_data {
	   void        *ptr;
	   int          fd;
	   uint32_t     u32;
	   uint64_t     u64;
	} epoll_data_t;

	struct epoll_event {
	   uint32_t     events;      // Epoll events
	   epoll_data_t data;        // User data variable
	};

	register "fd" on the epoll instance and associate the event "ev" with "fd"
	*/
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		printf("epoll_ctl failed:%s\n", strerror(errno));
		exit(1);
	}
}

void start_property_service(int flags)
{
	property_set_fd = create_socket(PROP_SERVICE_NAME, SOCK_STREAM | flags, //| SOCK_CLOEXEC, //| SOCK_NONBLOCK,
										0666, getuid(), getgid(), NULL);
	if (property_set_fd == -1) {
		printf("%s socket creation failed:%s\n", __func__, strerror(errno));
		exit(1);
	}
	listen(property_set_fd, 4);
	register_epoll_handler(property_set_fd, handle_property_set_fd);
}

int main(int argc, char *argv[])
{
	int flags = 0;//EPOLL_CLOEXEC
	epoll_event ev;
	int timeout = -1;//wait indefinitely
	int opt, count, ret;
	int test_mode = 0;
	int cli_block = 1;
	while((opt = getopt(argc, argv, "nt")) != -1){
		switch(opt) {
		case 'n':
			flags |= SOCK_NONBLOCK;
			break;
		case 't':
			test_mode = 1;
			break;
		//TODO:SOCK_CLOEXEC
		default:
			printf("%c is not implemented\n", opt);
			break;
		}
	}
	printf("socket type: 0x%x(%s)\n", flags, (flags & SOCK_NONBLOCK)? "NONBLOCK": "BLOCK");
	if (test_mode) {
		int cli_block = 0;
		property_set_fd = create_socket(PROP_SERVICE_NAME, SOCK_STREAM | flags, //| SOCK_CLOEXEC, //| SOCK_NONBLOCK,
											0666, getuid(), getgid(), NULL);
		if (property_set_fd == -1) {
			printf("%s socket creation failed:%s\n", __func__, strerror(errno));
			exit(1);
		}
		listen(property_set_fd, 8);
		for (count = 0, ret = -1; count < 12; count++) {
			ret = handle_property_set_fd(cli_block);
			cli_block = !cli_block;
			if (!ret)
				break;
			usleep(500*1000);//us
		}
		close(property_set_fd);
		return 0;
	}
	/*
	int epoll_create1(int flags);
	creates an epoll(7) instance and return a file descriptor referring to the new epoll instance. The file descriptor is used for all the
	subsequent calls to the epoll interface. When no longer required, the file descriptor returned by epoll_create1() should be closed by
	using close(2).   When  all  file descriptors referring to an epoll instance have been closed, the kernel destroys the instance and
	releases the associated	resources for reuse.
	flags can be 0 or EPOLL_CLOEXEC:Set the close-on-exec (FD_CLOEXEC) flag on the new file descriptor. See the description of the O_CLOEXEC
	flag in  open(2)  for  reasons why this may be useful.
	RETURN:
	on success a nonnegative file descriptor is returned.
	on error -1 is returned and errno is set appropriately.
	*/
	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
		printf("epoll_create1 failed: %s\n", strerror(errno));
		exit(1);
	}
	start_property_service(flags);
	for (;;) {
		/*
		int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
		The timeout argument specifies the number of milliseconds that epoll_wait() will block.  The call will block until either:
		*  a file descriptor delivers an event;
		*  the call is interrupted by a signal handler; or
		*  the timeout expires.
		*/
		int nr = epoll_wait(epoll_fd, &ev, 1, timeout);
		/*
		on error, -1 is returned and errno is set appropriately. If errno is EINTR: The call was interrupted by a signal handler before
		either (1) any of the requested events occurred or (2) the timeout expired; see signal(7).
		*/
		if (nr == -1) {
			printf("epoll_wait failed: %s\n", strerror(errno));
			//exit(1);
		} else if (nr == 1) {
			((int(*)(int))ev.data.ptr)(cli_block);
			cli_block = !cli_block;
		}
	}
	return 0;
}
