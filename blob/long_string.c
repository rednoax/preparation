#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>//uint32_t
#include "blob.h"
#include "blobmsg.h"
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


enum ubus_msg_attr {
	UBUS_ATTR_UNSPEC,

	UBUS_ATTR_STATUS,

	UBUS_ATTR_OBJPATH,//2
	UBUS_ATTR_OBJID,//3
	UBUS_ATTR_METHOD,//4

	UBUS_ATTR_OBJTYPE,
	UBUS_ATTR_SIGNATURE,//6

	UBUS_ATTR_DATA,//7
	UBUS_ATTR_TARGET,//8

	UBUS_ATTR_ACTIVE,
	UBUS_ATTR_NO_REPLY,//10

	UBUS_ATTR_SUBSCRIBERS,

	/* must be last */
	UBUS_ATTR_MAX,//12
};


char *count_to_number(uint32_t num)//return calloced & sprintf string
{
	uint32_t ptr = 0, size = 0;
	uint32_t written = 0, i;
	int new_line_every_n_numbers = 30;
	char *s;

	for (i = 0; i < num; ++i) {
		size += snprintf(NULL, 0, "%u ", i);
		if (i > 0 && i % new_line_every_n_numbers == 0)
			size++;
	}
	size++; /* one for null char */

	s = calloc(size, sizeof(char));
	if (!s)
		goto out;

	for (i = 0; i < num; ++i) {
		written = sprintf(&s[ptr], "%u ", i);
		ptr  += written;
		if (i > 0 && i % new_line_every_n_numbers == 0) {
			sprintf(&s[ptr], "\n");
			ptr++;
		}
	}
	printf("%s", s);
out:
	return s;
}
static char _buf[4096];//UBUS_MSG_CHUNK_SIZE];
#if 0
#define ulog(x, fmt, arg...) printf(fmt, ##arg)
#else
#define LOG_DEBUG 7
static const char *_ulog_ident = "C";

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
#define LOG_LINE_MAX (1024 - 32)//992
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
#if 0//this branch will return -1&errno==-EINVAL:[	13.460000] [1]procd: 0074b5e8 596(0x254): 1826, 22 -1:7:1:1
		last_ret = write(fd, kmsg, ret);
#else
/*
1.from drivers/char/mem.c and kernel/printk.c: if demand len > LOG_LINE_MAX, /dev/kmsg writing will return -EINVAL
2.when timestamp will show:there is '\n' no matter if there has been a '\n' before it;
*/
{
		int pos, len = 0;
		for (pos = 0; ret > 0; ) {
			len = write(fd, kmsg + pos, ret > LOG_LINE_MAX? LOG_LINE_MAX: ret);
			if (len < 0)
				break;
			pos += len;
			ret -= len;
		}
# if 0
		last_ret = ret;
		last_pos = pos;
		last_len = len;
# endif
}
#endif
	}
#endif
}

void ulog(int priority, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ulog_kmsg(priority, fmt, ap);
	va_end(ap);
}

#endif
void dump_mem(const void *_mem, int len)
{
	int i, ret = 0, j;
	const char *mem = _mem;
	if (len <= 0)
		return;
	if (len > 16)
		ret += snprintf(_buf + ret, sizeof(_buf) - ret, "\n");
	for (i = j = 0; i < len && j < 2; i++) {
		ret += snprintf(_buf + ret, sizeof(_buf) - ret, "%02x ", mem[i] & 0xff);
		if ((i + 1) % 16 == 0)
			ret += snprintf(_buf + ret, sizeof(_buf) - ret, "\n");
		if (ret > 2000 - 128) {
			ulog(LOG_DEBUG, _buf);
			ret = 0;
			j++;
			ulog(LOG_DEBUG, "\n");
		}
	}
	//
	if (i % 16 != 0)
		ret += snprintf(_buf + ret, sizeof(_buf) - ret, "\n");
	//
	ulog(LOG_DEBUG, _buf);
#if 0
{
	int info[8];
	ulog_check(info);
	ulog(LOG_DEBUG, "%08x %d(0x%x): %d, %d %d:%d:%d:%d\n", mem, len, len, ret, errno, info[0], info[1], info[2], info[3]);
}
#endif
}

int ubus_invoke_async(uint32_t obj, const char *method,
                       struct blob_attr *msg)
{
	struct blob_buf b = {0};
	blob_buf_init(&b, 0);
	blob_put_int32(&b, UBUS_ATTR_OBJID/*3*/, obj);
	blob_put_string(&b, UBUS_ATTR_METHOD/*4*/, method);
	if (msg)
		blob_put(&b, UBUS_ATTR_DATA/*7*/, blob_data(msg), blob_len(msg));
	dump_mem(b.head, blob_raw_len(b.head));
	return 0;
}

int main()
{
	uint32_t count_to = 100100;
	char *s = count_to_number(count_to);
	struct blob_buf b = {0};
	blob_buf_init(&b, 0);
	blobmsg_add_u32(&b, "to", count_to);
	blobmsg_add_string(&b, "string", s);
	ubus_invoke_async(0, "count", b.head);
	free(s);
	return 0;
}
