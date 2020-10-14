#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>

#define ARRAY_SIZE(x) sizeof(x)/(sizeof(x)[0])
static const char *tab = "inittab";
#if 0
rednoah@lucia:~/preparation/blob$ ./fgets.out
33[::sysinit:/etc/init.d/rcS S boot
]
0: [0, 33)
1: [0, 0)
2: [1, 1)
3: [2, 9)
4: [10, 33)
5: [0, 0)
38[::shutdown:/etc/init.d/rcS K shutdown
]
0: [0, 38)
1: [0, 0)
2: [1, 1)
3: [2, 10)
4: [11, 38)
5: [0, 0)
30[::askconsole:/bin/ash --login
]
0: [0, 30)
1: [0, 0)
2: [1, 1)
3: [2, 12)
4: [13, 30)
5: [0, 0)
rednoah@lucia:~/preparation/blob$
#endif
int main()
{
#define LINE_LEN	128
	FILE *fp = fopen(tab, "r");
	regex_t pat_inittab;
/*
1.Note that to return the offsets of N subexpression matches, nmatch must be at least N+1.
N==4 here so matches[5] is enough!
2.manual says Any unused structure elements will contain the value -1. But debug shows they are not touched by regexec()
*/
	regmatch_t matches[6] = {0};
	char *line = malloc(LINE_LEN);

	regcomp(&pat_inittab, "([a-zA-Z0-9]*):([a-zA-Z0-9]*):([a-zA-Z0-9]*):(.*)", REG_EXTENDED);
	while (fgets(line, LINE_LEN, fp)) {
		printf("%d[%s]\n", strlen(line), line);
		if (regexec(&pat_inittab, line, 5, matches, 0))
			printf("***");
		else {
			int i;
			printf("Parsing inittab - %s", line);			
			for (i = 0; i < ARRAY_SIZE(matches); i++) {
				printf("%d: [%d, %d)\n", i, matches[i].rm_so, matches[i].rm_eo);
			}
		}
	}
	fclose(fp);
	return 0;
}
