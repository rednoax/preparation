static volatile unsigned int console_base = 0;
#if 0
000103d8 <console_init>:
   103d8:       e3500000        cmp     r0, #0
   103dc:       012fff1e        bxeq    lr
   103e0:       e59f3004        ldr     r3, [pc, #4]    ; 103ec <console_init+0x14>
   103e4:       e5830000        str     r0, [r3]
   103e8:       e12fff1e        bx      lr
   103ec:       00021028        andeq   r1, r2, r8, lsr #32

000103f0 <console_init2>:
   103f0:       e3500000        cmp     r0, #0
   103f4:       159f3004        ldrne   r3, [pc, #4]    ; 10400 <console_init2+0x10>
   103f8:       15830000        strne   r0, [r3]
   103fc:       e12fff1e        bx      lr
   10400:       00021028        andeq   r1, r2, r8, lsr #32

   
Disassembly of section .bss:

00021024 <__bss_start>:
  21024:	   00000000 	   andeq   r0, r0, r0

00021028 <console_base>:
  21028:	   00000000 	   andeq   r0, r0, r0
#endif
/*
The disassembly of console_init is nearly the same as that of console_init2!
Note what's the following input part of inline assembly will become?
*/
void console_init(unsigned int base)
{
	if (base == 0)
		return;
	//console_base = base;
	__asm__ __volatile__(
	"str %1, [%0]"
	:
	:"r"(&console_base),"r"(base)//NOTE:what the input will become?
	:);
}

void console_init2(unsigned int base)
{
	if (base == 0)
		return;
	console_base = base;
}

int main()
{
	return 0;
}
