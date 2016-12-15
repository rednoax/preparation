#define CONFIG_DEBUG_APP
#ifdef CONFIG_DEBUG_APP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define offsetof(type, member) ((size_t)&((type*)0)->member)
#define container_of(ptr, type, member) ({\
		const typeof(((type*)0)->member) *_mptr = (ptr);\
		(type*)((char*)_mptr - offsetof(type, member));})
#define __initdata
#define __init
#define __user
#define PATH_MAX 4096
#define kmalloc(size, flags) malloc(size)
#define kfree(p) free(p)
#define kstrdup(s, flags) strdup(s)
#define __setup(s, func)
#define KERN_INFO "<6>"
#define printk(...) printf(__VA_ARGS__)
typedef unsigned int u32;
//attributes should be specified before the declarator in a function definition
void panic(const char *fmt, ...) __attribute__((format (printf, 1, 2)));
void panic(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	for(;;)
		;
}

struct list_head {
	struct list_head *next, *prev;
};
#define LIST_HEAD_INIT(name) {&(name), &(name)}
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)
#define list_entry(ptr, type, member) container_of(ptr, type, member)
void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

static inline void __list_del(struct list_head *prev, struct list_head  *next)
{
	prev->next = next;
	next->prev = prev;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->prev = LIST_POISON1;
	entry->next = LIST_POISON2;
}

#define list_for_each_entry_safe(pos, n, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos), member);\
		n = list_entry(pos->member.next, typeof(*pos), member), (head) != &pos->member;\
		pos = n)

#define debug(fmt, arg...) printf(fmt, ##arg)
#define simple_strtoul(nptr, endptr, base) strtoul(nptr, endptr, base)
#if 1//def __KERNEL__
#define MINORBITS	20
#define MINORMASK	((1U << MINORBITS) - 1)

#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))
static inline u32 new_encode_dev(dev_t dev)
{
	unsigned major = MAJOR(dev);
	unsigned minor = MINOR(dev);
	return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
}
#else
#define MAJOR(dev)	((dev)>>8)
#define MINOR(dev)	((dev) & 0xff)
#define MKDEV(ma,mi)	((ma)<<8 | (mi))
#endif
int __sys_newlstat(const char *pathname, struct stat *st);
int __sys_rmdir(const char *pathname);
int __sys_unlink(const char *pathname);
int __sys_mkdir(const char *pathname, mode_t mode);
int __sys_chown(const char *pathname, uid_t owner, gid_t group);
int __sys_chmod(const char *pathname, mode_t mode);
int __sys_open(const char *pathname, int flags, mode_t mode);
int __sys_fchown(int fd, uid_t owner, gid_t group);
int __sys_fchmod(int fd, mode_t mode);
int __sys_ftruncate(int fd, off_t length);
int __sys_link(const char *oldpath, const char *newpath);
ssize_t __sys_write(int fd, const void *buf, size_t count);
int __sys_close(int fd);
int __sys_mknod(const char *pathname, mode_t mode, dev_t dev);
int __sys_symlink(const char *old, const char *new);
int __sys_lchown(const char *path, uid_t owner, gid_t group);
#else
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/utime.h>
#endif
static __initdata char *message;
static void __init error(char *x)
{
	if (!message)
		message = x;
}

/* link hash */

#define N_ALIGN(len) ((((len) + 1) & ~3) + 2)

static __initdata struct hash {
	int ino, minor, major;
	mode_t mode;
	struct hash *next;
	char name[N_ALIGN(PATH_MAX)];
} *head[32];

static inline int hash(int major, int minor, int ino)
{
	unsigned long tmp = ino + minor + (major << 3);
	tmp += tmp >> 5;
	return tmp & 31;
}

static char __init *find_link(int major, int minor, int ino,
			      mode_t mode, char *name)
{
	struct hash **p, *q;
	for (p = head + hash(major, minor, ino); *p; p = &(*p)->next) {
		if ((*p)->ino != ino)
			continue;
		if ((*p)->minor != minor)
			continue;
		if ((*p)->major != major)
			continue;
		if (((*p)->mode ^ mode) & S_IFMT)
			continue;
		debug("found %s:ino %d, %d, %d, %o\n", (*p)->name, ino, major, minor, mode);
		return (*p)->name;
	}
	q = kmalloc(sizeof(struct hash), GFP_KERNEL);
	if (!q)
		panic("can't allocate link hash entry");
	q->major = major;
	q->minor = minor;
	q->ino = ino;
	q->mode = mode;
	strcpy(q->name, name);
	q->next = NULL;
	*p = q;
	debug("Add %s:ino %d, %d, %d, %o\n", name, ino, major, minor, mode);
	return NULL;
}

static void __init free_hash(void)
{
	struct hash **p, *q;
	for (p = head; p < head + 32; p++) {
		while (*p) {
			q = *p;
			*p = q->next;
			kfree(q);
		}
	}
}

static long __init do_utime(char __user *filename, time_t mtime)
{
	struct timespec t[2];

	t[0].tv_sec = mtime;
	t[0].tv_nsec = 0;
	t[1].tv_sec = mtime;
	t[1].tv_nsec = 0;

#ifdef CONFIG_DEBUG_APP
	{
		/*
		int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);	
		utimensat() update the timestamps of a file with nanosecond precision, while utime(2) and utimes(2) permit only
		second and microsecond precision, respectively, when setting timestamps.
		the new file timestamps are specified in the array times:
		times[0] specifies the new "last access time"(atime);
		times[1] specifies the new "last modification time"(mtime).
		Each of the elements of times specifies a time as the number of seconds and nanoseconds since the Epoch

       If pathname is relative and dirfd is the special value AT_FDCWD, then pathname is interpreted relative to the current work-
       ing directory of the calling process (like utimes(2)).

       If pathname is absolute, then dirfd is ignored.

       The flags field is a bit mask that may be 0, or include the following constant, defined in <fcntl.h>:

       AT_SYMLINK_NOFOLLOW
              If  pathname  specifies  a  symbolic  link, then update the timestamps of the link, rather than the file to which it
              refers.
		*/
		int ret = utimensat(AT_FDCWD, filename, t, AT_SYMLINK_NOFOLLOW);
		/*
		0:successful
		-1 and errno is set to indicate the error
		*/
		if (ret)
			debug("***");
		debug("utimensat:%s %ld\n", filename, mtime);
		return ret;
	}
#else
	return do_utimes(AT_FDCWD, filename, t, AT_SYMLINK_NOFOLLOW);
#endif
}

static __initdata LIST_HEAD(dir_list);
struct dir_entry {
	struct list_head list;
	char *name;
	time_t mtime;
};

static void __init dir_add(const char *name, time_t mtime)
{
	struct dir_entry *de = kmalloc(sizeof(struct dir_entry), GFP_KERNEL);
	if (!de)
		panic("can't allocate dir_entry buffer");
	INIT_LIST_HEAD(&de->list);
	de->name = kstrdup(name, GFP_KERNEL);
	de->mtime = mtime;
	list_add(&de->list, &dir_list);
}

static void __init dir_utime(void)
{
	struct dir_entry *de, *tmp;
	list_for_each_entry_safe(de, tmp, &dir_list, list) {
		list_del(&de->list);
		do_utime(de->name, de->mtime);
		kfree(de->name);
		kfree(de);
	}
}

static __initdata time_t mtime;

/* cpio header parsing */

static __initdata unsigned long ino, major, minor, nlink;
static __initdata mode_t mode;
static __initdata unsigned long body_len, name_len;
static __initdata uid_t uid;
static __initdata gid_t gid;
static __initdata unsigned rdev;

static void __init parse_header(char *s)
{
	unsigned long parsed[12];
	char buf[9] = {0};
	int i;
	//
	memcpy(buf, s, 6);
	debug("header magic:%s\n", buf);
	//
	//buf[8] = '\0';
	for (i = 0, s += 6; i < 12; i++, s += 8) {
		memcpy(buf, s, 8);
		parsed[i] = simple_strtoul(buf, NULL, 16);
	}
	ino = parsed[0];
	mode = parsed[1];
	uid = parsed[2];
	gid = parsed[3];
	nlink = parsed[4];
	mtime = parsed[5];
	body_len = parsed[6];//dir's body_len is 0, reg file's body_len is its file size via ls
	major = parsed[7];
	minor = parsed[8];
	rdev = new_encode_dev(MKDEV(parsed[9], parsed[10]));
	name_len = parsed[11];//AL strlen+1
	debug("ino %ld, mode %o, uid %d, gid %d, nlink %ld, mtime %ld, "
		"body_len %ld, major %ld, minor %ld, rdev %d, name len %ld\n", \
		ino, mode, uid, gid, nlink, mtime, \
		body_len, major, minor, rdev, name_len);
}

/* FSM */

static __initdata enum state {
	Start,
	Collect,
	GotHeader,
	SkipIt,
	GotName,
	CopyFile,
	GotSymlink,
	Reset
} state, next_state;

static __initdata char *victim;
static __initdata unsigned count;
static __initdata loff_t this_header, next_header;

static __initdata int dry_run;

static inline void __init eat(unsigned n)
{
	victim += n;
	this_header += n;
	count -= n;
}

static __initdata char *vcollected;
static __initdata char *collected;
static __initdata int remains;
static __initdata char *collect;

static void __init read_into(char *buf, unsigned size, enum state next)
{
	if (count >= size) {
		collected = victim;
		eat(size);
		state = next;
	} else {
		collect = collected = buf;
		remains = size;
		next_state = next;
		state = Collect;
	}
}

static __initdata char *header_buf, *symlink_buf, *name_buf;

static int __init do_start(void)
{
	debug("%s:110B header\n", __func__);
	read_into(header_buf, 110, GotHeader);
	return 0;
}

static int __init do_collect(void)
{
	unsigned n = remains;
	if (count < n)
		n = count;
	memcpy(collect, victim, n);
	eat(n);
	collect += n;
	if ((remains -= n) != 0)
		return 1;
	state = next_state;
	return 0;
}

static int __init do_header(void)
{
	if (memcmp(collected, "070707", 6)==0) {
		error("incorrect cpio method used: use -H newc option");
		return 1;
	}
	if (memcmp(collected, "070701", 6)) {
		error("no cpio magic");
		return 1;
	}
	parse_header(collected);
	next_header = this_header + N_ALIGN(name_len) + body_len;
	printf("%lld+%ld(%ld)+%ld=%lld(0x%llx)=>", this_header, N_ALIGN(name_len), name_len, body_len, next_header, next_header);
	next_header = (next_header + 3) & ~3;
	printf("%lld(0x%llx)\n", next_header, next_header);
	if (dry_run) {
		read_into(name_buf, N_ALIGN(name_len), GotName);
		return 0;
	}
	state = SkipIt;
	if (name_len <= 0 || name_len > PATH_MAX)
		return 0;
	if (S_ISLNK(mode)) {
		if (body_len > PATH_MAX)
			return 0;
		collect = collected = symlink_buf;
		remains = N_ALIGN(name_len) + body_len;
		next_state = GotSymlink;
		state = Collect;
		return 0;
	}
	if (S_ISREG(mode) || !body_len)
		read_into(name_buf, N_ALIGN(name_len), GotName);
	return 0;
}

static int __init do_skip(void)
{
	if (this_header + count < next_header) {
		eat(count);
		return 1;
	} else {
		eat(next_header - this_header);
		state = next_state;
		return 0;
	}
}

static int __init do_reset(void)
{
	while(count && *victim == '\0')
		eat(1);
	if (count && (this_header & 3))
		error("broken padding");
	return 1;
}

/*
ls -li before link(mtime is not chanegd of course):
539607 -rwxr-xr-x 1 root root 7320 Dec 13 18:45 a.out
ls -li after link:
539607 -rwxr-xr-x 2 root root 7320 Dec 13 18:45 a.out
539607 -rwxr-xr-x 2 root root 7320 Dec 13 18:45 b.out

0:	(nlink == 1) || (nlink >= 2 && "1st added")
-1:	nlink >= 2 && "not 1st adding(cache exists) and link fails"
1:	nlink >= 2 && "not 1st adding(cache exists) and link successes"
*/
static int __init maybe_link(void)
{
	if (nlink >= 2) {
		char *old = find_link(major, minor, ino, mode, collected);
		if (old)
			return (__sys_link(old, collected) < 0) ? -1 : 1;
	}
	return 0;
}
//
int __sys_newlstat(const char *pathname, struct stat *st)
{
	int ret = lstat(pathname, st);
	/*
	0:successful
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("lstat %s:%o\n", pathname, st->st_mode);
	return ret;
}

int __sys_rmdir(const char *pathname)
{
	int ret = rmdir(pathname);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("rmdir %s\n", pathname);
	return ret;
}

int __sys_unlink(const char *pathname)
{
	int ret = unlink(pathname);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("unlink %s\n", pathname);
	return ret;
}

int __sys_mkdir(const char *pathname, mode_t mode)
{
	int ret = mkdir(pathname, mode);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("mkdir %s:%o\n", pathname, mode);
	return ret;
}

int __sys_chown(const char *pathname, uid_t owner, gid_t group)
{
	int ret = chown(pathname, owner, group);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("chown %s:%d %d\n", pathname, owner, group);
	return ret;
}

int __sys_chmod(const char *pathname, mode_t mode)
{
	int ret = chmod(pathname, mode);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("chmod %s:%o\n", pathname, mode);
	return ret;
}

int __sys_open(const char *pathname, int flags, mode_t mode)
{
	int ret = open(pathname, flags, mode);
	/*
	>=0:success
	-1 and errno is set to indicate the error
	*/
	if (ret < 0)
		debug("***");
	debug("open %s:%x %o\n", pathname, flags, mode);
	return ret;
}

int __sys_fchown(int fd, uid_t owner, gid_t group)
{
	int ret = fchown(fd, owner, group);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("fchown %d:%d %d\n", fd, owner, group);
	return ret;
}

int __sys_fchmod(int fd, mode_t mode)
{
	int ret = fchmod(fd, mode);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("fchmod %d:%o\n", fd, mode);
	return ret;
}

int __sys_ftruncate(int fd, off_t length)
{
	int ret = ftruncate(fd, length);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("ftruncate %d:%ld\n", fd, length);
	return ret;
}

int __sys_link(const char *oldpath, const char *newpath)
{
	int ret = link(oldpath, newpath);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("link %s:%s\n", oldpath, newpath);
	return ret;
}

ssize_t __sys_write(int fd, const void *buf, size_t count)
{
	int ret = write(fd, buf, count);
	//On error, -1 is returned, and errno is set appropriately.
	if (ret == -1 || ret != count)
		debug("***");
	debug("write %d:%dB\n", fd, count);
	return ret;
}

int __sys_close(int fd)
{
	int ret = close(fd);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("close %d\n", fd);
	return ret;
}

int __sys_mknod(const char *pathname, mode_t mode, dev_t dev)
{
	int ret = mknod(pathname, mode, dev);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("mknod %s %o %d %d\n", pathname, mode, MAJOR(dev), MINOR(dev));
	return ret;
}

int __sys_symlink(const char *old, const char *new)
{
	int ret = symlink(old, new);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("symlink %s %s\n", old, new);
	return ret;
}
/*
The lchown() function shall be equivalent to chown(), except in the case where the named file is a symbolic link.  In  this
case, lchown() shall change the ownership of the symbolic link file itself, while chown() changes the ownership of the file
or directory to which the symbolic link refers.
*/
int __sys_lchown(const char *path, uid_t owner, gid_t group)
{
	int ret = lchown(path, owner, group);
	/*
	0:success
	-1 and errno is set to indicate the error
	*/
	if (ret)
		debug("***");
	debug("lchown %s:%d %d\n", path, owner, group);
	return ret;
}

//chroot should be used before test!!!
static void __init clean_path(char *path, mode_t mode)
{
	struct stat st;
	//even the @path of reg file exists and its mode is the same as @mode, it will be later truncated???
	if (!__sys_newlstat(path, &st) && (st.st_mode^mode) & S_IFMT) {
		if (S_ISDIR(st.st_mode))
			__sys_rmdir(path);
		else
			__sys_unlink(path);
	}
}

static __initdata int wfd;

static int __init do_name(void)
{
	state = SkipIt;
	next_state = Reset;
	debug("###\"%s\"\n", collected);
	if (strcmp(collected, "TRAILER!!!") == 0) {
		free_hash();
		return 0;
	}
	if (dry_run)
		return 0;
	clean_path(collected, mode);
	if (S_ISREG(mode)) {
		int ml = maybe_link();
		if (ml >= 0) {
			int openflags = O_WRONLY|O_CREAT;
			if (ml != 1)
				openflags |= O_TRUNC;
			wfd = __sys_open(collected, openflags, mode);

			if (wfd >= 0) {
				__sys_fchown(wfd, uid, gid);
				__sys_fchmod(wfd, mode);
				__sys_ftruncate(wfd, body_len);
				vcollected = kstrdup(collected, GFP_KERNEL);
				state = CopyFile;
			}
		}
	} else if (S_ISDIR(mode)) {
		__sys_mkdir(collected, mode);
		__sys_chown(collected, uid, gid);
		__sys_chmod(collected, mode);
		dir_add(collected, mtime);
	} else if (S_ISBLK(mode) || S_ISCHR(mode) ||
		   S_ISFIFO(mode) || S_ISSOCK(mode)) {
		if (maybe_link() == 0) {
			__sys_mknod(collected, mode, rdev);
			__sys_chown(collected, uid, gid);
			__sys_chmod(collected, mode);
			do_utime(collected, mtime);
		}
	}
	return 0;
}

static int __init do_copy(void)
{
	if (count >= body_len) {
		__sys_write(wfd, victim, body_len);
		__sys_close(wfd);
		do_utime(vcollected, mtime);
		kfree(vcollected);
		eat(body_len);
		state = SkipIt;
		return 0;
	} else {
		__sys_write(wfd, victim, count);
		body_len -= count;
		eat(count);
		return 1;
	}
}

/*
00000160  30 37 30 37 30 31 30 30  30 38 33 42 39 43 30 30  |07070100083B9C00|
00000170  30 30 41 31 46 46 30 30  30 30 30 30 30 30 30 30  |00A1FF0000000000|
00000180  30 30 30 30 30 30 30 30  30 30 30 30 30 31 35 38  |0000000000000158|
00000190  35 32 35 39 41 45 30 30  30 30 30 30 30 35 30 30  |5259AE0000000500|
000001a0  30 30 30 30 30 38 30 30  30 30 30 30 30 31 30 30  |0000080000000100|
000001b0  30 30 30 30 30 30 30 30  30 30 30 30 30 30 30 30  |0000000000000000|
000001c0  30 30 30 30 30 35 30 30  30 30 30 30 30 30 73 6f  |00000500000000so|
000001d0  66 74 00 00 68 65 6c 6c  6f 00 00 00 30 37 30 37  |ft..hello...0707|
this_header:+110
name_len:5
N_ALIGN(5):6
body_len:5
this_header+N_ALIGN(name_len)+body_len, then aligned:
110+6(5)+5=121(0x79)=>124(0x7c)
next_header:+124(0x7c)
"collected" use "symlink_buf" to hold memcpy 11B:soft\0\0hello
the2nd '\0' above is N_ALIGN's extra 1B
*/
static int __init do_symlink(void)
{
	collected[N_ALIGN(name_len) + body_len] = '\0';
	clean_path(collected, 0);
	__sys_symlink(collected + N_ALIGN(name_len), collected);
	__sys_lchown(collected, uid, gid);
	do_utime(collected, mtime);
	state = SkipIt;
	next_state = Reset;
	return 0;
}

static __initdata int (*actions[])(void) = {
	[Start]		= do_start,
	[Collect]	= do_collect,
	[GotHeader]	= do_header,
	[SkipIt]	= do_skip,
	[GotName]	= do_name,
	[CopyFile]	= do_copy,
	[GotSymlink]	= do_symlink,
	[Reset]		= do_reset,
};

static int __init write_buffer(char *buf, unsigned len)
{
	count = len;
	victim = buf;

	while (!actions[state]())
		;
	return len - count;
}

static void __init flush_buffer(void *_buf, unsigned len)
{
	char *buf = _buf;
	int written;
	if (message)
		return;
	while ((written = write_buffer(buf, len)) < len && !message) {
		char c = buf[written];
		if (c == '0') {
			buf += written;
			len -= written;
			state = Start;
		} else if (c == 0) {
			buf += written;
			len -= written;
			state = Reset;
		} else
			error("junk in compressed archive");
	}
}

/*
 * gzip declarations
 */

#define OF(args)  args

#ifndef memzero
#define memzero(s, n)     memset ((s), 0, (n))
#endif

typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;

#define WSIZE 0x8000    /* window size--must be a power of two, and */
			/*  at least 32K for zip's deflate method */

static uch *inbuf;
static uch *window;

static unsigned insize;  /* valid bytes in inbuf */
static unsigned inptr;   /* index of next byte to be processed in inbuf */
static unsigned outcnt;  /* bytes in output buffer */
static long bytes_out;

#define get_byte()  (inptr < insize ? inbuf[inptr++] : -1)
		
/* Diagnostic functions (stubbed out) */
#define Assert(cond,msg)
#define Trace(x)
#define Tracev(x)
#define Tracevv(x)
#define Tracec(c,x)
#define Tracecv(c,x)

#define STATIC static
#define INIT __init

static void __init flush_window(void);
static void __init error(char *m);

#define NO_INFLATE_MALLOC
#ifdef CONFIG_DEBUG_APP
#include "./inflate.c"
#else
#include "../lib/inflate.c"
#endif
/* ===========================================================================
 * Write the output window window[0..outcnt-1] and update crc and bytes_out.
 * (Used for the decompressed data only.)
 */
static void __init flush_window(void)
{
	ulg c = crc;         /* temporary variable */
	unsigned n;
	uch *in, ch;

	flush_buffer(window, outcnt);
	in = window;
	for (n = 0; n < outcnt; n++) {
		ch = *in++;
		c = crc_32_tab[((int)c ^ ch) & 0xff] ^ (c >> 8);
	}
	crc = c;
	bytes_out += (ulg)outcnt;
	outcnt = 0;
}

static char * __init unpack_to_rootfs(char *buf, unsigned len, int check_only)
{
	int written;
	dry_run = check_only;
	header_buf = kmalloc(110, GFP_KERNEL);
	symlink_buf = kmalloc(PATH_MAX + N_ALIGN(PATH_MAX) + 1, GFP_KERNEL);
	name_buf = kmalloc(N_ALIGN(PATH_MAX), GFP_KERNEL);
	window = kmalloc(WSIZE, GFP_KERNEL);
	if (!window || !header_buf || !symlink_buf || !name_buf)
		panic("can't allocate buffers");
	state = Start;
	this_header = 0;
	message = NULL;
	while (!message && len) {
		loff_t saved_offset = this_header;
		if (*buf == '0' && !(this_header & 3)) {
			state = Start;
			written = write_buffer(buf, len);
			buf += written;
			len -= written;
			continue;
		}
		if (!*buf) {
			buf++;
			len--;
			this_header++;
			continue;
		}
		this_header = 0;
		insize = len;
		inbuf = (uch*)buf;
		inptr = 0;
		outcnt = 0;		/* bytes in output buffer */
		bytes_out = 0;
		crc = (ulg)0xffffffffL; /* shift register contents */
		makecrc();
		gunzip();
		if (state != Reset)
			error("junk in gzipped archive");
		this_header = saved_offset + inptr;
		buf += inptr;
		len -= inptr;
	}
	/*
[root@localhost apue]# ls -l
total 4
drwxr-xr-x 5 root root 4096 Dec 14 23:57 initramfs
[root@localhost apue]# ls -l --time=atime
total 4
drwxr-xr-x 5 root root 4096 Dec 14 23:58 initramfs
[root@localhost apue]# mkdir initramfs/sub
[root@localhost apue]# ls -l
total 4
drwxr-xr-x 6 root root 4096 Dec 15 00:43 initramfs
[root@localhost apue]# ls -l --time=atime
total 4
drwxr-xr-x 6 root root 4096 Dec 14 23:58 initramfs
	*/
	dir_utime();//mkdir a/ under one exist dir b/ will change mtime of b/
	kfree(window);
	kfree(name_buf);
	kfree(symlink_buf);
	kfree(header_buf);
	return message;
}

static int __initdata do_retain_initrd;
#ifndef CONFIG_DEBUG_APP
static
#endif
int __init retain_initrd_param(char *str)
{
	if (*str)
		return 0;
	do_retain_initrd = 1;
	return 1;
}
__setup("retain_initrd", retain_initrd_param);

#ifdef CONFIG_DEBUG_APP
extern char *__initramfs_start, *__initramfs_end;
extern unsigned long initrd_start, initrd_end;
void free_initrd_mem(unsigned long start, unsigned long end)
{
	if (start)
		free((char*)start);
}
#else
extern char __initramfs_start[], __initramfs_end[];
#include <linux/initrd.h>
#include <linux/kexec.h>
#endif

static void __init free_initrd(void)
{
#ifdef CONFIG_KEXEC
	unsigned long crashk_start = (unsigned long)__va(crashk_res.start);
	unsigned long crashk_end   = (unsigned long)__va(crashk_res.end);
#endif
	if (do_retain_initrd)
		goto skip;

#ifdef CONFIG_KEXEC
	/*
	 * If the initrd region is overlapped with crashkernel reserved region,
	 * free only memory that is not part of crashkernel region.
	 */
	if (initrd_start < crashk_end && initrd_end > crashk_start) {
		/*
		 * Initialize initrd memory region since the kexec boot does
		 * not do.
		 */
		memset((void *)initrd_start, 0, initrd_end - initrd_start);
		if (initrd_start < crashk_start)
			free_initrd_mem(initrd_start, crashk_start);
		if (initrd_end > crashk_end)
			free_initrd_mem(crashk_end, initrd_end);
	} else
#endif
		free_initrd_mem(initrd_start, initrd_end);
skip:
	initrd_start = 0;
	initrd_end = 0;
}

static int __init populate_rootfs(void)
{
	char *err = unpack_to_rootfs(__initramfs_start,
			 __initramfs_end - __initramfs_start, 0);
	if (err)
		panic(err);
	if (initrd_start) {
#ifdef CONFIG_BLK_DEV_RAM
		int fd;
		printk(KERN_INFO "checking if image is initramfs...");
		err = unpack_to_rootfs((char *)initrd_start,
			initrd_end - initrd_start, 1);
		if (!err) {
			printk(" it is\n");
			unpack_to_rootfs((char *)initrd_start,
				initrd_end - initrd_start, 0);
			free_initrd();
			return 0;
		}
		printk("it isn't (%s); looks like an initrd\n", err);
		fd = __sys_open("/initrd.image", O_WRONLY|O_CREAT, 0700);
		if (fd >= 0) {
			__sys_write(fd, (char *)initrd_start,
					initrd_end - initrd_start);
			__sys_close(fd);
			free_initrd();
		}
#else
		printk(KERN_INFO "Unpacking initramfs...");
		err = unpack_to_rootfs((char *)initrd_start,
			initrd_end - initrd_start, 0);
		if (err)
			panic(err);
		printk(" done\n");
		free_initrd();
#endif
	}
	return 0;
}

#ifdef CONFIG_DEBUG_APP
char *__initramfs_start, *__initramfs_end;
unsigned long initrd_start, initrd_end;

int load_file(const char *filename, unsigned long * const start, unsigned long * const end)
{
	int fd, size, ret = -1;
	struct stat st;
	char *p;
	fd = open(filename, O_RDONLY);
	if (fd <0) {
		perror("open file error:");
		goto EXIT3;
	}
	ret = fstat(fd, &st);
	if (ret < 0) {
		perror("fstat error:");
		goto EXIT2;
	}
	size = st.st_size;
	printf("%s size %d\n", filename, size);
	p = malloc(size);
	if (!p) {
		ret = -1;
		perror("malloc error:");
		goto EXIT2;
	}
	ret = read(fd, p, size);
	if (ret != size) {
		printf("read underflow: %d != %d\n", ret, size);
		goto EXIT1;
	}
	ret = 0;
	*start = (unsigned long)p;
	*end = *start + size;
	printf("read %s:%lx-%lx\n", filename, *start, *end);
	goto EXIT2;
EXIT1:
	free(p);
EXIT2:
	close(fd);
EXIT3:
	return ret;
}

#define N_ALIGN(len) ((((len) + 1) & ~3) + 2)
int show_next_header(int argc, char **argv)
{
	int i;
	unsigned int nr[3], next;
	if (argc < 4)
		printf("usage:./main this_header name_len body_len\n");
	for (i = 1; i < 4; i++) {
		nr[i - 1] = atoi(argv[i]);
	}
	next = nr[0] + N_ALIGN(nr[1]) + nr[2];
	printf("%d+%d(%d)+%d=%d(0x%x)=>", nr[0], N_ALIGN(nr[1]), nr[1], nr[2], next, next);
	next = (next + 3) & ~3;
	printf("%d(0x%x)\n", next, next);
	return 0;
}

int show_N_ALIGN(int argc, char **argv)
{
	int i;
	if (argc < 2)
		printf("usage:./main number\n");
	for (i = 1; i < argc; i++) {
		unsigned int l = atoi(argv[i]);
		printf("%d(0x%x)\t->%d(0x%x)\n", l, l, N_ALIGN(l), N_ALIGN(l));
	}
	return 0;
}

int main(int argc, char **argv, char **envp)
{
	int ret;
	if (argc < 2) {//must >= 2
		printf("usage:./initramfs cpio|cpio.gz [cpio|cpio.gz]\n");
		return 0;
	}
	if (argc >= 3)
		load_file(argv[2], &initrd_start, &initrd_end);
	ret = load_file(argv[1], (unsigned long*)&__initramfs_start, (unsigned long*)&__initramfs_end);
	if (ret)
		return ret;
	populate_rootfs();

	if (__initramfs_start)
		free(__initramfs_start);
	return 0;
}
#else
rootfs_initcall(populate_rootfs);
#endif
