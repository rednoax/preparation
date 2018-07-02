#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define FILENAME "fstab.qcom"
int main(int argc, char *argv[])
{
	ssize_t len;
	char *line = NULL;
	const char *fstab_path = FILENAME;
	size_t alloc_len = 0;
	//int fd = open(FILENAME, O_RDONLY);
	if (argc > 1)
		fstab_path = argv[1];
	FILE *fstab_file = fopen(fstab_path, "r");
	printf("open %s: %p\n", fstab_path, fstab_file);
	/*
	if fstab_file doesnot exist at all, getline() will not work at all
open unexi: (nil)
Segmentation fault (core dumped)

	if fstab_file is a dummy empty file, which has existed, getline will allocate @line
	all the same, but return -1; So @line should be freed even if getline() return -1;

	alloc_len is the byte length of @line
	1. If @line==NULL and @alloc_len==0 before the call, getline() will allocated a buffer
	whose length is saved @alloc_len.
	2. If @line!=NULL and @alloc_len!=0 before the call, but the buffer is not large enough
	to hold the line(ie. @alloc_len is not big enough), getline() will realloc @line and
	update @alloc_len to reflect the new size;
	
	The buffer should be freed by the user program even	if getline() failed(return -1).
	-1: no line ending with '\n' found
	>= 1: a line ending with '\n' found, it is added with a null after '\n' to
	      form a null-terminated line and stored in @line; the returned value is strlen(line)(@line 's last 2
		  chars are '\n', '\0');
	*/
	while ((len = getline(&line, &alloc_len, fstab_file)) != -1) {
		printf("[%s]", line);
		printf("{%p: %ld %ld}\n", line, len, alloc_len);
	}
	printf("%p: last len %ld, alloc_len %ld\n", line, len, alloc_len);
	fclose(fstab_file);
	free(line);
	return 0;
}