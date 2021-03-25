#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int opt, nsecs;
	int nflags, tflags;
	nflags = tflags = nsecs = 0;
	while ((opt = getopt(argc, argv, "nt:")) != -1) {
		switch(opt) {
		case 'n':
			nflags = 1;
			break;
		case 't':
			nsecs = atoi(optarg);
			tflags = 1;
			break;
		default:
			/*
			'?':if there is no arg for -t, '?' is returned and optopt is set to 't'
			    or the option is not in the optstring
			*/
			fprintf(stderr, "Usage:%s [-t nsecs] [-n] name\n", argv[0]);
			exit(-1);
		}
	}
	printf("optind %d, nflags %d, tflags %d, nsecs %d\n", optind, nflags, tflags, nsecs);
	//the following is a very importand routine to test nonoption argument
	if (optind >= argc) {//optind is index of argv[], if ./a.out[enter], then optind is 1 here
		fprintf(stderr, "expect argument after options\n");
		exit(-1);
	}
	printf("name argument=%s\n", argv[optind]);
	return 0;
}
