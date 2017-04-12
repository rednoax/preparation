void get_user_4(char *argv[])
{
	register unsigned long __r2 asm("r2");
	register int __e asm("r0");
	__asm__ __volatile__(
	/*
	if use the following:
	".ifnc %0, r1\n"
	then:
/tmp/cc2I5jY6.s: Assembler messages:
/tmp/cc2I5jY6.s:30: Error: .err encountered
	*/
	".ifnc %0, r0;"
	".err;"
	".endif;"
	"ldr r2, [r0];"
	"mov r0, #0"
	:"=&r"(__e),"=r"(__r2)
	:"0"(argv)
	:"cc");
}

/*
arm-linux-objdump -D getuser.o |less
0000002c <get_user_4_1>:
  2c:   e52db004        push    {fp}            ; (str fp, [sp, #-4]!)
  30:   e28db000        add     fp, sp, #0
  34:   e24dd00c        sub     sp, sp, #12
  38:   e50b0008        str     r0, [fp, #-8]
  3c:   e51b3008        ldr     r3, [fp, #-8]
  40:   e1a01003        mov     r1, r3
  44:   e5912000        ldr     r2, [r1]
  48:   e24bd000        sub     sp, fp, #0
  4c:   e49db004        pop     {fp}            ; (ldr fp, [sp], #4)
  50:   e12fff1e        bx      lr

00000054 <get_user_4_2>:
  54:   e52db004        push    {fp}            ; (str fp, [sp, #-4]!)
  58:   e28db000        add     fp, sp, #0
  5c:   e24dd00c        sub     sp, sp, #12
  60:   e50b0008        str     r0, [fp, #-8]
  64:   e51b3008        ldr     r3, [fp, #-8]
  68:   e1a00003        mov     r0, r3
  6c:   e5902000        ldr     r2, [r0]
  70:   e24bd000        sub     sp, fp, #0
  74:   e49db004        pop     {fp}            ; (ldr fp, [sp], #4)
  78:   e12fff1e        bx      lr
  
for -Os -c
0000000c <get_user_4_1>:
   c:   e1a01000        mov     r1, r0
  10:   e5912000        ldr     r2, [r1]
  14:   e12fff1e        bx      lr

00000018 <get_user_4_2>:
  18:   e5902000        ldr     r2, [r0]
  1c:   e12fff1e        bx      lr
*/
void get_user_4_1(char *argv[])
{
	register unsigned long __r2 asm("r2");
	register int __e asm("r1");
	__asm__ __volatile__(
	"ldr %1, [%0];"
	:"=&r"(__e),"=r"(__r2)
	:"0"(argv)
	:"cc");
}

void get_user_4_2(char *argv[])
{
	register unsigned long __r2 asm("r2");
	register int __e asm("r0");
	__asm__ __volatile__(
	"ldr %1, [%0];"
	:"=&r"(__e),"=r"(__r2)
	:"0"(argv)
	:"cc");
}

int main(int argc, char**argv)
{
	get_user_4(argv);
	return 0;
}
