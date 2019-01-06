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
//#define CONFIG_TEST_UP 0
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
static unsigned long glob[2] = {0, 0};

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

int fake_spin_lock_nb(struct arg *argp)//fake spin lock
{
	volatile int val, ret = 1;
	__asm__ __volatile__(
"1:	ldrex %0, [%2]\n"
"	cmp %0, %3\n"
"	beq 2f\n"
"	mov %0, %3\n"
"	strex %1, %0, [%2]\n"
"2:\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "I"(LOCKED)
	: "cc");
	return !ret;
}


int fake_spin_lock_NoDummyBetweenLDREXAndSTREX_nb(struct arg *argp)
{
	volatile int val, ret = 1;
	__asm__ __volatile__(
"1:	ldrex %0, [%2]\n"
"	cmp %0, %3\n"
"	beq 2f\n"
"	#mov %0, %3\n"
"	strex %1, %3, [%2]\n"
"2:\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED)
	: "cc");
	return !ret;
}

int fake_spin_lock_NoDummyBetweenLDREXAndSTREX_nb_v0(struct arg *argp)
{
	volatile int val, ret;
	__asm__ __volatile__(
"1:	ldrex %1, [%2]\n"
"	cmp %1, %3\n"
"	beq 2f\n"
"	#mov %0, %3\n"
"	strex %1, %3, [%2]\n"
"2:\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED)
	: "cc");
	return !ret;
}

int fake_spin_lock_NoDummyBetweenLDREXAndSTREX_shrinked_nb(struct arg *argp)
{
	volatile int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 2f\n"
"	strex %0, %2, [%1]\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED)
	: "cc");
	return !ret;
}

void audit_consumer(unsigned long *l)
{
	unsigned long local;
	local = *l;
	local++;
	*l = local;
	cpu_consumer();
}

void audit(unsigned long *l)
{
	unsigned long local;
	local = *l;
	local++;
	*l = local;
}


int audit_verbose(unsigned long *l, int r1, int r2, int r3)
{
	unsigned long local;
	local = *l;
	local++;
	*l = local;
	if (local %1)
		return 1;
	else
		return 0;
}

int spin_lock_audit_nb(struct arg *argp)//spin with auditing
{
	int val, ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%2]\n"
"	cmp %0, %3\n"
	"	ldreq %0, [%4]\n"
	"	addeq %0, %0, #1\n"
	"	streq %0, [%4]\n"
"	beq 1b\n"
"	mov %0, %3\n"
"	strex %1, %0, [%2]\n"
"	cmp %1, #0\n"
	"	ldrne %0, [%4, #4]\n"
	"	addne %0, %0, #1\n"
	"	strne %0, [%4, #4]\n"
"	bne	1b\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "I"(LOCKED), "r" (argp->audit)
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


int try_lock_cpu_consumer_nb(struct arg *argp)
{
	int ret;
	cpu_consumer();
	__asm__ __volatile__(
"1: ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 2f\n"
"	strex %0, %3, [%1]\n"
"2:	#dmb\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED)
	: "cc");
	return !ret;
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

int spin_lock_bl_nb(struct arg *argp)//spin with auditing
{
	int val, ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%2]\n"
"	cmp %0, %3\n"
"	bne 2f\n"
	"	push {r0, r1, r2, r3, r12, lr}\n"
	"	mov r0, %4\n"
	"	bl audit\n"
	"	pop {r0, r1, r2, r3, r12, lr}\n"
	"	b 1b\n"
"2:	mov %0, %3\n"
"	strex %1, %0, [%2]\n"
"	cmp %1, #0\n"
"	beq	3f\n"
	"	push {r0, r1, r2, r3, r12, lr}\n"
	"	mov r0, %4\n"
	"	add r0, r0, #4\n"
	"	bl audit\n"
	"	pop {r0, r1, r2, r3, r12, lr}\n"
	"	b 1b\n"
"3:\n"
	: "=&r" (val), "=&r" (ret)
	: "r" (&my_lock), "I"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
}

int spin_lock_simplified_nb(struct arg *argp)//spin way based on try lock way that can emit much error
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 1b\n"
"	strex %0, %3, [%1]\n"
"	cmp %0, #0\n"
"	bne 1b\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
}


int spin_lock_simplified_nop_nb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"		nop\n"
"1:	ldrex %0, [%1]\n"
"		nop\n"
"	cmp %0, %2\n"
"		nop\n"
"	bne 1b\n"
"		nop\n"
"	strex %0, %3, [%1]\n"
"		nop\n"
"	cmp %0, #0\n"
"		nop\n"
"	bne 1b\n"
"		nop\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
}

int spin_lock_simplified_bl_nb(struct arg *argp)//spin way based on try lock way that can emit much error
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 1b\n"
"	strex %0, %3, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %4\n"
		"	add r0, r0, #4\n"
		"	bl audit\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
}

int spin_lock_more_simple_bl_nb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	strex %0, %2, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %3\n"
		"	add r0, r0, #4\n"
		"	bl audit\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
}

/*
use this dmb lock with unlock_with_nop_nb cannot save the world:
./smp_mb.out -t 8 -l 20000000 -m 10000000
when 120 batch has been finished, there are 12 times error:
---------
Search "0\.000.*%" (12 hits in 1 file)
  new 2 (12 hits)
	Line 14: 	Line 1146: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 31: 	Line 2785: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 37: 	Line 3297: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 40: 	Line 3501: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 44: 	Line 3808: ***2(0.000000% 159999998<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 72: 	Line 6572: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 86: 	Line 7903: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 90: 	Line 8211: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 98: 	Line 8926: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 106: 	Line 9643: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 116: 	Line 10564: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
	Line 132: 	Line 12099: ***1(0.000000% 159999999<160000000)L:0.001816 W:29.463289 B:0.000147 J:29.357800 :0.000685 L:0.001560 W:0.129561 B:0.000020 J:32.986162 :0.000692 L:0.001160 W:0.031707 B:0.000023 J:29.061855 :0.000761
*/
int spin_lock_more_more_simple_bl_dmb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	strex %0, %2, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %3\n"
		"	add r0, r0, #4\n"
		"	bl audit\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:	nop\n"
"	nop\n"
"	nop\n"
"	dmb\n"
	: "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return 1;
}


int spin_lock_more_more_simple_bl_nb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	strex %0, %2, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %3\n"
		"	add r0, r0, #4\n"
		"	bl audit\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return 1;
}

int spin_lock_more_more_simple_bl_nb_v0(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	strex %0, %2, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %3\n"
		"	bl audit\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return 1;
}

int spin_lock_more_more_simple_bl_nb_v1(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	strex %0, %2, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %3\n"
		"	add r0, r0, #4\n"
		"	mov r1, %0\n"
		"	mov r2, %1\n"
		"	mov r3, %2\n"
		"	bl audit_verbose\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return 1;
}

int spin_lock_more_more_simple_nb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 1b\n"
"	strex %0, %2, [%1]\n"
"	cmp %0, #0\n"
"	bne 1b\n"
	: "=&r" (ret)
	: "r" (&my_lock), "r"(LOCKED)
	: "cc");
	return 1;
}

int spin_lock_simplified_bl_cpu_consumer_nb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 1b\n"
"	strex %0, %3, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %4\n"
		"	add r0, r0, #4\n"
		"	bl audit_consumer\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
}

int spin_lock_simplified_pushpop_nb(struct arg *argp)//spin way based on try lock way that can emit much error
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 1b\n"
"	strex %0, %3, [%1]\n"
"	cmp %0, #0\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
}

int spin_lock_simplified_bl_more_nb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	beq 2f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %4\n"
		"	add r0, r0, #0\n"
		"	bl audit\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"2:	strex %0, %3, [%1]\n"
"	cmp %0, #0\n"
"	beq 3f\n"
		"	push {r0, r1, r2, r3, r12, lr}\n"
		"	mov r0, %4\n"
		"	add r0, r0, #4\n"
		"	bl audit\n"
		"	pop {r0, r1, r2, r3, r12, lr}\n"
"	b 1b\n"
"3:\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED), "r" (argp->audit)
	: "cc");
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

int unlock_with_nop_dmb(struct arg *argp)
{
	__asm__ __volatile__(
	"dmb");
	__asm__ __volatile__(
"	nop\n"
"	str %1, [%0]\n"
"	nop\n"
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

int unlock_with_inc_dec_nb_v0(struct arg *argp)
{
	__asm__ __volatile__(
"	nop\n"
"	ldr %1, [%2]\n"
"	add %1, %1, #1\n"
"	str %1, [%2]\n"
"	ldr %1, [%2]\n"
"	sub %1, %1, #1\n"
"	str %1, [%2]\n"
"	mov %1, %3\n"
"	str %1, [%0]\n"
"	nop\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED), "r" (glob), "I"(UNLOCKED)
	: "cc");
	return 0;
}

int unlock_with_inc_dec_nb_v1(struct arg *argp)
{
	int local = UNLOCKED;
	__asm__ __volatile__(
"	nop\n"
"	ldr %1, [%2]\n"
"	add %1, %1, #1\n"
"	str %1, [%2]\n"
"	ldr %1, [%2]\n"
"	sub %1, %1, #1\n"
"	str %1, [%2]\n"
"	str %4, [%0]\n"
"	nop\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED), "r" (glob), "I"(UNLOCKED), "r"(local)
	: "cc");
	return 0;
}


int unlock_with_inc_dec_nb_v2(struct arg *argp)
{
	int local = UNLOCKED;
	__asm__ __volatile__(
"	nop\n"
"	ldr %1, [%2]\n"
"	add %1, %1, #0xff\n"
"	str %1, [%2]\n"
"	nop\n"
"	ldr %1, [%2]\n"
"	sub %1, %1, #0xff\n"
"	str %1, [%2]\n"
"	nop\n"
"	str %4, [%0]\n"
"	nop\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED), "r" (glob), "I"(UNLOCKED), "r"(local)
	: "cc");
	return 0;
}

int unlock_with_inc_dec_nb_v3(struct arg *argp)
{
	int local = UNLOCKED;
	__asm__ __volatile__(
"	nop\n"
"	ldr %1, [%2]\n"
"	add %1, %1, #0xff\n"
"	str %1, [%2]\n"
"	ldr %1, [%2]\n"
"	sub %1, %1, #0xff\n"
"	str %1, [%2]\n"
"	str %4, [%0]\n"
"	nop\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED), "r" (glob), "I"(UNLOCKED), "r"(local)
	: "cc");
	return 0;
}

int unlock_with_inc_dec_nb(struct arg *argp)
{
	glob[0]++;
	glob[0]--;
	glob[0]--;
	glob[0]++;
	__asm__ __volatile__(
"	nop\n"
"	str %1, [%0]\n"
"	nop\n"
	:
	: "r" (&my_lock), "r"(UNLOCKED)
	: "cc");
	return 0;
}

int spin_lock_simplified_tuned_nop_nb(struct arg *argp)
{
	int ret;
	__asm__ __volatile__(
"		nop\n"
"1:	ldrex %0, [%1]\n"
"	cmp %0, %2\n"
"	bne 1b\n"
"	strex %0, %3, [%1]\n"
"		nop\n"
"	cmp %0, #0\n"
"		nop\n"
"	bne 1b\n"
"		nop\n"
	: "=&r" (ret)
	: "r" (&my_lock), "I"(UNLOCKED), "r"(LOCKED), "r" (argp->audit)
	: "cc");
	return !ret;
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
# if 0
	//{broken_lock_v5, broken_unlock_v5p7},//Final 40000000 took 4278.33s
	{try_lock_nb, unlock_with_dummy_nb},//w/o cpu_consumer:***46219(0.001155% 39953781<40000000)
	{try_lock_nb, unlock_nb},//w/o cpu_consumer:***4(0.000000% 159999996<160000000)
	//{spin_lock_dummy_nb, unlock_nb},//no error
	{try_lock_nb, unlock_with_nop_nb},//***172324(0.004308% 39827676<40000000)
	//{spin_lock_audit_nb, unlock_with_nop_nb},//no error
# endif
	//{spin_lock_bl_nb, unlock_with_nop_nb},//can get error even when glob is 32bits, but -t 8 should be used; 4 times error when 64billion(0.8 billion x 8 threads) in 64bits's glob
	//{try_lock_cpu_consumer_nb, unlock_nb},//no more error than {try_lock_nb, unlock_nb}
	//{spin_lock_simplified_nb, unlock_with_nop_nb},//**51412(0.001285% 39948588<40000000)
	//{spin_lock_simplified_nb, unlock_nb},//not easy to emit error
	//{spin_lock_simplified_nb, unlock_with_dummy_nb},//less than {spin_lock_simplified_nb, unlock_with_nop_nb}
	//{spin_lock_simplified_nop_nb, unlock_with_nop_nb},//***10534(0.000263% 39989466<40000000)
	//{spin_lock_simplified_bl_nb, unlock_with_nop_nb},//***537906(0.013448% 39462094<40000000)
	//{fake_spin_lock_nb, unlock_with_nop_nb},//hard to emit error
	//{spin_lock_more_simple_bl_nb, unlock_with_nop_nb},//***510804(0.012770% 39489196<40000000)
	//{spin_lock_simplified_bl_more_nb, unlock_with_nop_nb}//***455747(0.011394% 39544253<40000000)
	//{spin_lock_simplified_pushpop_nb, unlock_with_nop_nb},//***94411(0.002360% 39905589<40000000)
	//{spin_lock_simplified_tuned_nop_nb, unlock_with_nop_nb},//***36166(0.000904% 39963834<40000000)
	//{spin_lock_simplified_bl_cpu_consumer_nb, unlock_with_nop_nb},//***1818(0.000045% 39998182<40000000)
	//{fake_spin_lock_NoDummyBetweenLDREXAndSTREX_nb, unlock_with_nop_nb},//hardly no error
//{fake_spin_lock_NoDummyBetweenLDREXAndSTREX_nb_v0, unlock_with_nop_nb},//hardly no error
	//{fake_spin_lock_NoDummyBetweenLDREXAndSTREX_shrinked_nb, unlock_with_nop_nb},//***171203(0.004280% 39828797<40000000)
	{spin_lock_more_more_simple_bl_nb, unlock_with_nop_nb},//***677259(0.016931% 39322741<40000000)
	//{spin_lock_more_more_simple_bl_dmb, unlock_with_nop_nb},//1/10 batch can emit error
	//{spin_lock_more_more_simple_bl_nb, unlock_with_nop_dmb},//cannot emit error after mass test
	//{spin_lock_more_more_simple_bl_nb, unlock_with_inc_dec_nb},//***25096(0.000627% 39974904<40000000)
	//{spin_lock_more_more_simple_bl_nb, unlock_with_inc_dec_nb_v0}//***398547(0.009964% 39601453<40000000)
	{spin_lock_more_more_simple_bl_nb, unlock_with_inc_dec_nb_v1},//***194(0.000005% 39999806<40000000)
	//{spin_lock_more_more_simple_bl_nb, unlock_with_inc_dec_nb_v2},//***18446744073709545223(461168601842.738647% 40006393<40000000)
	{spin_lock_more_more_simple_bl_nb, unlock_with_inc_dec_nb_v3}
	//{spin_lock_more_more_simple_bl_nb_v0, unlock_with_nop_nb},//**265650(0.006641% 39734350<40000000)
	//{spin_lock_more_more_simple_bl_nb_v1, unlock_with_nop_nb}//***673290(0.016832% 39326710<40000000)
	//{spin_lock_more_more_simple_nb, unlock_with_nop_nb},//***209601(0.005240% 39790399<40000000)
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
			//err_cont(s, "***setaffinity tid %lu on C %d pass %.2fs(%d)", thread, cpu, secs, base_cnt);
			err_cont(s, "***%d:setaffinity tid %lu on C %d pass %.2fs", argp->index, thread, cpu, secs);
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
		if (mutex_lock(argp)) {
			glob[0]++;
#if 0//seems more error can emit after removing these higer writing
			if (glob[0] == ~0UL) {
				glob[1]++;
				glob[0] = 0;
			}
#endif
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
	int s, i, j, k, multi = 1, cpu = -1;
	void *rval_ptr;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	int public = LOCKED;
	unsigned int threads_nr = CONFIG_CPU_NR;
	uint64_t loops = CONFIG_PER_BATCH, delta,*pglob = (uint64_t*)glob;

	while((s = getopt(argc, argv, "t:l:m:c:")) != -1) {
		switch(s)
		{
		case 't':
			threads_nr = atoi(optarg);
			break;
		case 'l':
			loops = strtoull(optarg, NULL, 10);
			break;
		case 'm':
			multi = atoi(optarg);
			break;
		case 'c':
			cpu = atoi(optarg);
			break;
		default:
			break;
		}
	}
	printf("p %d t %lu: (%llu loops X %u threads), CPU:%d(%d times)\n", getpid(), pthread_self(), loops, threads_nr, cpu, multi);
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
		argp->cpu = (cpu >= 0) ? cpu: (i % CONFIG_CPU_NR);
		argp->lock = &lock;
		argp->cond = &cond;
		argp->precs = trecs_init(CONFIG_STAMPS_NR);
		memset(argp->audit, 0, sizeof(argp->audit));
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
	delta = threads_nr * loops - *pglob;
	if (delta != 0ULL) {
		char buf[256];
		//ohm: One hundred millionth(percent of 10^8)
		int ret = snprintf(buf, sizeof(buf), "***%llu(%.6f%% %llu<%llu)", \
			delta, (delta * 1.0)/(threads_nr * loops), *pglob, threads_nr * loops);
		write(STDERR_FILENO, buf, ret);
		//fprintf(stderr, );
	} else
		err_write("====>OK:%llu:", *pglob);
	trecs_dump(trecsp);
	for (i = 0; i < threads_nr; i++)  {
		argp = args + i;
		debug("[%d:C %d:%lu %lu:", i, argp->cpu, argp->audit[0], argp->audit[1]);
		trecs_dump(argp->precs);
	}
	//
	*pglob = 0;
	if (++j < ARRAY_SIZE(mutexes))
		goto next;
	if (++k < multi) {
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
