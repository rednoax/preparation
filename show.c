#include <stdio.h>
#include <unistd.h>
#include <linux/types.h>
#if 0
gcc -M show.c >show.d
gcc -M -Iunexist -I ~/opt/src/deb11.0.0/O_linux-source-5.10/usr/include/ show.c>show.d.2
-Idir:
1. there can be multi –I, either with a relative dir or an abs dir, if dir is relative, use CWD to get it.
2. the dir can be not existing at all, no error report
3. If there are >=2 files, which are of the same name and in different dir specified by –I, the 1st found dir is used.Take the build error
(see bpf note) as an eample: linux/types residents in both ./usr/include and tools/include, the former is used as –I./usr/include is before –I$(srctree)/tools/include
4. the –I search is before a system header file. eg linux/types.h exists in /usr/include/ and $(abs_objtree)/usr/include, `gcc -I$(abs_objtree)/usr/include`
 will make its own linux/types.h is used, rather than /usr/include/linux/types.h
#endif
int main()
{
	printf("pid %d, ppid %d\n", getpid(), getppid());
	return 0;
}
