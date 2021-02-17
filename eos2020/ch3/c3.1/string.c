/*
$ arm-none-eabi-gcc -Wall -c t.c
In file included from t.c:4:0:
string.c:1:6: warning: conflicting types for built-in function 'strcpy' [enabled by default]
 void strcpy(char *dst, const char *src)
      ^
$ grep -rn strcpy ~/arm-2013.11/bin/
Binary file /home/rednoah/arm-2013.11/bin/arm-none-eabi-cs matches
Binary file /home/rednoah/arm-2013.11/bin/arm-none-linux-gnueabi-c++filt matches
......<--a long trail of matching

solution:
https://stackoverflow.com/questions/20582536/error-conflicting-types-for-built-in-function-tolower-werror
QUOTED:
tolower is a function from the C library and its identifier is a reserved identifier for use as an
identifier with external linkage, even if you don't include the header where it is declared.
You could get rid of the warning by using -fno-builtin but the best is simply to chose another name
for tolower.
*/
void strcpy(char *dst, const char *src)
{
	while ((*dst++ = *src++) != 0)
		;
}
