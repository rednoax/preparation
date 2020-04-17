#include <stdio.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
struct of_dev_auxdata {
	char *compatible;
};

static struct of_dev_auxdata msm8909_auxdata_lookup[] = {//ARRAY_SIZE:1, all zero, confirmed by dump
	{},
};

static struct of_dev_auxdata msm8909_auxdata_lookup2[] = {//ARRAY_SIZE:2, all zero, confirmed by dump
	{},{}
};

void dump(struct of_dev_auxdata *p, int array_size)
{
	int i;
	for (i = 0; i < array_size; i++)
		printf("[%d]: %p\n", i, p[i].compatible);
}

int main(int argc, char *argv[])
{
	printf("%ld %ld\n", ARRAY_SIZE(msm8909_auxdata_lookup), ARRAY_SIZE(msm8909_auxdata_lookup2));
	dump(msm8909_auxdata_lookup, ARRAY_SIZE(msm8909_auxdata_lookup));
	dump(msm8909_auxdata_lookup2, ARRAY_SIZE(msm8909_auxdata_lookup2));
	return 0;
}
