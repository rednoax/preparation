//~/OP-TEE/toolchains/aarch32/bin/arm-linux-gnueabihf-gcc -Wall -static -pthread mutex.c
#define _GNU_SOURCE
#define __USE_GNU//actually seem useless at all!
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
typedef unsigned long long uint64_t;

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

#define CONFIG_THREAD_NR 4
#define CONFIG_PER_BATCH 10000000//25000000//
#define CONFIG_TEST_UP 0

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

double cpu_consumer(void)
{
	static double r = 10.0;
	long int val = random();
	val--;
	val |= val >> 1;
	val |= val >> 2;
	val |= val >> 4;
	val |= val >> 8;
	val |= val >> 16;
	val++;
	r += val * 3.1415;
	return r;
}

static unsigned int glob = 0;

struct arg {
	unsigned int loops;
	int cpu;
	pthread_mutex_t *lock;
	//int *my_lock;
};

//0:unlock 1:locked
#define LOCKED 1
#define UNLOCKED 0
volatile int my_lock = UNLOCKED;

typedef int (*mutex)(struct arg*);
int (*mutex_lock)(struct arg*);
int (*mutex_unlock)(struct arg*);

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
	__asm__ __volatile("11:");//mark the range in .s
	__asm__ __volatile__(
"1:	ldr %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	mov %0, %2\n"
"	str %0, [%1]\n"
	: "=&r" (val)
	: "r" (&my_lock), "I"(LOCKED)
	: "cc");
	__asm__ __volatile("22:");
}

void broken_unlock_v0(struct arg *argp)
{
	my_lock = UNLOCKED;
}

/*
ldrex/strex test result on x86 is right!(You can run arm's ./a.out on ubuntu x86 directly)
*/
int broken_lock_v1(struct arg *argp)//spin way
{
	volatile int val, ret;
	__asm__ __volatile("11:");//mark the range in .s
	__asm__ __volatile__(
"1:	ldrex %0, [%2]\n"
"	cmp %0, %3\n"
"	beq 1b\n"
"	mov %0, %3\n"
"	strex %1, %0, [%2]\n"
"	cmp %1, #0\n"
"	bne	1b\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "I"(LOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return !ret;
}

int broken_unlock_v1(struct arg *argp)
{
	my_lock = UNLOCKED;
	return 0;
}

/*
try_lock way:
1. it can emit error on smp:
---0: 1---
create 2838512384, expect on CPU 0
create 2846905088, expect on CPU 1
create 2855297792, expect on CPU 2
create 2863690496, expect on CPU 3
[0001]t 2838512384 runs on CPU 0
[0001]t 2855297792 runs on CPU 2
[0001]t 2863690496 runs on CPU 3
[0001]t 2846905088 runs on CPU 1
[ 7821.025617] healthd: Unknown battery health 'Warm'
[ 7846.923223] wlan: [3549:E :CTL] Error returned WDI_ProcessInitScanRspi:0 BMPS0
join 2838512384 get 0
join 2846905088 get 1
join 2855297792 get 2
join 2863690496 get 3
***866/(10^8):Final 99999134 took 45.20s

2. no ANY error on UP when k == 1:
---1: 1---
create 2838512384, expect on CPU 0
create 2846905088, expect on CPU 0
create 2855297792, expect on CPU 0
create 2863690496, expect on CPU 0
[0001]t 2855297792 runs on CPU 0
[0001]t 2863690496 runs on CPU 0
[0001]t 2846905088 runs on CPU 0
[0001]t 2838512384 runs on CPU 0
join 2838512384 get 0
join 2846905088 get 0
join 2855297792 get 0
join 2863690496 get 0
Final 100000000 took 35.57s
*/
int broken_lock_v2(struct arg *argp)
{
	int ret;
	__asm__ __volatile("11:");//mark the range in .s
	__asm__ __volatile__(
"1: ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 2f\n"
"	strex %0, %3, [%1]\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return !ret;
}

int broken_unlock_v2(struct arg *argp)
{
	my_lock = UNLOCKED;
	return 0;
}

int broken_lock_v3(struct arg *argp)
{
	volatile int val, ret;
	__asm__ __volatile("11:");//mark the range in .s
	__asm__ __volatile__(
"1:	ldrex %0, [%2]\n"
"	cmp %0, %3\n"
"	wfeeq\n"
"	beq 1b\n"
"	mov %0, %3\n"
"	strex %1, %0, [%2]\n"
"	cmp %1, #0\n"
"	bne	1b\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "I"(LOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return !ret;
}

int broken_unlock_v3(struct arg *argp)
{
	__asm__ __volatile("11:");
	__asm__ __volatile__(
"\n"
"1:\n"
"	str %1, [%0]\n"
"	sev\n"
"2:\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return 0;
}

int broken_lock_v4(struct arg *argp)//Add mem barrier to test
{
	int ret;
	__asm__ __volatile("11:");//mark the range in .s
	__asm__ __volatile__(
"1: ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 2f\n"
"	strex %0, %3, [%1]\n"
"2:	dmb\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return !ret;
}

int broken_unlock_v4(struct arg *argp)
{
	__asm__ __volatile("11:");
	__asm__ __volatile__(
"\n"
"1:\n"
"	str %1, [%0]\n"
"2:\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return 0;
}

int broken_lock_v5(struct arg *argp)//Add mem barrier to test
{
	int ret;
	__asm__ __volatile("11:");//mark the range in .s
	__asm__ __volatile__(
"1: ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 2f\n"
"	strex %0, %3, [%1]\n"
"2:	#dmb\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return !ret;
}

int broken_unlock_v5(struct arg *argp)
{
	__asm__ __volatile("11:");
	__asm__ __volatile__(
"\n"
"1:\n"
"	str %1, [%0]\n"
"2:\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return 0;
}

int broken_unlock_v5p5(struct arg *argp)
{
	cpu_consumer();
	__asm__ __volatile("11:");
	__asm__ __volatile__(
"\n"
"1:\n"
"	str %1, [%0]\n"
"2:\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	__asm__ __volatile("22:");
	return 0;
}

int broken_unlock_v5p6(struct arg *argp)
{
	__asm__ __volatile("11:");
	__asm__ __volatile__(
"\n"
"1:\n"
"	str %1, [%0]\n"
"2:\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	cpu_consumer();
	__asm__ __volatile("22:");
	return 0;
}

mutex mutexes[][2] = {
#if 10
	{broken_lock_v1, broken_unlock_v1},//spin way:no error
	{broken_lock_v2, broken_unlock_v2},//try lock way: ***596/(10^8):Final 39999404 took 18.41s
	{broken_lock_v3, broken_unlock_v3},//wfe-spin:no error
	{broken_lock_v4, broken_unlock_v4},//only one dmb for try lock: expect error but no error!!!
	{broken_lock_v5, broken_unlock_v5},//error will emit but much less than v2: ***1/(10^8):Final 39999999 took 18.68s
	{broken_lock_v5, broken_unlock_v5p5},//AL no error:Final 100000000 took 182.82s
	{broken_lock_v5, broken_unlock_v5p6},//Final 40000000 took 34.66s
	{broken_lock_v5, broken_unlock_v2},//***668/(10^8):Final 39999332 took 18.82s
#endif
};

/*
pthread_setaffinity_np
http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
gcc -Wall -pthread pthread_setaffinity.c
*/
static void *threadFunc(void *_arg)
{
	struct arg * argp = _arg;
	unsigned int loops = argp->loops;
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
	for (i = 0; i < loops; ) {
/*
1. why cpu_consumer is necessary:
when it is removed:
***Final 39999996 took 4.52s
after adding:
***Final 39999385 took 18.42s
2. If you run this arm binary on ubuntu, NOT a single error can emit!AL 4000 0000!
*/
		cpu_consumer();
		if (mutex_lock(argp)) {
			glob++;
			mutex_unlock(argp);
			i++;
		}
	}
#endif
	return (void*)cpu;
}

uint64_t gettime_ns() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000ULL + now.tv_nsec;
}

int main(int argc, char **argv)
{
	pthread_t t[CONFIG_THREAD_NR];
	int s, i, j, k;
	unsigned int delta, loops = CONFIG_PER_BATCH;
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
	uint64_t start;
	double duration;

	if (argc > 1)
		loops = atoi(argv[1]);
	printf("pid%d tid%lu loop %u\n", getpid(), thread, loops);
	//sleep(10);

	//
	j = k = 0;
next:
	mutex_lock = mutexes[j][0];
	mutex_unlock = mutexes[j][1];
	printf("---%d: %d---\n", k, j);
	start = gettime_ns();
	//
	for (i = 0; i < CONFIG_THREAD_NR; i++) {
		struct arg * argp = arg + i;
		argp->loops = loops;
		argp->cpu = k? 0: i;
		argp->lock = &lock;
		s = pthread_create(&t[i], NULL, threadFunc, argp);
		if (s != 0)
			err_exit(s, "***cannot create thread %d", i);
		else
			printf("create %lu, expect on CPU %d\n", t[i], argp->cpu);
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
	duration = (gettime_ns() - start) / 1000000000.0;
	delta = CONFIG_THREAD_NR * loops - glob;
	if (delta != 0) {
		char buf[1024];
		int ret = snprintf(buf, sizeof(buf), "***%d/(10^8):", delta);
		write(STDERR_FILENO, buf, ret);
		//fprintf(stderr, );
	}
	printf("Final %d took %.2fs\n", glob, duration);
	//
	glob = 0;
	if (++j < ARRAY_SIZE(mutexes))
		goto next;
	if (k++ < CONFIG_TEST_UP) {
		j = 0;
		goto next;
	}
	//
	return 0;
}
