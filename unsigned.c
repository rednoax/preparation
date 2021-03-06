/*
from gcc 4.6 manual:
The original ANSI C standard (X3.159-1989) was ratified in 1989 and published in 1990.
This standard was ratified as an ISO standard (ISO/IEC 9899:1990) later in 1990. There
were no technical differences between these publications, although the sections of the ANSI
standard were renumbered and became clauses in the ISO standard. This standard, in
both its forms, is commonly known as C89, or occasionally as C90, from the dates of
ratification. The ANSI standard, but not the ISO standard, also came with a Rationale...

The default, if no C language dialect options are given, is ‘-std=gnu90’; this will change to ‘-std=gnu99’ in
some future release when the C99 support is complete. 

from gcc 5.2 manual:
The default, if no C language dialect options are given, is ‘-std=gnu11’.

http://port70.net/~nsz/c/c89/c89-draft.html#3.1.3.2
Unsuffixed decimal: int, long int, unsigned long int; //int->long->ulong
unsuffixed octal or hexadecimal: int, unsigned int, long int, unsigned long int; //int->uint->long->ulong
suffixed by the letter u or U: unsigned int, unsigned long int; //uint->ulong
suffixed by the letter l or L: long int, unsigned long int; //long->ulong
suffixed by both the letters u or U and l or L: unsigned long int . //ulong

from C99's free manual n1256:
//int->long->long long
Unsuffixed decimal: int, long int, long long int; 
//int->uint->long->ulong->ll->ull
unsuffixed octal or hexadecimal: int, unsigned int, long int, unsigned long int, long long int, unsigned long long int;
//uint->ulong->ull
suffixed by the letter u or U: unsigned int, unsigned long int, unsigned long long int; 
//long->ll
decimal suffixed by the letter l or L: long int, long long int; 
//long->ulong->ll->ull
octal or hex suffixed by 'l' or 'L':long int, unsigned long int, long long int, unsigned long long int;
//ulong->ull
suffixed by both the letters u or U and l or L: unsigned long int, unsigned long long int
//ll
decimal suffixed by ll or LL:long long int
//ll->ull
octal or hex suffixed by ll or LL:long long int, unsigned long long int
//ull
decimal suffixed by both u or U and ll or LL:unsigned long long int
//ull
octal or hex suffixed by both u or U and ll or LL:unsigned long long int

usefull example:
1. http://stackoverflow.com/questions/13134956/what-is-the-reason-for-explicitly-declaring-l-or-ul-for-long-values
A common reason to use a suffix is ensuring that the result of a computation doesn't overflow. 
...
Another example is the comparison x < 12U where variable x has type int...

2. http://en.cppreference.com/w/cpp/language/integer_literal

3. http://stackoverflow.com/questions/2347936/cant-get-rid-of-this-decimal-constant-is-unsigned-only-in-iso-c90-warning
 The C90 Standard says

    The type of an integer constant is the first of the corresponding list in which its value can be represented. Unsuffixed decimal: int, long int, unsigned long int;

Your literal has no suffix (like u for unsigned or L for long). C99 has a different list: int, long int, long long int. Yet another rule has C++ (the current edition), which has the list: int, long int (current C++ has no long long int type). 
*/
#include <stdio.h>

typedef unsigned int u32;
typedef unsigned long long u64;
typedef long long s64;
//__NSEC_PER_JIFFY will be fffefce7 when the following macro is opened
#define NSEC_PER_SEC1 1000000000L// 0x3B9A CA00
//__NSEC_PER_JIFFY will be 18c42 when the following macro is opened
#define NSEC_PER_SEC2 1000000000U// 0x3B9A CA00

#define ACTHZ 25600
//#define __PART(MOD,DEN,LSH) ((((MOD)%(DEN))<<(LSH))+(DEN)/2)/(DEN)
#define NSEC_PER_JIFFY	((u32)((((u64)NSEC_PER_SEC1)<<8)/ACTHZ))//==10^9/HZ==10^7 //unit:ns/jiffies
#define __NSEC_PER_JIFFY(x)	((u32)(((x)<<8)/ACTHZ))//==10^9/HZ==10^7 //unit:ns/jiffies
#define FSEC_PER_SEC	1000000000000000L//should be LL!?

#define SH_DIV(NOM,DEN,LSH) (   (((NOM) / (DEN)) << (LSH))              \
                             + ((((NOM) % (DEN)) << (LSH)) + (DEN) / 2) / (DEN))

#define __PART2(NOM,DEN,LSH) ((((NOM) % (DEN)) << (LSH)) + (DEN) / 2)

/* TICK_NSEC is the time between ticks in nsec assuming real ACTHZ */
#define TICK_NSEC (SH_DIV (1000000UL * 1000, ACTHZ, 8))
/*
check the following 3 conditions
1. 10^9/25600:0x9896, is among [24:0]
2. 10^9%25600:0x3200, is among [24:0]
3. (10^9%25600)<<8+12800:0x323200, is among [31,0]
*/
#define __TICK_NSEC (SH_DIV (1000000L * 1000, ACTHZ, 8))//==TICK_NSEC, no difference
//~'s precedence is higher than >>or<<
#define ___LONG_MAX (~0)//int
#define __LONG_MAX ((~0)>>1)//int
#define _LONG_MAX (~0UL>>1)//long unsigned int
#define LONG_MAX ((long)(~0UL>>1))//long int
#define KTIME_MAX ((s64)(~((u64)1<<63)))//long long int//will be 7fff ffff ffff ffff
int main()
{
	int i = 0x9aca0000;
	long j = 0x9aca0000;
	printf("%ld, %lu, %u, %u, %llx\n", LONG_MAX, _LONG_MAX, __LONG_MAX, ___LONG_MAX, KTIME_MAX);
	/*
$ gcc -Wall unsigned.c
unsigned.c: In function ‘main’:
unsigned.c:89:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘unsigned int’ [-Wformat=]
  printf("%lx %x\n", 0xff000000 + 0x100, 0xff000000UL + 0x100UL);
  ^
unsigned.c:89:2: warning: format ‘%x’ expects argument of type ‘unsigned int’, but argument 3 has type ‘long unsigned int’ [-Wformat=]
unsigned.c:89:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘unsigned int’ [-Wformat=]
unsigned.c:89:2: warning: format ‘%x’ expects argument of type ‘unsigned int’, but argument 3 has type ‘long unsigned int’ [-Wformat=]
	*/
	//0xff00 0100 is uint, 0xff00 0100UL is ulong
	printf("%x %lx\n", 0xff000000 + 0x100, 0xff000000UL + 0x100UL);
	printf("%d %lu\n", 0xff000000 + 0x100, 0xff000000UL + 0x100UL);
	//1000000000 is int, when left shifted by 8, the result is still int so higher bits will disappear
	printf("%x %lx\n", (1000000000<<8)/25600, (1000000000UL<<8)/25600);//minus division then positive division
	printf("%x %lx\n", (1000000000<<8), (1000000000UL<<8));//no difference
	/*
	l or u cannot be used as typecast:0x9aca0000l/0x9aca0000u/0x9aca0000 are all unsinged, to do minus division, 
	use three way in the middle
	*/
	printf("%lx %x %lx %x %x\n", 0x9aca0000l/25600, ((int)0x9aca0000)/25600, j/25600, i/25600, 0x9aca0000U/25600);
	//division cares the minus!!!
	printf("%x %lx %llx\n", __NSEC_PER_JIFFY(NSEC_PER_SEC1), NSEC_PER_SEC1<<8, (u64)NSEC_PER_SEC1<<8);
	printf("%x %x %llx\n", __NSEC_PER_JIFFY(NSEC_PER_SEC2), NSEC_PER_SEC2<<8, (u64)NSEC_PER_SEC2<<8);

	printf("%x %x\n", ~0x9aca0000+1, ~0x10319+1);
	//for 32bit gcc(seems c90 default):what really matters are commented with "<------"
	
	//0xXX MSB set & with no suffix:unsigned int
	//0xXX MSB set & with 'u' suffix:unsigned int
	//0xXX MSB set & with 'l' suffix:long unsigned int
	//0xXX MSB set & with 'ul' suffix:long unsigned int
	//0xXX MSB set & with 'll' suffix:long long int
	//0xXX MSB set & with 'ull' suffix:long long unsigned int

	//0xXX MSB not set & with no suffix:int
	//0xXX MSB not set & with 'u' suffix:unsigned int	//<------
	//0xXX MSB not set & with 'l' suffix:long int
	//0xXX MSB not set & with 'ul' suffix:long unsigned int//<------
	//0xXX MSB not set & with 'll' suffix:long long int
	//0xXX MSB not set & with 'ull' suffix:long long unsigned int
	
	//a positive decimal value with MSB set & no suffix:unsigned
	//a positive decimal value with MSB set & 'u':unsigned int
	//a positive decimal value with MSB set & 'L':long unsigned int//<------'L' cannot change the decimal to "long int"!!!
	//a positive decimal value with MSB set & 'll':long long int//<------here MSB means bit 31
	//a positive decimal value with MSB set & 'ULL':long long unsigned int
	//a positive decimal value without MSB set & no suffix:int
	//a positive decimal value without MSB set & 'u':unsigned int//<------
	//a positive decimal value without MSB set & 'l':long int
	//a positive decimal value without MSB set & 'LL':long long int
	//a positive decimal value without MSB set & 'ULL':long long unsigned int
	//**suffix***
	//u:unsigned int
	//l:long int
	//**format**
	//lx:long unsigned int
	//x:unsigned int
	//d:int
	//ld:long int
	//u:unsigned int
	//lu:long unsigned int
	/*
$ gcc -Wall unsigned.c
unsigned.c: In function ‘main’:
unsigned.c:140:2: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long long unsigned int’ [-Wformat=]
  printf("%d\n", 2147483647ULL);//7FFF FFFF;
  ^
unsigned.c:140:2: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long long unsigned int’ [-Wformat=]
unsigned.c:142:2: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long long int’ [-Wformat=]
  printf("%d\n", 0x80000000LL);
  ^
unsigned.c:142:2: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long long int’ [-Wformat=]
	*/
	//2147483647ULL is ull
	printf("%llu\n", 2147483647ULL);//7FFF FFFF;
	//0x9aca0000 is uint
	printf("%d\n", 0x9aca0000);
	//0x8000 0000LL is long long
	printf("%lld\n", 0x80000000LL);
	/*
$ gcc -Wall unsigned.c -std=c99
unsigned.c: In function ‘main’:
unsigned.c:166:2: warning: format ‘%x’ expects argument of type ‘unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]
  printf("%x\n", 2147483648);//0x8000 0000
  ^
unsigned.c:166:2: warning: format ‘%x’ expects argument of type ‘unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]
	*/
	/*
$ gcc -Wall unsigned.c
unsigned.c: In function ‘main’:
unsigned.c:166:2: warning: this decimal constant is unsigned only in ISO C90 [enabled by default]
  printf("%x\n", 2147483648);//0x8000 0000
  ^
unsigned.c:166:2: warning: format ‘%x’ expects argument of type ‘unsigned int’, but argument 2 has type ‘long unsigned int’ [-Wformat=]
unsigned.c:166:2: warning: format ‘%x’ expects argument of type ‘unsigned int’, but argument 2 has type ‘long unsigned int’ [-Wformat=]
	*/
	/*
c99:6 If an integer constant cannot be represented by anytype in its list, it may have an
extended integer type, if the extended integer type can represent its value. If all of the
types in the list for the constant are signed, the extended integer type shall be signed. If
all of the types in the list for the constant are unsigned, the extended integer type shall be
unsigned. If the list contains both signed and unsigned types, the extended integer type
may be signed or unsigned. If an integer constant cannot be represented by anytype in
its list and has no extended integer type, then the integer constant has no type.
	
	redoah:gcc default option is C89/C90(?), but from C99's spec, the 1000000000000000L is not "long long long"
$ gcc -Wall unsigned.c
unsigned.c: In function ‘main’:
unsigned.c:182:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]
  printf("%lx\n", 1000000000000000L);//3 8D7E A4C6 8000
  ^
unsigned.c:182:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]
	*/
	/*
unsigned.c: In function ‘main’:
unsigned.c:76:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]
  printf("%lx\n", 1000000000000000);
  ^
unsigned.c:76:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]	
	*/
	/*
unsigned.c: In function ‘main’:
unsigned.c:83:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]
  printf("%lx\n", 1000000000000000LL);
  ^
unsigned.c:83:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long int’ [-Wformat=]	
	*/
	/*
unsigned.c: In function ‘main’:
unsigned.c:90:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long unsigned int’ [-Wformat=]
  printf("%lx\n", 1000000000000000u);
  ^
unsigned.c:90:2: warning: format ‘%lx’ expects argument of type ‘long unsigned int’, but argument 2 has type ‘long long unsigned int’ [-Wformat=]
	*/
	printf("%llx(%lld)\n", FSEC_PER_SEC, FSEC_PER_SEC);
	//>>,/,% care signed/unsigned
	printf("%d %d %d %d "
			"%u==%d %d(%d) %x-%x\n"
			"%x(%d) %lx-%x "
			"%x %x %lx "
			"%lx:%lx\n", 
			ACTHZ, NSEC_PER_JIFFY, sizeof(long), 1000000000%25600, 
			0x80000001, 0x80000001, 0x80000001%129, ((int)0x80000001)%129, (unsigned)0x80000000>>1, (int)0x80000000>>1,
			__NSEC_PER_JIFFY(NSEC_PER_SEC1), __NSEC_PER_JIFFY(NSEC_PER_SEC1), NSEC_PER_SEC1<<8, NSEC_PER_SEC2<<8, //leftshift is ALWAYS unsigned
			(((u32)NSEC_PER_SEC1)<<8)/25600, ((u32)(NSEC_PER_SEC1<<8))/25600, (NSEC_PER_SEC1<<8)/25600,
			TICK_NSEC, __TICK_NSEC);
	//the following __PART2 all beyond limit:		0xff<<24+ 0x780 0000, , and the final result of SH_DIV is wrong!
	printf("%d, %x\n",  SH_DIV(0xff, 0x0f000000, 24), __PART2(0xff, 0x0f000000, 24));
	printf("%u, %x\n",  SH_DIV(0xffu, 0x0f000000u, 24), __PART2(0xffu, 0x0f000000u, 24));
	printf("%ld, %lx\n",  SH_DIV(0xffl, 0x0f000000l, 24), __PART2(0xffl, 0x0f000000l, 24));
	/*
	the following __PART2 donnot beyond 64bits limit: 0xffll<<24 + 0x780 0000, and the final result of SH_DIV is right;
	Win8.1 calc.exe's result:(ffll<<24)/0x0f000000==0x11
	limits when no LL or ULL suffix:
	1. NOM/DEN fits in (32-LSH) bits
	2. NOM%DEN fits in (32-LSH) bits
	3. __PART2 fits in 32bits
	so use LL or ULL suffix in SH_DIV if one of the above limits are not meeted!!!
	*/
	printf("%lld, %llx\n",  SH_DIV(0xffll, 0x0f000000, 24), __PART2(0xffll, 0x0f000000, 24));
	printf("%llu, %llx\n",  SH_DIV(0xffull, 0x0f000000ull, 24), __PART2(0xffull, 0x0f000000ull, 24));	
	//
	printf("%x %x\n", ((unsigned int)0xa0000000)<<3, ((int)0xa0000000)<<3);
	return 0;
}