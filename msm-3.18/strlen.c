#include <stdio.h>
#include <string.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
char *compatible[] = {"qcom,msm8917-qrd-sku5", "qcom,msm8917-qrd", "qcom,msm8917", "qcom,qrd"};
char * model[] = {"Qualcomm Technologies, Inc. MSM8917-PMI8937 QRD SKU5"};
#define ARRAY(x) {x, ARRAY_SIZE(x)}
struct array {
	char **p;
	int nr;
};
struct array tags[] = {
	ARRAY(compatible),
	ARRAY(model),
};
#define FDT_ALIGN(x, a) (((x) + (a) -1) & ~((a) - 1))
int main()
{
	int i, j;
	int len , len_aligned;
	for (i = 0; i < ARRAY_SIZE(tags); i++) {
		const struct array* tag = &tags[i];
		len = 0;
		for (j = 0; j < tag->nr; j++) {
			const char *str = tag->p[j];
			printf("%04d:%s %ld\n", j, str, strlen(str));
			len += strlen(str) + 1;
		}
#if 0		
		len_aligned = ((len + 3)/4)*4;
#else
		len_aligned = FDT_ALIGN(len, 4);
#endif
		printf("total len %d(0x%x)=>%d(0x%x)\n", len, len, len_aligned, len_aligned);
	}
	printf("(uint)~0ULL is %x\n", (unsigned int)~0ULL);
	return 0;
}