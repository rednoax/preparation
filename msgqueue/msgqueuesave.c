//gcc -Wall msgqueuelog.c msgqueuesave.c -o server.out
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include "msgqueuelog.h"
#define MAXSIZE 4096
struct mymsg {
	long type;
	char mtext[MAXSIZE];
};

struct mymsg msg;

int main(int argc, char **argv)
{
	
	key_t key;//typedef int __kernel_key_t;
	int id, size;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s key, key != 0\n", argv[0]);
		return 1;
	}
	key = atoi(argv[1]);
	id = initqueue(key);
	if (id == -1)
		return 1;
	printf("msg queue %d\n", id);
	for ( ; ; ) {
		char buf[32];
#if 0
       The argument msgflg specifies the action to be taken if a message of the desired type is not on the queue.  These  are  as follows:
        *  If  (msgflg  & IPC_NOWAIT) is non-zero, the calling thread shall return immediately with a return value of -1 and errno set to [ENOMSG].
        *  If (msgflg & IPC_NOWAIT) is 0, the calling thread shall suspend execution until one of the following occurs:
           --  A message of the desired type is placed on the queue.
           --  The message queue identifier msqid is removed from the system; when this occurs, errno shall be set to [EIDRM]  and -1 shall be returned.
           --  The  calling  thread receives a signal that is to be caught; in this case a message is not received and the calling thread resumes execution in the manner prescribed in sigaction().

       Upon successful completion, the following actions are taken with respect to the data structure associated with msqid:

        *  msg_qnum shall be decremented by 1.
        *  msg_lrpid shall be set to the process ID of the calling process.
        *  msg_rtime shall be set to the current time, as described in Section 2.7.1, IPC General Description.
#endif
		/*
		*  If msgtyp is 0, the first message on the queue shall be received.
        *  If msgtyp is greater than 0, the first message of type msgtyp shall be received.
		*/
		size = msgrcv(id, &msg, MAXSIZE, 0/*1*/, 0);
		if (size == -1) {
			perror("msgrcv fails");
			break;
		}
		/*
		//the following way cannot display successfully
		printf("PID %ld:", msg.type);
		fsync(STDOUT_FILENO);
		*/
		sprintf(buf, "PID %ld:", msg.type);
		write(STDOUT_FILENO, buf, strlen(buf));
		
		//here mtext/size can contain '\0' or not.
		if (write(STDOUT_FILENO, msg.mtext, size) == -1) {
			perror("output error");
			break;
		}
	
	}
	return 0;
}
#if 0//when running:
[root@localhost msgqueue]# ./server 1
msg queue 32768
PID 1869:This is process 1869
PID 1869:hi
PID 1871:This is process 1871
PID 1871:hi

[root@localhost ~]# ps -a
  PID TTY          TIME CMD
 1868 pts/0    00:00:00 server
 1869 pts/1    00:00:00 client
 1871 pts/2    00:00:00 client
 1881 pts/3    00:00:00 ps
[root@localhost ~]#

[root@localhost ~]# ipcs -a

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x00000001 32768      root       600        0            0

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status

------ Semaphore Arrays --------
key        semid      owner      perms      nsems
#endif
#if 0
[root@localhost msgqueue]# ./server 1
msg queue 65536
//ipcsrm -a in another console, then server will return
msgrcv fails: Identifier removed

#endif