/*
$ gcc -Wall main.c -I include/<--I will not add sub dirs of include
main.c:1:10: fatal error: my.h: No such file or directory
    1 | #include <my.h>
      |          ^~~~~~
compilation terminated.
$ gcc -Wall main.c -I include/configs/<-right, no error emitted
*/
#include <my.h>//include/configs/my.h
int main()
{
	return 0;
}
