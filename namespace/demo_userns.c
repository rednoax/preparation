#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

int *g_p;
#define errExit(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)
static int childFunc(void *arg)
{
	char *_arg = arg;
	printf("child access stack:%d %d,arg:\'%s\'\n", g_p[0], g_p[1], _arg);
	return 0xa5;
}

#define STACK_SIZE (1024*1024)

static char child_stack[STACK_SIZE];
int main(int argc, char *argv[])
{
	int stack_var[2] = {-99, -100};
	int status, ret, flags = SIGCHLD;//|CLONE_NEWUSER;//CLONE_NEWUSER make clone invocation emit "Invalid argument"
	pid_t pid;
	g_p = stack_var;
	/*
	clone(r0,r1,r2,r3)
	(r0==0 || r1==0) will "beq __error"
	*/
	pid = clone(childFunc, child_stack + STACK_SIZE, flags, argv[1]);
	if (pid == -1)
		errExit("clone");
	else
		printf("new Child PID:%d\n", pid);
	
	ret = waitpid(pid, &status, 0);
	if (ret == -1)
		errExit("waitpid");
	else
		printf("child %d return 0x%x;parent stack:%d, %d\n", ret, WEXITSTATUS(status), stack_var[0], stack_var[1]);
	exit(EXIT_SUCCESS);
}