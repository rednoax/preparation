#include <stdio.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
struct of_dev_auxdata {
	char *compatible;
};

static struct of_dev_auxdata msm8909_auxdata_lookup[] = {
	{},
};

static struct of_dev_auxdata msm8909_auxdata_lookup2[] = {
	{},{}
};

int main(int argc, char *argv[])
{
	printf("%ld %ld\n", ARRAY_SIZE(msm8909_auxdata_lookup), ARRAY_SIZE(msm8909_auxdata_lookup2));
	return 0;
}
