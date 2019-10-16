#include <stdio.h>
/*
USEAGE:gcc -m32 -Wall endptr.c, all .data in the following struct are AL:
1.occupy 0B in sizeof(struct devresX)
2.4B rather than 8B aligned
*/
struct devres0 {//sizeof==4
	char c;
	unsigned long long data[];//4B aligned & occupy 0B of sizeof(struct devres0)
};

struct devres1 {//sizeof==4
	int i;
	unsigned long long data[];//ditto
};

struct devres2 {//sizeof==8
	unsigned long long ull;
	unsigned long long data[];//ditto
};

int main()
{
	struct devres0 r0;
	struct devres0 r1;
	unsigned long long data[1];
/*
.data in (struct devresX) occupy 0B in sizeof(struct devresX), but .data is 4B aligned,
so the result is 4, 4, 8
*/
	printf("%d, %d, %d\n", sizeof(struct devres0), sizeof(struct devres1),
		sizeof(struct devres2));//4, 4, 8
/*
	printf("%d %d\n", sizeof(r0.data), sizeof(r1.data));//wrong:
endptr.c: In function 'main':
endptr.c:22:26: error: invalid application of 'sizeof' to incomplete type 'long long unsigned int[]'
  printf("%d %d\n", sizeof(r0.data), sizeof(r1.data));
                          ^
endptr.c:22:43: error: invalid application of 'sizeof' to incomplete type 'long long unsigned int[]'
  printf("%d %d\n", sizeof(r0.data), sizeof(r1.data));
                                           ^
*/
/*
data is a 32bit ptr because of "gcc -m32 ..." even it points to a ull
sizeof(data) is 8 for the pointed area is ull that is 4Bx2 when using "gcc -m32 ..."
*/
	printf("%p: %d\n", data, sizeof(data));//0xffec4750: 8
	return 0;
}
