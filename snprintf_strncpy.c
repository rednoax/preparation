#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
//2.6.29-rc3's vsnprint usage example
#define ARRAY_SIZE(x) sizeof(x)/sizeof((x)[0])
struct kobject_uevent_env {
	char *envp[32];
	int envp_idx;
	char buf[2048];
	int buflen;
};
/*
format (archetype, string-index, first-to-check)
cause the compiler to check the arguments in calls to "add_uevent_var" for consistency with the printf style
format string argument "fmt", the parameter string-index specifies which argument is the format string argument(starting from 1),
while first-to-check is the number of the first argument to check against the format string.
*/
int add_uevent_var(struct kobject_uevent_env *env, const char *format, ...) __attribute__((format (printf, 2, 3)));
int add_uevent_var(struct kobject_uevent_env *env, const char *format, ...)
{
	va_list args;
	int len;
	if (env->envp_idx >= ARRAY_SIZE(env->envp)) {
		printf("no free index\n");
		return -1;
	}
	va_start(args, format);
	len = vsnprintf(&env->buf[env->buflen], sizeof(env->buf) - env->buflen, format, args);
	va_end(args);
	if ((env->buflen + len + 1) > sizeof(env->buf)) {//kernel ver:len >= (sizeof(env->buf) - env->buflen)
		printf("no free mem\n");
		return -1;
	}
	env->envp[env->envp_idx++] = &env->buf[env->buflen];
	env->buflen += len + 1;
	//va_end(args);
	return 0;
}

int kobject_uevent_env(char *envp_ext[])
{
	struct kobject_uevent_env *env;
	int i = 0;
	int retval = 0;
	env = malloc(sizeof(struct kobject_uevent_env));
	if (!env)
		return -1;
	if (envp_ext) {
		for (i = 0; envp_ext[i]; i++) {
			retval = add_uevent_var(env, "%s", envp_ext[i]);
			if (retval)
				goto exit;
		}
		for (i = 0; env->envp[i]; i++)
			printf("%d:\"%s\"\n", i, env->envp[i]);
	}
exit:
	free(env);
	return retval;
}
//
//return value and behivor is similar to snprintf
size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);
	if (size) {
		size_t len = ret >= size ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}
/*
char *strcpy(char *dest, const char *src);
strcpy's FINAL result:if src is a null terminated string, then the dest will be the same as src and the copy don't take
dest's space into account, so there may be overflow happens, that's why strncpy is introduced!

char *strcat(char *dest, const char *src);
strcat's FINAL result:similar to strcpy, it will append all src's null terminated string to the end of
dest and it don't take account if dest has enough spare space to hold src, so there may be overflow happends,
that's why strncat is introduced!

char* strncpy(char *dest, const char *src, size_t n):
math induction for strncpy and strlcpy:
n:0, no any copy operation;
n:1, 1 byte is copied:suppose strlen(src) is 0/1/2 respectively, which is <n,==n,>n respectively
	src				dest		strlen(src) vs n
	'\0'			'\0'		<
	'a' '\0'		'a'			==
	'a' 'b'			'a'			>
n:2, 2 bytes is copied:suppose strlen(src) is 0/1/2/3 respectively, which is <n,<n,==n,>n respectively
	src				dest		strlen(src) vs n
	'\0'			'\0' '\0'	<
	'a' '\0'		'a' '\0'	<
	'a''b''\0'		'a''b'		==
	'a''b''c'		'a''b'		>
summarizaton:AL n (n>=0) bytes are copied;
when n>=1, if:
a. strlen(src) < n: the whole src including its tailing '\0' is copied. The ending bytes of (n - strlen(src) - 1) of dest, if >0 rather
than==0,will be appended with '\0' to make totally n bytes copied;
b. strlen(src) == n:the whole src except its trailing '\0' is copied, and dest has no terminated '\0'
c. strlen(src) > n:src's [0,n - 1] is copied, and dest has no terminated '\0'

size_t strlcpy(char *dest, const char *src, size_t count)
to solve the above b and c problem that the @dest is not null terminated after copy; strlcpy is introduced:
a.count: 0, no copy happpened, return strlen(src); strlen(src)>=0 is AL true, which means truncating AL happens even strlen(src)==0 since no '\0' is appended to @dest)
b.count:1 src		dest		return				trucated(strlen(src)>=count)?
		'0'			'\0'		strlen(src):0		false
		'a''\0'		'\0'		ditto:1				true
		'a''b''\0'	'\0'		ditto:2				true
c.count:2	src			dest			return 		trucated(strlen(src)>=count)?
		'\0'			'\0'		strlen(src):0	false
		'a''\0'			'a''\0'		strlen(src):1	false
		'a''b''\0'		'a''\0'		strlen(src):2	true
when count>=1,if:
a.strlen(src)<count:the whole src including trailing '\0' is copied to dest so no trucated;
b.strlen(src)>=count:AL truncated and only src's [0,count-2] is copied and [count-1] is written with '\0';
summarizaton:strlcpy AL make sure the dest is null terminated so there may be truncating occurred, to let the caller
know if truncating happened, the return value is used. Besides, strlcpy will not repeat to write trailing '\0' once the 1st '\0'
is written to @dest;

char *strncat(char *dest, const char *src, size_t count),dest should has >=(count+1) spare bytes(dest's old '\0' before strncat is also spare byte)
math induction for strncat and strlcat:
n:0, no any append ops;
n:1, suppose strlen(src) is 0/1/2 respectively, which is <n,==n,>n respectively
	src				dest		strlen(src) vs n
	'\0'			'\0'		<
	'a' '\0'		'a''\0'		==
	'a' 'b'			'a''\0'		>
n:2, suppose strlen(src) is 0/1/2/3 respectively, which is <n,<n,==n,>n respectively
	src				dest				strlen(src) vs n
	'\0'			'\0'				<
	'a''\0'			'a''\0'				<
	'a''b''\0'		'a''b''\0'			==
	'a''b''c'		'a''b''\0'			>
summarizaton:n==0 is special case that nearly impossible for normal use.So we just remember n>=1
when n>=1, if:
a. strlen(src) < n: the whole src including the trailing '\0' is copied to dest. The ending bytes of (n - strlen(src) - 1), which >=0 and will not be touched.
NOTE:strncpy's a is similar to strncat's a, except that the former will fill (n -strlen(src)-1),if any, with '\0' but the later will not touch them;
b. strlen(src) >= n:src's [0,n - 1] is appended(no chance for src's trailing '\0' is appended since '\0' is at [n] at most(when
strlen(src)==n)); then dest is written with a terminated '\0'; so totaly (n+1) bytes is written and the dest should have >=(n+1) spare
 bytes;

size_t strlcat(char *dest, const char *src, size_t count) count is sizeof dest; AL return strlen(dest)+strlen(src)
note count <= strlen(dest) is forbidden so count - strlen(dest) >= 1;if we use n for (count-strlen(dest)):
n:1, suppose strlen(src) is 0/1/2 respectively, which <,==,> n respectively;
		src			dest		strlen(src) vs n (truncated)
		'0'			'\0'		< (false)
		'a''\0'		'\0'		== (true)
		'a''b'		'\0'		> (true)
n:2,suppose strlen(src) is 0,1,2,3 respectively, which <,<,==,>n respectively
	src				dest			strlen(src) vs n (truncated)
	'\0'			'\0'			< (false)
	'a''\0'			'a''\0'			< (false)
	'a''b''\0'		'a''\0'			> (true)
	'a''b''c'		'a''\0'			> (true)
summarazation when n>=1,if:
a.strlen(src)<n:no truncated and the whole src include its trailing '\0' is appended to dest+strlen(dest)
b.strlen(src)>=n:trucated happens and src's [0,n-1) is appened to dest[strlen(dest)], then dest[strlen(dest)+n-1] is written with '\0'

for strncpy,strlcpy,strlcat:
when n>=1(strncpy/strlcpy:n is sizeof dest, for strlcat:n is (count-strlen(dest)):
to make the src 's all chars including its trailing '\0' is copied(strncpy/strlcpy) or appended(strlcat)
to dest,the strlen(src) should be < n;

for strncat(dest, src, count), to copy all src chars including trailing '\0' to dest, strlen(src) should <=count
*/
/*
suppose most common examples:
len = strlen(src); len > 0 && n > 0
1.len < n(at most len == (n - 1)):completely strcpy('\0' is included);
2.len == n:	a non null-terminated src, but no missing, will be put in dest
3.len > n:	[0, n - 1] of src will be put in dest and dest is not null-terminated
special case:
n == 0, then no strcpy and return dest directly;
n == 1 && len == 0, then dest will be copied with one byte '\0';
n == 1 && len == 1, then dest will be copied with one non-null byte only and then stops;
n == 1 && len > 1, then dest will be copied with one non-null byte only and then stops;
------
for common case:len = strlen(src);len > 0 && n > 0
1. len < n:(at most len == (n-1))completely copy('\0' included), return len;
2. len == n, a non null-terminated src, but no missing, will be put in dest, return len;
3. len > n, [0, n - 1] of src will be put in dest and dest is not null-terminated, return len;
arm 's assembly __strncpy_from_user(dst, src, n):
	ip = r1;
S:	r2 -= 1;
	if (r2 >= 0) {
		r3 = [r1];//only byte[0] is copied, byte[1],byte[2],byte[3] is filled with 0
		r1 += 1;
	} else
		goto E;
	[r0] = r3;//only byte[0] is copied
	r0 += 1;
	if (r3 != 0)//(r3^0)!=0
		goto S;
	r1 -= 1;
E:	r0 = r1 - ip;
	pc = lr;
*/
char *__strncpy(char *dest, const char *src, size_t n)
{
	int i;
	/*
	The strncpy is similar to strcpy, except that at most n bytes of src are copied. Warning:If no null byte among
	the first n bytes of src, the string placed in dest will not be null-terminated.
	*/
	for (i = 0; i < n && src[i]; i++)//if (i == n && src[i] != 0), the dest will not be null-terminated!
		dest[i] = src[i];
	/*
	If the length of src is less than n, strncpy writes additional null bytes to dest to ensure that a total of n 
	bytes are written.
	*/
	for (; i < n; i++)//if (i == n && !src[i]), additonal '\0' is written.
		dest[i] = '\0';
	/*
	if (i ==n && src[i] == 0), the dest will not be null-terminated!
	*/
	return dest;
	
}

int snprintf_test(const char *str)
{
	int ret;
	char buf[8];
	ret = snprintf(buf, sizeof(buf), str);
	printf(buf);
	printf("\t%d\n", ret);
	return ret;
}

int main(int argc, char *argv[], char *envp[])
{
	/*
	size_t is unsigned long, 32bits on arch32 and 64bits on arch64
	ssize_t is SIGNED long!
	int snprintf(char *buf, size_t size, const char *fmt, ...);
	int vsnprintf(char *str, size_t size, const char *format, va_list ap);
	The functions snprintf() and vsnprintf() write at most size bytes (including the terminating null byte ('\0')) 
	to str.
	return value is AL the strlen of the original source string:
	1. when no truncating, return strlen of string
	2. when truncating, the strlen of original source string which has been truncated
	so:
	1. if the return value <= size - 1, there is no truncating; when return size - 1, the whole buf has been used, the
	   last byte is '\0'
	2. if the return value >= size, truncating has happened
	*/
	snprintf_test("deadbe");//return strlen, i.e. 6
	snprintf_test("deadbee");//return strlen, i.e. 7
	snprintf_test("deadbeef");//return 8
	snprintf_test("deadbeeffacebooc");//return 16
	kobject_uevent_env(envp);
	return 0;
}
