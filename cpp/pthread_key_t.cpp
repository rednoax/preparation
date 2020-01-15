#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define report_func() printf("%s: %p\n", __PRETTY_FUNCTION__, this)

static pthread_mutex_t gTLSMutex = PTHREAD_MUTEX_INITIALIZER;
static bool gHaveTLS = false;
static pthread_key_t gTLS = 0;
//static bool gShutdown = false;

class IPCThreadState
{
public:
	static IPCThreadState *self();
private:
	IPCThreadState()
	{
		report_func();
		pthread_setspecific(gTLS, this);
	}
	~IPCThreadState()
	{
		report_func();
	}
	static void threadDestructor(void *st);
};


IPCThreadState * IPCThreadState::self()
{
	if (gHaveTLS) {
restart:
		const pthread_key_t k = gTLS;
		IPCThreadState *st = (IPCThreadState*)pthread_getspecific(k);
		printf("%s:pthread_getspecific %p\n", __func__, st);
		if (st) {
			return st;
		}
		return new IPCThreadState;
	}
	pthread_mutex_lock(&gTLSMutex);
	if (!gHaveTLS) {
		int key_create_value = pthread_key_create(&gTLS, threadDestructor);
		printf("%s: gTLS %d, return %d\n",__func__, gTLS, key_create_value);
		if (key_create_value) {
			pthread_mutex_unlock(&gTLSMutex);
			printf("%s unable to create TLS key, expect a crash %d\n", __func__, key_create_value);
			return NULL;
		}
		gHaveTLS = true;
	}
	pthread_mutex_unlock(&gTLSMutex);
	goto restart;
}

void IPCThreadState::threadDestructor(void * st)//called when return or pthread_exit, but not called when exit
{
	//report_func();
	IPCThreadState *const self = static_cast<IPCThreadState*>(st);
	printf("%s: delete %p\n", __func__, st);
	delete self;
}
void printids(const char *s)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("---%s: pid %d, tid %ld\n", s, pid, tid);
	IPCThreadState::self();
	IPCThreadState::self();
	printf("---%s: pid %d, tid %ld\n", s, pid, tid);
}

void* thr_fn(void *arg)
{
	printids("child thread:");
	return (void*)0;
}

int main()
{
	int err = 0;
	pthread_t ntid;
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (err != 0) {
		printf("cannot create thread: %d\n", err);
		exit(1);
	}
	printids("main thread:");
#if 10
	sleep(2);
	printf("sleep fin\n");
	pthread_exit(NULL);//return or exit will not call theadDestructor
#endif
	return 0;
}
/*
---main thread:: pid 2117, tid 140405459769152
self: gTLS 0, return 0<--main thread create pthread_key_t to 0
self:pthread_getspecific (nil)<--the 1st getspecific return NULL for no data is attached to main thread
IPCThreadState::IPCThreadState(): 0x55d74c1143b0<--new IPCThreadState, which is run via IPCThreadState's static function for who exists before IPCThreadState object
self:pthread_getspecific 0x55d74c1143b0<--the 2nd getspecific return NON-NULL for IPCThreadState instance has been attached to main thread
---main thread:: pid 2117, tid 140405459769152
---child thread:: pid 2117, tid 140405441603328<--child thread will not create pthread_key_t for main thread has created it
self:pthread_getspecific (nil)<--the 1st getspecific return NULL for no data is attached to child thread
IPCThreadState::IPCThreadState(): 0x7fb2a8000b20
self:pthread_getspecific 0x7fb2a8000b20<--the 2nd getspecific return NON-NULL for IPCThreadState instance has been attached to main thread
---child thread:: pid 2117, tid 140405441603328
threadDestructor: delete 0x7fb2a8000b20<--child thread's return will call specific data's dtor when specific data is NOT NULL
IPCThreadState::~IPCThreadState(): 0x7fb2a8000b20<---dtor's delete will trigger IPCThreadState's dtor
sleep fin
threadDestructor: delete 0x55d74c1143b0<--main thread's dtor is called for whose thread specific data!=NULL
IPCThreadState::~IPCThreadState(): 0x55d74c1143b0<--main thread's dtor threadDestructor's delete this will call IPCThreadState's dtor
*/
