#include <unistd.h>//getopt
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	extern char *optarg;
	extern int optind, opterr, optopt;
	char c;
	int t;
	t = -1;
	int i;
	/*
	1. ...getopt() places a pointer to the following text in the same argv-element, or the text of the following argv-element, in optarg.
	e.g. c is included in optstring
	-cfoo optarg points to "foo" in the same argv-element
	-c foo optarg points to the following argv-element after "-c"
	2. some special cases in manual are not tested here, like optstring starts with +, -, :, or including "::"
	*/
	/*
	./getopt
	./getopt -a -b
	./getopt -ab<--equivalent to 2 optiosn:-a -b
	./getopt -c foo
	./getopt -cfoo
	./getopt arg1
	./getopt -a arg1
	./getopt -c foo arg1
	./getopt -a -- -b
	./getopt -a -
	./getopt - //"An element of argv that starts with '-' (and is not exactly "-" or "--") is an option element.", so '-' without anything is not an option and optind is 1!
	./getopt -t -n		//-n will be recognized as -t's parameter, "The variable optind is the index of the next element to be processed in argv"
	./getopt -n 9 -t 1
	./getopt -x
	./getopt -n -t	//when handle 't', '?' returned ("if it detects a missing option argument, it returns '?'")and optarg is 0("then it is returned in optarg, otherwise optarg is set to zero")
	./getopt -n -t 9 -z x -a 1 xx
	./getopt -n -t 9 -z x -a 1 xx yy
	./getopt -n -t 9 -z -a 1 xx
	./getopt -n -t 9 -z -a 0 -b 4 -c file
	./getopt -n -t 1 aa -x
	*/
	for (i = 0; i < argc; i++)
		printf("%s[%d]  ", argv[i], i);
	printf("\noptind=%d\n", optind);//optind is initialized to 1
	printf("\tc\toptind\toptarg\n");
	while ((c = getopt(argc, argv, "t:n")) != -1) {
		//the oprind will be the index of the next element to be processed in argv
		printf("opterr=%d %c(%c)\t%d\t[%6s]\t", opterr, c, c=='?'?optopt:'-', optind, optarg);
		for (i = 0; i < argc; i++)
			printf("%s[%d]  ", argv[i], i);
		printf("\n");
		switch(c)
		{
		case 't':
			t = atoi(optarg);
			//tflag = 1;
			break;
		case 'n':
			//nflag = 1;
			break;
		default:
			break;
#if 0
			printf("usage:%s [-t time] [-n] filename\n", argv[0]);
			exit(1);
#endif
		}
	}
	//when getopt return -1, optind will be the 1st nonoption's index
	printf("t=%d\t%d\t%d\t[%s]\t", t, c, optind, optarg);
	for (i = 0; i < argc; i++)
		printf("%s[%d]  ", argv[i], i);
	printf("\n");
	return 0;
}