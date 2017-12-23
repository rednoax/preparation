#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
//https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html

int main(int argc, char **argv)
{
	int aflags, bflags, index, c;
	char *cvalue = NULL;
	aflags = bflags = 0;
	
	while((c = getopt(argc, argv, "abc:")) != -1) {
		switch(c) {
		case 'a':
			aflags = 1;
			break;
		case 'b':
			bflags = 1;
			break;
		case 'c':
			cvalue = optarg;
			break;
		case '?'://default should be OK since there is only 4 possiblities:'abc?'
			if (optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			exit(-1);
		default:
			fprintf(stderr, "REALLY IMPOSSIBLE!!!\n");
			exit(-1);
		}
	}
	printf("optind=%d, aflags=%d, bflags=%d, cvalue=%s\n", optind, aflags, bflags, cvalue);	
	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);
	return 0;
}
