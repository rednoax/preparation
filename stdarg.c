#include <stdarg.h>
#include <stdio.h>
//the following 2 is equivalent, __VA_ARGS__ tokens should be >=1, for __android_log_print demands fmt, so the __VA_ARGS__ here should >= 2
#define android_printlog(prio, tag, fmt...) \
	__android_log_print(prio, tag, fmt)

#define android_printlog2(prio, tag, ...) \
	__android_log_print(prio, tag, __VA_ARGS__)

#define android_printlog3(prio, tag, fmt, arg...) \
	__android_log_print(prio, tag, fmt, ##arg)
/*
stdarg.c:13:41: error: expected expression before ‘)’ token
  __android_log_print(prio, tag, fmt, arg)
                                         ^
stdarg.c:69:2: note: in expansion of macro ‘android_printlog4’
  android_printlog4(4, "Atfwd_Daemon", "***4\n");
*/
#define android_printlog4(prio, tag, fmt, arg...) \
	__android_log_print(prio, tag, fmt, arg)
int __android_log_print(int prio, const char *tag, const char *fmt, ...)
{
	va_list ap;
	char buf[1024];
	//char *bufp = buf;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	return printf(buf);
}

int msgprintf(char *fmt, ...)
{
	va_list ap;
	char *s;
	int d;
	char c;
	
	va_start(ap, fmt);
	printf("ap %p, &fmt %p\n", ap, &fmt);
	s = va_arg(ap, char*);
	printf("ap %p\n", ap);
	/*
	c = (char)va_arg(ap, char);
	
	when build:
stdarg.c: In function 'msgprintf':
stdarg.c:15:6: warning: 'char' is promoted to 'int' when passed through '...'
  c = (char)va_arg(ap, char);
      ^
stdarg.c:15:6: note: (so you should pass 'int' not 'char' to 'va_arg')
stdarg.c:15:6: note: if this code is reached, the program will abort
	when run:
[root@localhost preparation]# ./a.out
ap 0xbfd18174, &fmt 0xbfd18170
ap 0xbfd18178
Illegal instruction (core dumped)
	*/
	c = (char)va_arg(ap, int);
	printf("ap %p\n", ap);
	d = va_arg(ap, int);
	printf("ap %p\n", ap);
	printf("%s\n", s);
	printf("%c\n", c);
	printf("%d\n", d);
	va_end(ap);
	return 0;
}

int main(int argc, char **argv)
{
	msgprintf("%s:%c %d\n", __func__, 'X', __LINE__);
	android_printlog(4, "Atfwd_Daemon", "***%s:%d\n", __func__, __LINE__);
	android_printlog2(4, "Atfwd_Daemon", "***%s:%d\n", __func__, __LINE__);
	android_printlog3(4, "Atfwd_Daemon", "***3\n");
	//android_printlog4(4, "Atfwd_Daemon", "***4\n");//triggered error is list above
	return 0;
}
