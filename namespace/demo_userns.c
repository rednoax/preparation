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
	/*
	The low byte of flags, ie lowest byte, contains the number of the termination signal sent to the parent when
	the child dies. If this signal is specified as anything other then SIGCHLD, then the parent process must specify
	the __WALL or __WCLONE options when waiting for the child with wait(2). If no signal is specified, then the parent
	process is not signaled when the child terminates.
	*/
	/*
	for fork(), it will call sys_clone and the signal is SIGCHLD:
Program received signal SIGINT, Interrupt.
cpu_arm926_do_idle () at /root/kernel/arch/arm/mm/proc-arm926.S:113
113             mcr     p15, 0, r1, c1, c0, 0           @ Restore ICache enable
(gdb) b sys_clone
Breakpoint 1 at 0xc00265d4: file /root/kernel/arch/arm/kernel/sys_arm.c, line 233.
(gdb) c
Continuing.

Breakpoint 1, sys_clone (clone_flags=18874385, newsp=0, parent_tidptr=0x0, tls_val=0, child_tidptr=0x40007068, regs=0xc7817fb0)
    at /root/kernel/arch/arm/kernel/sys_arm.c:233
233     {
(gdb) p/x clone_flags
$1 = 0x1200011
(gdb) c
Continuing.

Breakpoint 1, sys_clone (clone_flags=18874385, newsp=0, parent_tidptr=0x0, tls_val=0, child_tidptr=0x30518, regs=0xc7d2ffb0)
    at /root/kernel/arch/arm/kernel/sys_arm.c:233
233     {
(gdb) p/x clone_flags
$2 = 0x1200011
(gdb) p/x ((struct thread_info*)((int)$sp&~8191))->task->pid
$3 = 0x128
(gdb) p ((struct thread_info*)((int)$sp&~8191))->task->pid
$4 = 296
(gdb) c
Continuing.
---------------------------
~ # ./fork
new proc:297 ppid:296
parent:296 child pid 297
~ #
	*/
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