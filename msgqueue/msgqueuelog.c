/*
XSI message queue example from Unix System Programming
*/
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define PERM (S_IRUSR | S_IWUSR)

static int queueid;

struct mymsg{
	long mtype;//this field should be "long" and must be a value >0
	char mtext[1];
};

int initqueue(key_t key)
{
	queueid = msgget(key, PERM | IPC_CREAT);
	if (queueid == -1) {
		perror("msgget error");
		return -1;
	}
	return queueid;
}

/*
	msgprintf("%s:%d\n", __func__, __LINE__);
	msgprintf("");
	the result will be 8 and 0
	
	int snprintf(char *str, size_t size, const char *format, ...);
	int vsnprintf(char* str, size_t size, const char *format, va_list ap);
	
	for snprintf/vsnprintf with sizeof(str)>=1, if the return value>=size, then the output string is truncated.
	
*/
int msgprintf(int pid, char *fmt, ...)
{
	va_list ap;
	char ch;
	int error;
	int len;
	struct mymsg *msgp;
	
	va_start(ap, fmt);
	len = vsnprintf(&ch, 1, fmt, ap);
#if 0
	The functions snprintf() and vsnprintf() do not write more than size bytes (including the terminating  null  byte  ('\0')).
	If the output was truncated due to this limit, then the return value is the number of characters (excluding the terminating null byte) which would have been written to the final string if enough space had been available.  Thus, a return  value  of size or more means that the output was truncated.  (See also below under NOTES.)
#endif
	printf("len %d\n", len);//return value of vsnprintf like strlen, no '\0'
	msgp = malloc(sizeof(struct mymsg) + len);//space containing '\0' will be allocated.
	if (msgp == NULL)
		return -1;
	msgp->mtype = pid;
	//vsnprintf(msgp->mtext, len + 1, fmt, ap);//actually no need to specify len!
	vsprintf(msgp->mtext, fmt, ap);
	va_end(ap);
#if 0
	MANUAL of msgsnd:
	struct mymsg {
	   long   mtype;       /* Message type. */
	   char   mtext[1];    /* Message text. */
	}
	The structure member mtype is a non-zero positive type long that can be used by the receiving process  for  message  selection.
	The structure member mtext is any text of length msgsz bytes. The argument msgsz can range from 0 to a system-imposed maximum.
#endif
	error = msgsnd(queueid, msgp, len + 1, 0);//'\0' will be sent
	free(msgp);
	if (error == -1) {
		perror("msgsnd error!");
		error = errno;
		return error;
	}
	return 0;
}

/*
here len can contain '\0', or not. It doesn't matter.
if containing '\0', msgrcv will get a string containing '\0' and write (its strlen + 1)
if no '\0', msgrcv will get a string without '\0' at the tail but its strlen is known so write (strlen) is OK!
*/
int msgwrite(int pid, void *buf, int len)
{
	struct mymsg *msgp;
	msgp = malloc(sizeof(struct mymsg) + len - 1);
	if (msgp == NULL)
		return -1;
	msgp->mtype = pid;
	memcpy(msgp->mtext, buf, len);
	if (msgsnd(queueid, msgp, len, 0) == -1) {
		perror("msgsnd error");
		free(msgp);
		return errno;
	}
	free(msgp);
	return 0;
}

int rmmsgqueue(void)
{
	if (msgctl(queueid, IPC_RMID, NULL) == -1) {
		perror("rm msg queue fails!");
		return -1;
	}
	return 0;
}
#if 0
int main(int argc, char **argv)
{
	msgprintf("%s:%d\n", __func__, __LINE__);
	msgprintf("");
	return 0;
}
#endif