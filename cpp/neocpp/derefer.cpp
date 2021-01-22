#include <stdio.h>
#include <unistd.h>
#include <string.h>
void dump(const char *mem, int nr)
{
	int i, ret = 0;
	static char buf[512];
	for (i = 0; i < nr; i++) {
		ret += snprintf(buf + ret, sizeof(buf) - ret, "%02x ", 0xff & mem[i]);
		if ((i + 1) % 16 == 0)
			ret += snprintf(buf + ret, sizeof(buf) - ret, "\n");
	}
	write(STDOUT_FILENO, buf, ret);
}

int main(int argc, char **argv)
{
	char buf[8], buf2[16];
	int len, ret;
	//after stty -icrnl,[ENTER] cannot terminate input, use ^d
	while ((len = read(STDIN_FILENO, buf, sizeof(buf) - 1))) {
		buf[len] = 0;
#if 10
		ret = snprintf(buf2, sizeof buf2, "[%s]%d\n", buf, len);
		write(STDOUT_FILENO, buf2, ret);
#endif
		dump(buf, len);
	}
	printf("%s %d\n", __func__, len);
	return 0;
}