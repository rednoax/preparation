#include <stdio.h>

const char str[] = "0123456789abcdef";
const char str2[] = {
	'1', '2', '3', '4', '\0', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 
};
int main()
{
	printf("[%*s]\n", 4, str);//[0123456789abcdef]
	printf("[%*s]\n", 10, str);//[0123456789abcdef]
	printf("[%.*s]\n", 4, str);//[0123]
	printf("[%.*s]\n", 10, str);//[0123456789]
	printf("[%.*s]\n", 3, str2);//[123]
	printf("[%.*s]\n", 4, str2);//[1234]
	printf("[%.*s]\n", 8, str2);//[1234]
	return 0;
}