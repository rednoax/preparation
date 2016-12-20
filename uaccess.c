#include <stdlib.h>
#include <stdio.h>

unsigned long addr_limit = 0xbf000000;
int __range_ok(const char *addr, unsigned long size)
{
	unsigned long flag, roksum;
	/*
	-Wall -static -O2's output:
00010318 <__range_ok>:
   10318:       e59f3014        ldr     r3, [pc, #20]   ; 10334 <__range_ok+0x1c>
   1031c:       e5933000        ldr     r3, [r3]
   10320:       e0902001        adds    r2, r0, r1
   10324:       30d22003        sbcscc  r2, r2, r3
   10328:       33a03000        movcc   r3, #0
   1032c:       e1a00003        mov     r0, r3
   10330:       e12fff1e        bx      lr
   10334:       00028094        muleq   r2, r4, r0
......
00028094 <addr_limit>:
   28094:       bf000000        svclt   0x00000000

	addr:r0
	size:r1
	roksum:r2
	flag/addr_limit:r3

	flag = addr_limit;
	roksum = addr + size;
	//adds Rd,Rn,shifter_operand
	//C FLAG = CarryFrom(Rn+shifter_operand):1 when the addition caused a carry(true result is bigger than 2^32-1, where
	//the operands are treated as unsigned integers), and return 0 in all other cases.
	//###1.adding, CS:carry is set; CC:no carry when adding
	if (((unsigned long long)addr + size) > ((1ULL<<32)-1))
		;//CS i.e carry set
	else {
		//adds's CC branch
		//sbcs Rd, Rn, shifter_operand
		//Rd = Rn - shifter_operand - NOT(C Flag), since it's sbcscc, C Flag is 0 in the above adds, so:
		//Rd = Rn - shifter_operand - 1
		//C Flag = Not BorrowFrom(Rn - shifter_operand - NOT(C Flag))
		//BorrowFrom:1 if the substraction specified as its parameter caused a borrow(the true result is < 0, where the
		//operands are treated as unsigned integers), and return 0 in other cases.
		//###2.sub/sbc:CC:borrow,i.e unsigned <. CS:NO borrow,i.e. unsigned >=
		roksum = roksum - addr_limit - 1;
		if (roksum < (addr_limit + 1))
			flag = 0;//CC
		else
			;//CS
	}
	return flag;
	//summarization:
	//1.user process's addr_limit is 0xbf00 0000.
	//	if ((addr+size) < 2^32 && ((addr+size) < (addr_limit + 1))), access_ok is true.
	//	the above < is unsigned less
	//	to simplified:addr+size <= 0xbf00 0000
	//2.kernel thread addr_limit is 0. asctually bypass access_ok since it AL return 0;
	*/
	__asm__ __volatile__(
	"adds %1, %2, %3\n"
	"sbcccs %1, %1, %0\n"
	"movcc %0, #0"
	:"=&r"(flag), "=&r"(roksum)
	:"r"(addr), "Ir"(size), "0"(addr_limit)
	:"cc");
	return flag;
}

#define access_ok(type, addr, size) (__range_ok(addr, size) == 0)
//qemu-arm a.out to test it directly
int main(int argc, char **argv)
{
	const char *p = (const char*)(0xbf000000 - 1);
	int ret;
	if (argc > 1)
		addr_limit = strtoul(argv[1], NULL, 0);
	printf("addr_limit %lx\n", addr_limit);
	ret = access_ok(0, p, 1);
	printf("%p access_ok 1B %d\n", p, ret);

	ret = access_ok(0, p, 2);
	printf("%p access_ok 2B %d\n", p, ret);

	p = (const char*)0xffffffff;
	ret = access_ok(0, p, 1);
	printf("%p access_ok 1B %d\n", p, ret);
	p = (const char*)(0xbf000000);
	ret = access_ok(0, p, 1);
	printf("%p access_ok 1B %d\n", p , ret);
	return 0;
}