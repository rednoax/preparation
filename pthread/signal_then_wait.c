#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

struct arg {
	pthread_cond_t *condp;
	pthread_mutex_t *mp;
	int flag;
};

static void debug(const char *fmt, ...)
{
	char buf[4096];
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsnprintf(buf, sizeof(buf), fmt, ap);
	write(STDERR_FILENO, buf, ret);
	va_end(ap);
}

void* func(void *_arg)
{
	struct arg *argp = _arg;
	int s = 8;
wait:
	pthread_mutex_lock(argp->mp);
	while (argp->flag == 0) {
		static int cnt = 0;
		debug("(%d:", cnt++);
		pthread_cond_wait(argp->condp, argp->mp);
		debug(")\n");
	}
	argp->flag = 0;
	pthread_cond_signal(argp->condp);
	pthread_mutex_unlock(argp->mp);
	debug("begin sleep %d\n", s);
	sleep(s);
	debug("end sleep\n");
	goto wait;
	return NULL;
}

int main()
{
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	int ret, i;
	pthread_t tid;
	struct arg arg = {
		&cond,
		&mutex,
		0,
	};
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
	/*
	It is a logical error to call pthread_cond_signal() before calling pthread_cond_wait():
	if the thread you signal is not blocked in pthread_cond_wait(), it will
	miss the event (pthread_cond_signal() does not queue up)
	*/
#if 0
	pthread_cond_signal(&cond);
	debug("signal==>\n");
	pthread_mutex_lock(&mutex);
	debug("<==before wait\n");
	pthread_cond_wait(&cond, &mutex);
	debug("<==fin wait\n");
	pthread_mutex_unlock(&mutex);
#endif
	ret = pthread_create(&tid, NULL, func, &arg);
	debug("create %lu: %d\n", tid, ret);
	pthread_mutex_lock(&mutex);
	arg.flag = 1;
	/*
	https://stackoverflow.com/questions/14924469/does-pthread-cond-waitcond-t-mutex-unlock-and-then-lock-the-mutex
	see its "Another way...": It is right to put pthread_cond_signal in critical area!
	*/
	pthread_cond_signal(&cond);
	while (arg.flag == 1) {
		static int cnt = 0;
		debug("{%d;", cnt++);
		pthread_cond_wait(&cond, &mutex);
		debug("}\n");
	}
	pthread_mutex_unlock(&mutex);
	debug("main start to signal 1000 times:\n");
	pthread_mutex_lock(&mutex);
	arg.flag = 0;
	for (i = 0; i < 1000; i++)
		pthread_cond_signal(&cond);
	debug("main fin to signal and clear flag\n");
	pthread_mutex_unlock(&mutex);
	pthread_join(tid, NULL);
	return 0;
}
