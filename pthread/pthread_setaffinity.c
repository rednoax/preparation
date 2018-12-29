#define _GNU_SOURCE
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define CONFIG_THREAD_NR 4

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char buf[4096];
	vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	if (errnoflag)
		snprintf(buf + strlen(buf), sizeof(buf) - 1 - strlen(buf), ":%s", strerror(error));
	strcat(buf, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
}

void err_exit(int error, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
	exit(1);
}

static int glob = 0;

struct arg {
	int loops;
	int cpu;
};

/*
pthread_setaffinity_np
http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
gcc -Wall -pthread pthread_setaffinity.c
*/
static void *threadFunc(void *_arg)
{
	struct arg * argp = _arg;
	int loops = argp->loops;
	int i, s, cpu = argp->cpu;
	pthread_t thread = pthread_self();
	
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	if (s != 0)
		err_exit(s, "***pthread_setaffinity_np for %ld on CPU %d", thread, cpu);
	s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	if (s != 0)
		err_exit(s, "***pthread_getaffinity_np for %ld on CPU %d", thread, cpu);
	printf("Expect %d on thead %ld:", cpu, thread);
	for (i = 0; i < CONFIG_THREAD_NR; i++) {
		if (CPU_ISSET(i, &cpuset))
			printf("CPU %d\n", i);
	}
#if 0
	printf("==t%ld sleep\n", thread);
	/*
https://stackoverflow.com/questions/8032372/how-can-i-see-which-cpu-core-a-thread-is-running-in
	f to select last CPU field to make sure 4 threads are on expected CPU;
	*/
	pause();
#else
	for (i = 0; i < loops; i++)
		glob++;
#endif
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t t[CONFIG_THREAD_NR];
	int loops = 10000000, s, i;
	pthread_t thread = pthread_self();
	/*
	if using single arg rather than array, RACE will occure as following!!!:
loop 10000000
create 139728456488704, expect on CPU 0
create 139728448096000, expect on CPU 1
create 139728439703296, expect on CPU 2
create 139728431310592, expect on CPU 3
Expect 3 on thead 139728431310592:CPU 3
==t139728431310592 sleep
Expect 3 on thead 139728439703296:CPU 3
==t139728439703296 sleep
Expect 2 on thead 139728448096000:CPU 2
==t139728448096000 sleep
Expect 1 on thead 139728456488704:CPU 1
==t139728456488704 sleep
	*/
	struct arg arg[CONFIG_THREAD_NR];

	if (argc > 1)
		loops = atoi(argv[1]);
	printf("%d loop %d\n", getpid(), loops);
	sleep(10);
	for (i = 0; i < CONFIG_THREAD_NR; i++) {
		struct arg * argp = arg + i;
		argp->loops = loops;
		argp->cpu = i;
		s = pthread_create(&t[i], NULL, threadFunc, argp);
		if (s != 0)
			err_exit(s, "***cannot create thread %d", i);
		else
			printf("create %ld, expect on CPU %d\n", t[i], i);
	}
	for (i = 0; i < CONFIG_THREAD_NR; i++) {
		s = pthread_join(t[i], NULL);
		if (s != 0)
			err_exit(s, "***join %d", i);		
	}
	/*
	printf("t%ld sleep\n", thread);
	pause();
	*/
	printf("Final %d\n", glob);
	return 0;
}