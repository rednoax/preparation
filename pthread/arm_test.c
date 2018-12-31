//~/OP-TEE/toolchains/aarch32/bin/arm-linux-gnueabihf-gcc -Wall -static -pthread mutex.c
#define _GNU_SOURCE
#define __USE_GNU//actually seem useless at all!
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

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

void err_cont(int error, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
}

static int glob = 0;

struct arg {
	int loops;
	int cpu;
	pthread_mutex_t *lock;
	//int *my_lock;
};

//0:unlock 1:locked
#define LOCKED 1
#define UNLOCKED 0
volatile int my_lock = 0;

typedef void (*mutex)(struct arg*);
void (*mutex_lock)(struct arg*);
void (*mutex_unlock)(struct arg*);

void dummy_lock(struct arg *argp)
{
}

void dummy_unlock(struct arg *argp)
{
}

void std_lock(struct arg *argp)
{
	pthread_mutex_lock(argp->lock);
}

void std_unlock(struct arg *argp)
{
	pthread_mutex_unlock(argp->lock);
}

void broken_lock_v0(struct arg *argp)
{
	volatile int val;
	__asm__ __volatile__(
"1:	ldr %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	mov %0, %2\n"
"	str %0, [%1]\n"
	: "=&r" (val)
	: "r" (&my_lock), "I"(LOCKED)
	: "cc");
}

void broken_unlock_v0(struct arg *argp)
{
	my_lock = UNLOCKED;
}

mutex mutexes[][2] = {
	{dummy_lock, dummy_unlock},
	{std_lock, std_unlock},
	{broken_lock_v0, broken_unlock_v0},
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
	//pthread_mutex_t *lock = argp->lock;
	cpu_set_t cpuset;
	int cnt = 1;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
#if 10
reset:
	//pthread_mutex_lock(lock);//no improvament, only many times trying can acchive successful binding
	s = pthread_setaffinity_np(thread, sizeof(size_t), &cpuset);
	if (s != 0) {
		//err_cont(s, "***pthread_setaffinity_np for %lu on CPU %d", thread, cpu);
		cnt++;
		goto reset;
	} else {
		//printf("tid %lu on %d:", thread, cpu);
		cpu_set_t real;
		s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &real);
		if (s != 0)
			err_cont(s, "***pthread_getaffinity_np for %lu on CPU %d", thread, cpu);
		else {
			/*man 3 CPU_SET
			if (real != cpuset)//wrong!
			*/
			if (!CPU_EQUAL(&real, &cpuset))
				printf("***expect %d, but", cpu);
			for (i = 0; i < CONFIG_THREAD_NR; i++) {
				if (CPU_ISSET(i, &real))
					printf("[%04d]t %lu runs on CPU %d\n", cnt, thread, i);
			}
		}
	}
	//pthread_mutex_unlock(lock);
#endif
#if 0
	printf("==t%lu sleep\n", thread);
	/*
https://stackoverflow.com/questions/8032372/how-can-i-see-which-cpu-core-a-thread-is-running-in
	f to select last CPU field to make sure 4 threads are on expected CPU;
	*/
	pause();
#else
/*
android ver:
https://stackoverflow.com/questions/9287315/finding-usage-of-resources-cpu-and-memory-by-threads-of-a-process-in-android
ps -p -t -c 7624
x86 ver:
https://stackoverflow.com/questions/8032372/how-can-i-see-which-cpu-core-a-thread-is-running-in
*/
	for (i = 0; i < loops; i++) {
		mutex_lock(argp);
		glob++;
		mutex_unlock(argp);
	}
#endif
	return (void*)cpu;
}

int main(int argc, char **argv)
{
	pthread_t t[CONFIG_THREAD_NR];
	int loops = 10000000, s, i;
	void *rval_ptr;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
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
	int j = 0;

	if (argc > 1)
		loops = atoi(argv[1]);
	printf("pid%d tid%lu loop %d\n", getpid(), thread, loops);
	//sleep(10);

	//
next:
	mutex_lock = mutexes[j][0];
	mutex_unlock = mutexes[j][1];
	printf("---%d---\n", j);
	//
	for (i = 0; i < CONFIG_THREAD_NR; i++) {
		struct arg * argp = arg + i;
		argp->loops = loops;
		argp->cpu = i;
		argp->lock = &lock;
		s = pthread_create(&t[i], NULL, threadFunc, argp);
		if (s != 0)
			err_exit(s, "***cannot create thread %d", i);
		else
			printf("create %lu, expect on CPU %d\n", t[i], i);
	}
	for (i = 0; i < CONFIG_THREAD_NR; i++) {
		s = pthread_join(t[i], &rval_ptr);
		if (s != 0)
			err_exit(s, "***join %d", i);
		else
			printf("join %lu get %d\n", t[i], (int)rval_ptr);
	}
	/*
	printf("t%lu sleep\n", thread);
	pause();
	*/
	printf("Final %d\n", glob);
	//
	j++;
	glob = 0;
	if (j < ARRAY_SIZE(mutexes))
		goto next;
	//
	return 0;
}
