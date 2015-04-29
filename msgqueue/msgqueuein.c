//gcc -Wall msgqueuelog.c msgqueuein.c -o client.out
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#if 0
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include "msgqueuelog.h"

#define MAXLINE 1024
char buf[MAXLINE];
//
int readline(int fd, char *buf, int nbytes) {
   int numread = 0;
   int returnval;

   while (numread < nbytes - 1) {
      returnval = read(fd, buf + numread, 1);
      if ((returnval == -1) && (errno == EINTR))
         continue;
      if ((returnval == 0) && (numread == 0))
         return 0;
      if (returnval == 0)
         break;
      if (returnval == -1)
         return -1;
      numread++;
      if (buf[numread-1] == '\n') {
         buf[numread] = '\0';
         return numread;
      }  
   }   
   errno = EINVAL;
   return -1;
}

//
int main(int argc, char **argv)
{
	int key, size, pid;
	
	pid = getpid();
	if (argc != 2) {
		fprintf(stderr, "Usage: %s key, key != 0\n", argv[0]);
		return 1;
	}
	
	key = atoi(argv[1]);
	if (initqueue(key) == -1) {
		return 1;
	}
	if (msgprintf(pid, "This is process %d\n", getpid()) == -1) {
		return 1;
	}
	for ( ; ; ) {
#if 1
		if ((size = readline(STDIN_FILENO, buf, MAXLINE)) == -1)
			break;
#else
		char *s = readline(NULL);
		if (!s)
			break;
		free(s);
#endif
		if (msgwrite(pid, buf, size))//buf[size]=='\0', so size will not contain '\0'
			break;
	}
	return 0;
}
