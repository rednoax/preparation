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
/*
void err_exit(int error, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
	exit(1);
}
*/
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

//static uint64_t glob = 0;//once use 64, no error happens!
static unsigned long glob = 0;

struct tRec {
	uint64_t stamp;
	double delta;
	const char *desp;
	int index;
};

struct trecs {
	struct tRec *parray;
	int used;
	int array_size;
};
struct arg {
	uint64_t loops;
	int cpu;
	pthread_mutex_t *lock;
	pthread_cond_t *cond;
	//int *my_lock;
	pthread_t tid;
	struct trecs *precs;
	//int stamps_nr;
	pthread_mutex_t l;
	pthread_cond_t c;
	int local, *pub;
	int index;
	unsigned long audit[2];
};
//0:unlock 1:locked
#define LOCKED 1
#define UNLOCKED 0
#define CONFIG_STAMPS_NR 16
volatile int my_lock = UNLOCKED;

typedef int (*mutex)(struct arg*);
int (*mutex_lock)(struct arg*);
int (*mutex_unlock)(struct arg*);

void trecs_dump(struct trecs *p)
{
	int i;
	struct tRec *rec = p->parray;
	for (i = 1; i < p->used; i++) {
		rec[i].delta = DELTA(rec[i].stamp, rec[i - 1].stamp);
		debug("%s:%.6f ", rec[i].desp, rec[i].delta);
	}
	debug("\n");
}

void RecTime(struct trecs *p, const char *str)
{
	struct tRec *recp;
	if (p->used < p->array_size) {
		recp = &p->parray[p->used];
		recp->stamp = gettime_ns();
		recp->desp = str;
		recp->index = p->used;
		/*
		if (p->used > 0)
			recp->delta = DELTA(recp->stamp, recp[-1].stamp);
		*/
		p->used++;
	}
}

struct trecs *trecs_init(int nr)
{
	int size;
	struct trecs *p = malloc(sizeof(*p));
	if (p == NULL) {
		err_write("***fail to malloc trecs");
		return NULL;
	}
	p->array_size = nr;
	p->used = 0;
	size = sizeof(p->parray[0]) * nr;
	if ((p->parray = malloc(size)) == NULL) {
		free(p);
		p = NULL;
		err_write("***fail to malloc for %d tRec", nr);
	} else
		memset(p->parray, 0, size);
	return p;
}

void trecs_free(struct trecs *p)
{
	if (p == NULL)
		return;
	if (p->parray)
		free(p->parray);
	free(p);
}

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
int spin_lock_nb(struct arg *argp)//spin way
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

int spin_lock_audit_nb(struct arg *argp)//spin with auditing
{
	int val, ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%3]\n"
"	cmp %0, %4\n"
	"	ldreq %0, [%2]\n"
	"	addeq %0, %0, #1\n"
	"	streq %0, [%2]\n"
"	beq 1b\n"
"	mov %0, %4\n"
"	strex %1, %0, [%3]\n"
"	cmp %1, #0\n"
	"	ldrne %0, [%2, #4]\n"
	"	addne %0, %0, #1\n"
	"	strne %0, [%2, #4]\n"
"	bne	1b\n"
	: "=&r" (val), "=&r" (ret), "=&r" (argp->audit)
	: "r" (&my_lock), "I"(LOCKED)
	: "cc");
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

int try_lock_nb(struct arg *argp)
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

int unlock_nb(struct arg *argp)
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

int spin_lock_dummy_nb(struct arg *argp)//alt spin way
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

int unlock_with_dummy_nb(struct arg *argp)
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


int unlock_with_nop_nb(struct arg *argp)
{
	__asm__ __volatile__(
"	nop\n"
"	str %1, [%0]\n"
"	nop\n"
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
	{try_lock_nb, unlock_with_dummy_nb},//w/o cpu_consumer:***46219(0.001155% 39953781<40000000)
	{try_lock_nb, unlock_nb},//w/o cpu_consumer:***4(0.000000% 159999996<160000000)
	//{spin_lock_dummy_nb, unlock_nb},//no error
	{try_lock_nb, unlock_with_nop_nb},//***172324(0.004308% 39827676<40000000)
	{spin_lock_audit_nb, unlock_with_nop_nb},
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
			err_write("***{%d:expect C %d,but on C ", argp->index, cpu);
			for (i = 0; i < CONFIG_CPU_NR; i++) {
				if (CPU_ISSET(i, &real)) {
					argp->cpu = i;
					err_write(":%d", i);
				}
			}
			err_write("}");
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
	uint64_t i;
	unsigned int s, base_cnt = 1, cpu = argp->cpu;
	pthread_t thread = pthread_self();
	int cnt = 1;
	cpu_set_t cpuset;
	struct trecs *trecsp = argp->precs;

	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	RecTime(trecsp, "start");
reset:
	s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	delta = DELTA(gettime_ns(), trecsp->parray[trecsp->used - 1].stamp);
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
	RecTime(trecsp, "b");
	pthread_mutex_lock(&argp->l);
	//Never change, nor check, the predicate condition unless the mutex is locked. Ever.
	argp->local = UNLOCKED;
	pthread_mutex_unlock(&argp->l);
	pthread_cond_signal(&argp->c);
	pthread_mutex_lock(argp->lock);
	if (argp->pub[0] == LOCKED)
		pthread_cond_wait(argp->cond, argp->lock);
	pthread_mutex_unlock(argp->lock);
	RecTime(trecsp, "w");
	debug("$$%d:starts loop\n", argp->index);
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
	RecTime(trecsp, "L");
	getaffinity(argp, &cpuset);
	debug("##[%d]:end loop\n", argp->index);
	return (void*)argp->index;
}

int main(int argc, char **argv)
{
	struct arg *args, *argp;
	struct trecs *trecsp = NULL;
	int s, i, j, k;
	void *rval_ptr;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	int public = LOCKED;
	unsigned int threads_nr = CONFIG_CPU_NR;
	uint64_t loops = CONFIG_PER_BATCH, delta;

	while((s = getopt(argc, argv, "t:l:")) != -1) {
		switch(s)
		{
		case 't':
			threads_nr = atoi(optarg);
			break;
		case 'l':
			loops = strtoull(optarg, NULL, 10);
			break;
		default:
			break;
		}
	}
	printf("p %d t %lu: (%llu loops X %u threads)\n", getpid(), pthread_self(), loops, threads_nr);
	if ((args = calloc(threads_nr, sizeof(struct arg))) == NULL) {
		err_write("***fail to malloc for %u threads", threads_nr);
		goto end;
	}
	//
	if ((trecsp = trecs_init(CONFIG_STAMPS_NR)) == NULL)
		goto end;
	//
	j = k = 0;
next:
	mutex_lock = mutexes[j][0];
	mutex_unlock = mutexes[j][1];
	printf("---%d: [%d]---\n", k, j);
	RecTime(trecsp, "");
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
		argp->precs = trecs_init(CONFIG_STAMPS_NR);
		memset(argp->audit, 0, sizeof(argp->audit)));
		if (argp->precs == NULL)
			goto end;
		pthread_mutex_init(&argp->l, NULL);
		pthread_cond_init(&argp->c, NULL);
		s = pthread_create(&argp->tid, NULL, threadFunc, argp);
		if (s != 0) {
			err_cont(s, "***cannot create thread %d", i);
			goto end;
		} else
			debug("%d.lunch %lu on CPU %d(expected)\n", argp->index, argp->tid, argp->cpu);
	}
	RecTime(trecsp, "L");
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
	RecTime(trecsp, "W");
	pthread_mutex_lock(&lock);
	public = UNLOCKED;
	//pthread_cond_signal(&cond);//on x86 it can trigger [1,2] threads, on arm there seems al only 1 treads waiting first on signal
	pthread_cond_broadcast(&cond);
	debug("main fin signal\n");
	RecTime(trecsp, "B");
	pthread_mutex_unlock(&lock);
	for (i = 0; i < threads_nr; i++) {
		argp = args + i;
		s = pthread_join(argp->tid, &rval_ptr);//if a joined tid is joined again:the following error will emit!
		if (s != 0)
			err_cont(s, "***join %lu fails", argp->tid);
		else
			debug("join %lu get %d\n", argp->tid, (int)rval_ptr);
	}
	RecTime(trecsp, "J");
	/*
	printf("t%lu sleep\n", thread);
	pause();
	*/
	delta = threads_nr * loops - glob;
	if (delta != 0ULL) {
		char buf[256];
		//ohm: One hundred millionth(percent of 10^8)
		int ret = snprintf(buf, sizeof(buf), "***%llu(%.6f%% %lu<%llu)", \
			delta, (delta * 1.0)/(threads_nr * loops), glob, threads_nr * loops);
		write(STDERR_FILENO, buf, ret);
		//fprintf(stderr, );
	} else
		err_write("====>OK:%lu:", glob);
	trecs_dump(trecsp);
	for (i = 0; i < threads_nr; i++)  {
		argp = args + i;
		debug("[%d:C %d:", i, argp->cpu);
		trecs_dump(argp->precs);
	}
	//
	glob = 0;
	if (++j < ARRAY_SIZE(mutexes))
		goto next;
	if (k++ < CONFIG_TEST_UP) {
		j = 0;
		goto next;
	}
end:
	if (args) {
		for (i = 0; i < threads_nr; i++)  {
			argp = args + i;
			trecs_free(argp->precs);
		}
	}
	free(args);
	trecs_free(trecsp);
	//
	return 0;
}
