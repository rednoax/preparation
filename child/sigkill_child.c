#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define MAXLINE 4096
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char buf[MAXLINE];
	/*
	int snprintf(char *str, size_t size, const char *format, ...);
	int vsnprintf(char *str, size_t size, const char *format, va_list ap);
	snprintf and vsnprintf write at most size bytes
	(including the terminating '\0') to str
	*/
	vsnprintf(buf, MAXLINE, fmt, ap);
	if (errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s", strerror(error));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
}

/*
va_start(ap, last)
va_arg(ap, type)

once the above macro is used, both of them will make ap point to a position
on the stack whose address is immediately after the 2nd parameter, which can
be passed to another function with variable parameter.
The left parameter is at low address of stack!
*/
void err_sys(const char *fmt, ...)
{
	va_list ap;//char *ap;
	va_start(ap, fmt);//after the macro, ap point to address right after &fmt on the stack
	err_doit(1, errno, fmt, ap);
	va_end(ap);//0.11 show it is nothing
	exit(1);

}

typedef void Sigfunc(int);
Sigfunc * signal_install(int signo, Sigfunc *func)
{
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
		act.sa_flags = SA_INTERRUPT;
	} else {
		act.sa_flags = SA_RESTART;
	}
	if (sigaction(signo, &act, &oact) < 0)
		return SIG_ERR;
	return oact.sa_handler;
}

int main(int argc, char* argv[])
{
	int status, ret;
	pid_t pid;
	printf("%d\n", sizeof("/.fs_state"));
	if (argc >1 && atoi(argv[1]) > 0 && signal_install(SIGCHLD, SIG_IGN) == SIG_ERR)
		err_sys("signal(SIGCHLD) error");
	pid = fork();
	if (pid < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		printf("new proc:%d ppid:%d\n", getpid(), getppid());
		sleep(65536);
	} else {
#if 0
	man 2 wait
POSIX.1-2001 specifies that if the disposition of SIGCHLD is set to SIG_IGN or the SA_NOCLDWAIT flag  is  set  for  SIGCHLD
(see  sigaction(2)),  then children that terminate do not become zombies and a call to wait() or waitpid() will block until
all children have terminated, and then fail with errno set to ECHILD.  (The original POSIX standard left  the  behavior  of
setting  SIGCHLD  to SIG_IGN unspecified.  Note that even though the default disposition of SIGCHLD is "ignore", explicitly
setting the disposition to SIG_IGN results in different treatment of zombie process children.)
	apue 10.7:
	If it subsequently calls one of the wait functions, the calling process will block until all its children have terminated, and then wait returns 1 with errno set to ECHILD.
	***actually return -1 on x86 vmware archlinux
#endif
		char *argv[] = { "sleep", "10", NULL};
		printf("ppid %d:parent:%d child pid %d\n", getppid(), getpid(), pid);
		//exit(1);//neednot, use kill to test:if parent is killed then child process's ppid becomes 1
		sleep(50);
		ret = kill(pid, SIGKILL);
		printf("kill %d: %d\n", pid, ret);
		system("ls");
		sleep(50);
		printf("start execvp\n");
		execvp(argv[0], argv);
		printf("end execvp\n");
		if ((ret = wait(&status)) != pid) {
			printf("wait error %d\n", ret);
			exit(1);
		}
		sleep(5);// for ps or top
	} 
	return 0;
}
