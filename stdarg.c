#include <stdarg.h>
#include <stdio.h>

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
	return 0;
}
