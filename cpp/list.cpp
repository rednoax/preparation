#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <list>
#include <stdio.h>
#include <atomic>
#include <signal.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

class LogTimeEntry {
	static pthread_mutex_t timesLock;
public:
	LogTimeEntry()
	{
		printf("%s:%p\n", __func__, this);
	}
	LogTimeEntry(const LogTimeEntry &e)//if not const, a lot of mess emits
	{
		printf("%s:%p=>%p\n", __func__, &e, this);
	}
	~LogTimeEntry()
	{
		printf("%15s:%p\n", __func__, this);
	}
	static void lock(void)
	{
		pthread_mutex_lock(&timesLock);
	}
	static void unlock(void);
};

void LogTimeEntry::unlock(void)
{
	pthread_mutex_unlock(&timesLock);
}

pthread_mutex_t LogTimeEntry::timesLock = PTHREAD_MUTEX_INITIALIZER;
#if 1//namespace test
namespace wired{
typedef class LogTimeEntry ** LTEpp;
}
/*
list.cpp:22:1: error: ‘LTEpp’ does not name a type
 LTEpp ptr;
 ^
LTEpp ptr;
*/
wired::LTEpp pp_notused;//ok
//LTEpp pp_nu2;//wrong
using wired::LTEpp;
LTEpp pp_nu3;//ok
#endif
volatile unsigned long g_var = 0;
/*
if there is neither "using namespace std;" not std::atomic<unsigned long> varname;
error will emits:
rednoah@ubuntu:~/apue/cpp$ g++ -Wall -std=c++11 list.cpp -lpthread
list.cpp:21:1: error: ‘atomic’ does not name a type
 atomic<unsigned long> g_var2(0);
atomic<unsigned long> g_var2(0);
*/
#if 0
using namespace std;
atomic<unsigned long> g_var2(0);
#else
std::atomic<unsigned long> g_var2(0);
#endif
static void inc_global(void)
{
	unsigned int i;
	for (i = 0; i < 1000000UL; i++) {
		g_var++;
		g_var2++;
	}
}

void printtids(const char *s)
{
	int pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s\tpid %d, tid %ld(%lx)\n", s, pid, (unsigned long)tid, (unsigned long)tid);
}

void *reinit_thread_start(void *arg)
{
	printtids("new thread:");
	inc_global();
	return (void*)1;
}

typedef std::list<LogTimeEntry*> LastLogTimes;
void LogBuffer_init(void)
{
	LastLogTimes *times  = new LastLogTimes();
	LastLogTimes &mTimes = *times;
	LastLogTimes::iterator times_iter;
	LogTimeEntry::lock();
	printf("mTimes.begin() %s mTimes.end(), .end@%ld\n", mTimes.begin()==mTimes.end()?"==":"!=", std::distance(mTimes.end(), mTimes.begin()));
	for (times_iter = mTimes.begin(); times_iter != mTimes.end();times_iter++) {
	}
	LogTimeEntry::unlock();
	LogTimeEntry tmp;
	mTimes.push_back(&tmp);
	printf("end@%ld after push\n", std::distance(mTimes.end(), mTimes.begin()));
}

static sem_t reinit;
static bool reinit_running = false;

void* reinit_thread_start2(void *arg)
{
	int waitnr = 0;
	int ret;
	pthread_t tid = pthread_self();
	printf("%s tid %ld:start sem_wait\n", __func__, tid);
	/*
	sem_wait block if semaphore count is 0. It will not return until it has successfully decrement the semaphore count
	or is interrupted by a signal
	*/
	while(reinit_running && !(ret = sem_wait(&reinit)) && reinit_running) {
		waitnr++;
		printf("%s:after sem_wait, waitnr %d\n", __func__, waitnr);
	}
	/*
	sem_wait:
RETURN VALUE
       All of these functions return 0 on success; on error, the value of the semaphore is left unchanged, -1 is returned, and errno is set  to  indi‐
       cate the error.

ERRORS
       EINTR  The call was interrupted by a signal handler; see signal(7).
	*/
	printf("%s exit: waitnr %d, ret %d, errno %d\n", __func__, waitnr, ret, errno);
	return NULL;
}

void reinit_signal_handler(int signal)
{
	pthread_t tid = pthread_self();
	printf("%s:%ld get signal %d\n", __func__, tid, signal);
	/*
	If a process is blocked in a call to sem_wait (or sem_timedwait)when we call
	sem_post,the process is awakened and the semaphorecount that was just
	incremented by sem_post is decremented by sem_wait (or sem_timedwait).
	*/
	sem_post(&reinit);
}

void thread_test(void)
{
	pthread_attr_t attr;
	pthread_t thread;
	char buf[1024];
	/*
	when we want to use POSIX semaphores within a single process, it is easier to use
	unnamed semaphores. This only changes the way we create and destroy the semaphore.
	To create an unnamed semaphore, we call sem_init function.
	sem_init(sem_t *sem, int pshared, uint value)
	pshared:if multiple processes share it, set it to a nonzero value.
	value:specifies the initial value of the semaphore. 0 is locked.
	*/
	sem_init(&reinit, 0, 0);
	if (pthread_attr_init(&attr) || pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
		goto ERROR;
	reinit_running = true;
	if (pthread_create(&thread, &attr, reinit_thread_start2, NULL)) {
		reinit_running = false;
		goto ERROR;
	} else {
		signal(SIGHUP, reinit_signal_handler);
		printf("main PID %d, main thread %ld, new thread %ld\n", getpid(), pthread_self(), thread);
		while (fgets(buf, sizeof(buf), stdin) != NULL) {
			if (!strncmp(buf, "term", 4)) {
				reinit_running = false;
				sem_post(&reinit);
				sleep(2);
				break;
			}
			printf("%ld sema_post %ld\n", pthread_self(), thread);
			sem_post(&reinit);
		}
	}
	pthread_attr_destroy(&attr);
	return;
ERROR:
	perror("the thread error:");
	return;
}

int main()
{
	pthread_attr_t attr;
	void *ret;
	if (!pthread_attr_init(&attr) ) {//&& !pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
		pthread_t thread;
		if (!pthread_create(&thread, &attr, reinit_thread_start, NULL)) {
			printf("new thread %ld\n", (unsigned long)thread);
			printtids("main thread:");
			inc_global();
			printf("main thread fin\n");
			if (!pthread_join(thread, &ret)) {
				printf("thread exit code %ld, g_var %ld, g_var2 %ld\n", (long)ret, g_var, g_var2.load());
			}
			//pause();
		}
	}
	printf("before list template class instantiated...\n");
	std::list<LogTimeEntry> list;//no LogTimeEntry constructor will be called here since list store T* rather than T
	printf("after list template class instantiated...\n");
	LogTimeEntry time;
	printf("push back %p\n", &time);
	list.push_back(time);//list new its own LogTimeEntry and use copy constructor
	printf("push back fin\n");
	std::list<LogTimeEntry*> *p = new std::list<LogTimeEntry*>;
	delete p;
	p = new std::list<LogTimeEntry*>();//OK
	p->push_back(&time);//list will not construct T since T is (base*), just a pointer!
	delete p;
	printf("wait for other destruct...\n");
	thread_test();
	LogBuffer_init();
	return 0;
}