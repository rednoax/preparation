#include "blob.h"

static struct blob_buf b;

int main()
{
	blob_buf_init(&b, 0);
	return 0;
}
