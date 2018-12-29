#define _GNU_SOURCE
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
#if 10
	for (i = 0; i < loops; i++)
		glob++;
#endif
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t t[CONFIG_THREAD_NR];
	int loops = 10000000, s, i;
	struct arg arg;

	if (argc > 1)
		loops = atoi(argv[1]);
	printf("loop %d\n", loops);
	arg.loops = loops;
	for (i = 0; i < CONFIG_THREAD_NR; i++) {
		arg.cpu = i;
		s = pthread_create(&t[i], NULL, threadFunc, &arg);
		if (s != 0)
			err_exit(s, "***cannot create thread %d", i);
		s = pthread_join(t[i], NULL);
		if (s != 0)
			err_exit(s, "***join %d", i);		
	}
	return 0;
}