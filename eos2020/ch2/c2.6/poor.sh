CROSS_COMPILE=~/arm-2013.11/bin/arm-none-eabi-
LD=${CROSS_COMPILE}ld

$LD -verbose -T poor.ld ts.o t.o -o poor.elf