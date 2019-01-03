#include <pthread.h>
#include <stdio.h>
struct arg {
	pthread_cond_t *condp;
	pthread_mutex_t *mp;
};

void* func(void *_arg)
{
	struct arg *argp = _arg;
	pthread_cond_signal(argp->condp);
	return NULL;
}

int main()
{
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	int ret;
	pthread_t tid;
	struct arg arg = {
		&cond,
		&mutex,
	};
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
	/*
	It is a logical error to call pthread_cond_signal() before calling pthread_cond_wait():
	if the thread you signal is not blocked in pthread_cond_wait(), it will
	miss the event (pthread_cond_signal() does not queue up)
	*/
	pthread_cond_signal(&cond);
	printf("signal==>\n");
	pthread_mutex_lock(&mutex);
	printf("<==before wait\n");
	pthread_cond_wait(&cond, &mutex);
	printf("<==fin wait\n");
	pthread_mutex_unlock(&mutex);
	ret = pthread_create(&tid, NULL, func, &arg);
	printf("create %lu: %d\n", tid, ret);
	pthread_join(tid, NULL);
	
	
	return 0;
}
