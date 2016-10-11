#include <stdio.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
	/*
	int snprintf(char *buf, size_t size, const char *fmt, ...)
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
	snprintf_test("deadbee");//return strlen, i.e. 7
	snprintf_test("deadbeef");//return 8
	snprintf_test("deadbeeffacebooc");//return 16
	return 0;
}