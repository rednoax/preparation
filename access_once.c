#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x)) 
extern int a;
int b;
//all using "arm-linux-gcc -S volatilc.c -S -Os"
//1. use volatile to prevent GCC from optimizing assignments to the same var
#if 0
void func()
{
	b = 3;//optimized if no ACCESS_ONCE since the following assignment to "b"
	if (a) {
		b = 1;
	} else {
		b = 2;
	}
}
/*
* the equivalent of assembly:
	*** no "b = 3;"
	if (a)
		b = 1;
	else
		b = 2;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L5
	ldr	r3, .L5+4
	ldr	r2, [r2, #0]
	cmp	r2, #0			@ "b = 3;" is removed!
	movne	r2, #1
	moveq	r2, #2
	str	r2, [r3, #0]
	bx	lr
.L6:
	.align	2
.L5:
	.word	a
	.word	b

void func()
{
	ACCESS_ONCE(b) = 3;//will be reserved!
	if (a) {
		b = 1;
	} else {
		b = 2;
	}
}
/*
* "b = 3;" is reserved! It will be not optimized out by gcc

* the equivalent of assembly:
	b = 3;
	if (a)
		b = 1;
	else
		b = 2;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L5			@ the start of "b = 3;"
	mov	r2, #3
	str	r2, [r3, #0]	@ the end of "b = 3;"
	ldr	r2, .L5+4
	ldr	r2, [r2, #0]
	cmp	r2, #0
	movne	r2, #1
	moveq	r2, #2
	str	r2, [r3, #0]
	bx	lr
.L6:
	.align	2
.L5:
	.word	b
	.word	a
#endif

//2. use ACCESS_ONCE() to prevent conditonal from disappearing!
#if 0
void func()
{
	ACCESS_ONCE(b) = 3;
	if (a) {//will be optimized because 2 branches is exactly the same
		b = 1;
	} else {
		b = 1;
	}
}
/*
the quivalent of assembly:
	b = 3;
	***no load of "a";
	b = 1;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L4
	mov	r2, #3
	str	r2, [r3, #0]	@ the end of "b = 3;"
	mov	r2, #1
	str	r2, [r3, #0]	@ the end of "b = 1;" if(a) is optimized out!
	bx	lr
.L5:
	.align	2
.L4:
	.word	b
	.size	func, .-func

void func()
{
	ACCESS_ONCE(b) = 3;//use ACCESS_ONCE() to reserve conditional
	if (ACCESS_ONCE(a)) {//will not be optimized even with 2 same branches
		b = 1;
	} else {
		b = 1;
	}
}
/*
* the equivalent of assembly:
	b = 3;
	load a;
	b = 1;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L4
	mov	r2, #3
	str	r2, [r3, #0]	@ the end of "b = 3;"
	ldr	r2, .L4+4
	ldr	r2, [r2, #0]	@ load "a" will be reserved!
	mov	r2, #1
	str	r2, [r3, #0]	@ branch is still combined to only one single "b = 1;"
	bx	lr				@ because they are exactly the same
.L5:
	.align	2
.L4:
	.word	b
	.word	a
	.size	func, .-func
#endif

//3. use ACCESS_ONCE to prevent GCC from optimize some branch!
#if 0
void func()
{
	if (a) {
		b = 2;
	} else {
		b = 1;
	}
	b = 2;
}

/*
* the equivalent of assembly:
	***the whole if is optimized out!
	b = 2;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L2
	mov	r2, #2
	str	r2, [r3, #0]	@ only "b = 2;", no if and its branches
	bx	lr
.L3:
	.align	2
.L2:
	.word	b
	.size	func, .-func

void func()
{
	if (a) {
		b = 2;//will be optimized out
	} else {
		ACCESS_ONCE(b) = 1;//will not be optimized
	}
	b = 2;
}
/*
* the quivalent of assembly, the 1st branch is optimized out:
	if (!a)
		b = 1;//branch "b = 2" is optimized out
	b = 2;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L3
	ldr	r3, [r3, #0]		@<---load of "a"
	cmp	r3, #0				@ if(!a)
	ldr	r3, .L3+4
	moveq	r2, #1
	streq	r2, [r3, #0]	@ 	b = 1;
	mov	r2, #2
	str	r2, [r3, #0]		@ b = 3;
	bx	lr
.L4:
	.align	2
.L3:
	.word	a
	.word	b
	.size	func, .-func

void func()
{
	if (a) {
		ACCESS_ONCE(b) = 2;//will not be optimized
	} else {
		b = 1;//will be optimized out
	}
	b = 3;
}
/*
* the equivalent of assembly
	if (a)
		b = 2;//the 2nd branch of if is optimized out
	b = 3;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L6
	ldr	r3, [r3, #0]
	cmp	r3, #0
	ldr	r3, .L6+4
	movne	r2, #2
	strne	r2, [r3, #0]
	mov	r2, #3
	str	r2, [r3, #0]
	bx	lr
.L7:
	.align	2
.L6:
	.word	a
	.word	b
	.size	func, .-func
#endif
void func()
{
	if (a) {
		ACCESS_ONCE(b) = 2;//will not be optimized out
	} else {
		ACCESS_ONCE(b) = 1;//will not be optimized out
	}
	b = 3;
}
#if 0
/*
* the quivalent of assembly, all 2 branched is reserved!
	if (a)
		b = 2;
	else
		b = 1;
	b = 3;
*/
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L5
	ldr	r3, [r3, #0]
	cmp	r3, #0
	ldr	r3, .L5+4
	movne	r2, #2
	moveq	r2, #1
	str	r2, [r3, #0]
	mov	r2, #3
	str	r2, [r3, #0]
	bx	lr
.L6:
	.align	2
.L5:
	.word	a
	.word	b
	.size	func, .-func
#endif	