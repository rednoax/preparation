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
	int status;
	pid_t pid;

	if (argc >1 && atoi(argv[1]) > 0 && signal_install(SIGCHLD, SIG_IGN) == SIG_ERR)
		err_sys("signal(SIGCHLD) error");
	pid = fork();
	if (pid < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		printf("new proc:%d ppid:%d\n", getpid(), getppid());
	} else {
		printf("parent:%d child pid %d\n", getpid(), pid);
		sleep(10);
		if (wait(&status) != pid) {
			printf("wait error");
			exit(1);
		}
	} 
	return 0;
}
