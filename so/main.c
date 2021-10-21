/*
a. to make -lm can really link libm.so, all the following must be meet:
1.#include <math.h>
2.call a function in math.h. Besides, it must be really used i.e. unoptimized by gcc.
to make 1&2 convinent, MATH macro is used.
*/
#ifdef MATH
#include <math.h>
#include <stdlib.h>
#endif
#include "a.h"
#include "b.h"
int main(int argc, char **argv)
{
	int ret = 0;
#ifdef MATH
	ret = (int)sin(atoi(argv[1]));
#endif
	add(1,2);
	return ret;
}
