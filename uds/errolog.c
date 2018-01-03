#include <sys/types.h>
#include <sys/socket.h>/*protocol family for communication*/
#include <syslog.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd, n;
	char line[1024];
	char buf[1024];
	/*
	communication domain/protocol family: AF_UNIX
	sock type: full-duplex byte streams	
	*/
#if 0
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
		;
#else
	//openlog("open.serv", LOG_PID, LOG_USER);
	/*
	openlog(ident, option, facility)
	@ident:label, program name(e.g. a.out) is used if it's NULL
	@option(LOG_PID) controls the operation of openlog and subsequent calls to syslog().(if LOG_PID is used, then
	 all subsequently syslog will record pid)
	@facility:establishs a default to be used if none is specified in subsequently calls to syslog(); it specifies
	 what type of program is logging the message.
	*/
	openlog("uds/server.c", LOG_PID, LOG_USER);
	/*
	leave 1B for '\0'
	*/
	fd = 0;
	while ((n = read(STDIN_FILENO, line, 8)) > 0) {
		line[n] = 0;
		sprintf(buf, "[%04d:%s]", n, line);
		syslog(LOG_INFO, buf);
		/*
		only one time's "0123456789abcdef" will consume 3 times:01234567 89abcdef '\n';
		it is the same as a regular file's reading, there will be no '\0' when reading
		*/
		if (++fd > 3)
			break;
	}
	printf("2nd round begin\n");
	fd = 0;
	while ((n = read(STDIN_FILENO, line, sizeof(line) - 1)) > 0) {
		line[n] = 0;
		sprintf(buf, "[%04d=>%s]", n, line);//a trailing new line is added form /var/log/syslog
		/*
		syslog(priority, fmt, ...) generates a log message, ..., a trailing new line may be added if needed.
		@priority:facility|level, if facility is not specified, it will use openlog's @facility
		*/
		/*
		LOG_INFO is level, facility will use LOG_USER specified by openlog
		*/
		syslog(LOG_INFO, buf);
	}
# if 0
	/*
	use 0123456789abcdef to test it and there are 3 batches:
	8B:01234567
	8B:89abcdef
	1B:'\n'
	no '\0' will be read!!!
	*/	
	while ((n = read(STDIN_FILENO, line, 8)) > 0) {
		line[n] = 0;
		sprintf(buf, "=%d:[%s]", n, line);
		write(STDOUT_FILENO, buf, strlen(buf));
	}
# endif
#endif
	return 0;
}