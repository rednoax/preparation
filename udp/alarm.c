#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <err.h>
#include <poll.h>

void handler(int sig)
{
	printf("%d\n", sig);
}
int main()
{
	int r;
	struct sigaction sa = {
		.sa_flags = 0,
		.sa_handler = handler,
	};
	sigaction(SIGALRM, &sa, 0);
	r = alarm(3);//will not block but launch a timer in 3s
	printf("alarm: %d\n", r);
	while (1) {
		r = poll(NULL, 0, -1);
		warn("p %d %d", r, errno);
	}
	return 0;
}

