#include "blobmsg.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
static struct blob_buf b;

static void
fill_message(struct blob_buf *buf)
{
	void *tbl;

	blobmsg_add_string(buf, "message", "Hello, world!");//comment a
	printf("%p %p\n", b.buf, b.head);
	tbl = blobmsg_open_table(buf, "testdata");
	blobmsg_add_u32(buf, "hello", 1);
	blobmsg_add_string(buf, "world", "2");
	blobmsg_close_table(buf, tbl);
	printf("%p %p\n", b.buf, b.head);
	tbl = blobmsg_open_array(buf, "list");
	blobmsg_add_u32(buf, NULL, 0);
	blobmsg_add_u32(buf, NULL, 1);
	blobmsg_add_u32(buf, NULL, 2);
	blobmsg_close_table(buf, tbl);
}

void write_file(const char *buf, int len)
{
	int fd = open("blob.out", O_CREAT | O_RDWR | O_TRUNC, 0644);
	int ret = write(fd, buf, len);
	printf("write %d\n", ret);
	close(fd);
}

int main()
{
	blob_buf_init(&b, BLOBMSG_TYPE_TABLE);
	printf("%p %p\n", b.buf, b.head);
	fill_message(&b);
	write_file(b.buf, b.buflen);
	if (b.buf)
		free(b.buf);
	return 0;
}
