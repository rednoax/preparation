#include <stdio.h>
#include <string.h>
/*
1. if no __attribute__((unused)), error will be emited:
used_and_unused.c:2:12: warning: 'print' defined but not used [-Wunused-function]
 static int print(const char* fmt, ...)
            ^
2. 
unused 	This attribute, attached to a function, means that the function is meant to be
	possibly unused. GCC will not produce a warning for this function.

the following will be ok:
__unused static int print(const char* fmt, ...)
static __unused int print(const char* fmt, ...)

the following will be not ok:
static  int print(const char* fmt, ...) __unused

used_and_unused.c:16:1: error: expected ',' or ';' before '{' token
 {
 ^
used_and_unused.c:15:12: warning: 'print' declared 'static' but never defined [-Wunused-function]
 static int print(const char* fmt, ...)__unused
            ^
*/

#define __unused __attribute__((unused))
#define __used __attribute__((used))
static __unused int print(const char* fmt)
{
	return printf(fmt);
}

/*
unused 	This attribute, attached to a variable, means that the variable is meant to be
	possibly unused. GCC will not produce a warning for this variable.
used 	This attribute, attached to a variable, means that the variable must be emitted
	even if it appears that the variable is not referenced.
*/
/*
if no __unused, error will be emitted:
used_and_unused.c:33:12: warning: 'g_var' defined but not used [-Wunused-variable]
 static int g_var;
            ^
*/
/*
after -Wall -Os, g_var1 cannot be found in symbol table
*/
__unused static int g_var1;

/*
if no __used, it will emit error:
used_and_unused.c:57:13: warning: 'g_var2' defined but not used [-Wunused-variable]
  static int g_var2;
             ^
*/
/*
if no __used, after -Wall -Os, g_var2 cannot be found in symbol table;
now it exists in symbol table because of __used
*/
__used static int g_var2;

int main()
{
	return 0;
}
