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
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
typedef unsigned long long uint64_t;

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define CONFIG_CPU_NR 4
#define CONFIG_PER_BATCH 10000000//25000000//
#define CONFIG_TEST_UP 0
#define CONFIG_DEBUG
#ifdef CONFIG_DEBUG
#define debug(fmt, arg...) err_write(fmt, ##arg)
#else
#define debug(fmt, ...) do{}while(0)
#endif

#define CONFIG_NM (1000000000ULL)
#define CONFIG_OHM (CONFIG_NM * 1.0)
#define DELTA(t1, t2) (((t1) - (t2)) /  CONFIG_OHM)

uint64_t gettime_ns() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000ULL + now.tv_nsec;
}

static void err_write(const char *fmt, ...)
{
	char buf[4096];
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsnprintf(buf, sizeof(buf), fmt, ap);
	write(STDERR_FILENO, buf, ret);
	va_end(ap);
}

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
#if 10
	long int val = random();
	val--;
	val |= val >> 1;
	val |= val >> 2;
	val |= val >> 4;
	val |= val >> 8;
	val |= val >> 16;
	val++;
	r += val * 3.1415;
#endif
	return r;
}

static unsigned int glob = 0;

struct stamp {
	uint64_t stamp;
	double delta;
};

struct arg {
	unsigned int loops;
	int cpu;
	pthread_mutex_t *lock;
	pthread_cond_t *cond;
	//int *my_lock;
	pthread_t tid;
	struct stamp stamps[64];
	int stamps_nr;
	pthread_mutex_t l;
	pthread_cond_t c;
	int local, *pub;
	int index;
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

int broken_unlock_v2_near(struct arg *argp)
{
	my_lock = UNLOCKED;
	return 0;
}

int broken_unlock_v5p7(struct arg *argp)
{
	DIR *dp;
	struct dirent *dirp;
	char buf[4096];
	int pos = 0, ret;
	if ((dp = opendir(".")) == NULL)
		err_cont(errno, "can't open .");
	while((dirp = readdir(dp)) != NULL) {
		ret = snprintf(buf + pos, sizeof(buf) - pos, "%s", dirp->d_name);
		if (ret >= sizeof(buf) - pos)
			break;
		else
			pos += ret;
	}
	__asm__ __volatile__(
"	str %1, [%0]\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	while((dirp = readdir(dp)) != NULL) {
		ret = snprintf(buf + pos, sizeof(buf) - pos, "%s", dirp->d_name);
		if (ret >= sizeof(buf) - pos)
			break;
		else
			pos += ret;
	}
	memset(buf, 0, sizeof(buf));
	closedir(dp);
	return 0;
}

int broken_lock_v1p1(struct arg *argp)//alt spin way
{
	volatile int val, ret;
	__asm__ __volatile__(
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"1:	ldrex %0, [%2]\n"
"	mov %2, %2\n"
"	mov %1, %1\n"
"	mov %0, %0\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	cmp %0, %3\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	beq 1b\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	mov %0, %3\n"
"	strex %1, %0, [%2]\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	cmp %1, #0\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	bne	1b\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
"	mov %0, %0\n"
"	mov %1, %1\n"
"	mov %2, %2\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "I"(LOCKED)
	: "cc");
	return !ret;
}

int broken_unlock_v5p8(struct arg *argp)
{
	__asm__ __volatile__(
"	mov %0, %0\n"
"	mov %0, %0\n"
"	mov %0, %0\n"
"	mov %0, %0\n"
"	mov %0, %0\n"
"	mov %0, %0\n"
"	mov %0, %0\n"
"	mov %0, %0\n"
"	str %1, [%0]\n"
"	mov %1, %1\n"
"	mov %1, %1\n"
"	mov %1, %1\n"
"	mov %1, %1\n"
"	mov %1, %1\n"
"	mov %1, %1\n"
"	mov %1, %1\n"
"	mov %1, %1\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	return 0;
}


mutex mutexes[][2] = {
#if 0
	{broken_lock_v1, broken_unlock_v1},//spin way:no error
	{broken_lock_v2, broken_unlock_v2},//try lock way: ***596/(10^8):Final 39999404 took 18.41s
	{broken_lock_v3, broken_unlock_v3},//wfe-spin:no error
	{broken_lock_v4, broken_unlock_v4},//only one dmb for try lock: expect error but no error!!!
	{broken_lock_v5, broken_unlock_v5},//error will emit but much less than v2: ***1/(10^8):Final 39999999 took 18.68s
	{broken_lock_v5, broken_unlock_v5p5},//AL no error:Final 100000000 took 182.82s
	{broken_lock_v5, broken_unlock_v5p6},//Final 40000000 took 34.66s
	{broken_lock_v5, broken_unlock_v2},//***668/(10^8):Final 39999332 took 18.82s
	{broken_lock_v2, broken_unlock_v2_near},//try lock way with a much near unlock: ***9/(10^8):Final 39999991 took 19.02s
#else
	//{broken_lock_v5, broken_unlock_v5p7},//Final 40000000 took 4278.33s
	{broken_lock_v5, broken_unlock_v5p8},//***1345/(10^8):Final 39998655 took 17.97s, if cpu_consumer removed:***98934/(10^8):Final 39901066 took 4.55s
	{broken_lock_v1p1, broken_unlock_v5p8},//alter spin way:Final 40000000 took 13.41s
#endif
};

void getaffinity(struct arg *argp, cpu_set_t *expected)
{
	cpu_set_t real;
	pthread_t thread = pthread_self();
	int i, ret, cpu = -1;
	for (i = 0; i < CONFIG_CPU_NR; i++) {
		if (CPU_ISSET(i, expected)) {
			cpu = i;
			break;
		}
	}
	ret = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &real);
	if (ret != 0)
		err_cont(ret, "***pthread_getaffinity_np for %lu on CPU %d", thread, cpu);
	else {
		if (!CPU_EQUAL(&real, expected)) {
			err_write("***{%d}expect C %d,but on C ", argp->index, cpu);
			for (i = 0; i < CONFIG_CPU_NR; i++) {
				if (CPU_ISSET(i, &real)) {
					argp->cpu = i;
					err_write(":%d", i);
				}
			}
			err_write(">");
			*expected = real;
		}
	}

}
/*
pthread_setaffinity_np
http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
gcc -Wall -pthread pthread_setaffinity.c
*/
static void *threadFunc(void *_arg)
{
	struct arg * argp = _arg;
	double delta, secs, base = 10.0;
	unsigned int i, s, base_cnt = 1, cpu = argp->cpu;
	pthread_t thread = pthread_self();
	int index = 0, cnt = 1;
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	argp->stamps[index++].stamp = gettime_ns();
reset:
	s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	delta = DELTA(gettime_ns(), argp->stamps[index - 1].stamp);
	if (s != 0) {
		cnt++;
		secs = base * base_cnt;
		if (delta > secs) {
			err_cont(s, "***setaffinity tid %lu on C %d pass %.2fs", thread, cpu, secs);
			base_cnt++;
		}
		goto reset;
	} else {
		if (cnt > 1)
			err_write("(%d:+%d:%.4fs)", argp->index, cnt, delta);
		getaffinity(argp, &cpuset);
		err_write("{%d:C %d %lu}\n", argp->index, argp->cpu, thread);
	}
#if 0
	printf("==t%lu sleep\n", thread);
	/*
https://stackoverflow.com/questions/8032372/how-can-i-see-which-cpu-core-a-thread-is-running-in
	f to select last CPU field to make sure 4 threads are on expected CPU;
	*/
	pause();
#else
	argp->stamps[index++].stamp = gettime_ns();
	pthread_mutex_lock(&argp->l);
	//Never change, nor check, the predicate condition unless the mutex is locked. Ever.
	argp->local = UNLOCKED;
	pthread_mutex_unlock(&argp->l);
	pthread_cond_signal(&argp->c);
	pthread_mutex_lock(argp->lock);
	if (argp->pub[0] == LOCKED)
		pthread_cond_wait(argp->cond, argp->lock);
	pthread_mutex_unlock(argp->lock);
	argp->stamps[index++].stamp = gettime_ns();
	debug("$$[%d]starts loop\n", argp->index);
/*
android ver:
https://stackoverflow.com/questions/9287315/finding-usage-of-resources-cpu-and-memory-by-threads-of-a-process-in-android
ps -p -t -c 7624
x86 ver:
https://stackoverflow.com/questions/8032372/how-can-i-see-which-cpu-core-a-thread-is-running-in
*/
	for (i = 0; i < argp->loops; ) {
/*
1. why cpu_consumer is necessary:
when it is removed:
***Final 39999996 took 4.52s
after adding:
***Final 39999385 took 18.42s
2. If you run this arm binary on ubuntu, NOT a single error can emit!AL 4000 0000!
*/
		//cpu_consumer();
		if (mutex_lock(argp)) {
			glob++;
			mutex_unlock(argp);
			i++;
		}
	}
#endif
	argp->stamps[index++].stamp = gettime_ns();
	argp->stamps_nr = index;
	getaffinity(argp, &cpuset);
	debug("##[%d]:end loop\n", argp->index);
	return (void*)argp->index;
}

int main(int argc, char **argv)
{
	struct arg *args, *argp;
	struct stamp *stamps;
	int s, i, j, k, index;
	unsigned int delta;
	void *rval_ptr;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	int threads_nr, loops, public = LOCKED;
	threads_nr = CONFIG_CPU_NR;
	loops = CONFIG_PER_BATCH;

	while((s = getopt(argc, argv, "t:l:")) != -1) {
		switch(s)
		{
		case 't':
			threads_nr = atoi(optarg);
			break;
		case 'l':
			loops = atoi(optarg);
			break;
		default:
			break;
		}
	}
	printf("p %d t %lu: (%u loops X %d threads)\n", getpid(), pthread_self(), loops, threads_nr);
	if ((args = malloc(sizeof(struct arg) * threads_nr)) == NULL)
		err_exit(errno, "***fail to malloc for %d threads", threads_nr);
	if ((stamps = malloc(sizeof(struct stamp) * threads_nr)) == NULL)
		err_exit(errno, "***fail to malloc for %d stamps", threads_nr);
	//
	index = j = k = 0;
next:
	mutex_lock = mutexes[j][0];
	mutex_unlock = mutexes[j][1];
	printf("---%d: [%d]---\n", k, j);
	stamps[index++].stamp = gettime_ns();
	//
	for (i = 0; i < threads_nr; i++) {
		argp = args + i;
		argp->index = i;
		argp->local = LOCKED;
		argp->pub = &public;
		argp->loops = loops;
		argp->cpu = k? 0: (i % CONFIG_CPU_NR);
		argp->lock = &lock;
		argp->cond = &cond;
		pthread_mutex_init(&argp->l, NULL);
		pthread_cond_init(&argp->c, NULL);
		s = pthread_create(&argp->tid, NULL, threadFunc, argp);
		if (s != 0)
			err_exit(s, "***cannot create thread %d", i);
		else
			debug("%d.lunch %lu on CPU %d(expected)\n", argp->index, argp->tid, argp->cpu);
	}
	stamps[index++].stamp = gettime_ns();
	for (i = 0; i < threads_nr; i++) {
		argp = args + i;
		pthread_mutex_lock(&argp->l);
		/*
		pthread_cond_wait shall be called with @mutex locked by the calling thread
		or undefined behavior results
		*/
		if (argp->local == LOCKED)
			pthread_cond_wait(&argp->c, &argp->l);
		pthread_mutex_unlock(&argp->l);
	}
	pthread_mutex_lock(&lock);
	stamps[index++].stamp = gettime_ns();
	public = UNLOCKED;
	//pthread_cond_signal(&cond);//on x86 it can trigger [1,2] threads, on arm there seems al only 1 treads waiting first on signal
	pthread_cond_broadcast(&cond);
	stamps[index++].stamp = gettime_ns();
	debug("main fin signal\n");
	pthread_mutex_unlock(&lock);
	for (i = 0; i < threads_nr; i++) {
		argp = args + i;
		s = pthread_join(argp->tid, &rval_ptr);//if a joined tid is joined again:the following error will emit!
		if (s != 0)
			err_exit(s, "***join %lu fails", argp->tid);
		else
			debug("join %lu get %d\n", argp->tid, (int)rval_ptr);
	}
	stamps[index++].stamp = gettime_ns();
	/*
	printf("t%lu sleep\n", thread);
	pause();
	*/
	delta = threads_nr * loops - glob;
	if (delta != 0) {
		char buf[256];
		//ohm: One hundred millionth(percent of 10^8)
		int ret = snprintf(buf, sizeof(buf), "***%d(%.6f):", delta, (delta * 1.0)/(threads_nr * loops));
		write(STDERR_FILENO, buf, ret);
		//fprintf(stderr, );
	}
	printf("==>%d", glob);
	for (i = 0; i < index - 1; i++) {
		stamps[i].delta = DELTA(stamps[i + 1].stamp, stamps[i].stamp);
		printf("%.6f ", stamps[i].delta);
	}
	printf("\n");
	for (i = 0; i < threads_nr; i++)  {
		argp = args + i;
		printf("[%02d]:(%d)tid %lu:", i, argp->cpu, argp->tid);
		for (index = 0; index < argp->stamps_nr - 1; index++) {
			struct stamp *p = &argp->stamps[index];
			p->delta = DELTA(p[1].stamp, p[0].stamp);
			printf("%.6f ", p->delta);
		}
		printf("\n");
	}
	//
	glob = 0;
	if (++j < ARRAY_SIZE(mutexes))
		goto next;
	if (k++ < CONFIG_TEST_UP) {
		j = 0;
		goto next;
	}
	free(args);
	free(stamps);
	//
	return 0;
}
