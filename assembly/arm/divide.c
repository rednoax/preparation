/*
different -mabi option causes different divide function name!
if no -mabi, aapcs-linux is used
arm$ arm-none-linux-gnueabi-gcc -S -mabi=aapcs-linux divide.c -o 2.s <= bl	__aeabi_idiv
arm$ arm-none-linux-gnueabi-gcc -S -mabi=apcs-gnu divide.c -o 1.s <= bl	__divsi3
*/
int divide(int i)
{
	return i / *(int*)4;
}
