#CROSS_TOOLCHAIN=~/arm-2013.11/bin/arm-none-eabi-
CROSS_TOOLCHAIN=arm-none-eabi-
AS=$(CROSS_TOOLCHAIN)as
LD=$(CROSS_TOOLCHAIN)ld
OBJCOPY=$(CROSS_TOOLCHAIN)objcopy

bin=t.bin
all:$(bin)

$(bin):t.elf
	$(OBJCOPY) -O binary $< $@
	
t.elf:ts.o t.ld
	$(LD) -T t.ld -o $@ $<

#if -g is not specified:
#The following is emitted when running "arm-none-eabi-gdb t.elf":
# Reading symbols from /home/rednoah/preparation/eos2020/ch2/c2.1/t.elf...(no debugging symbols found)...done.
#Once -g is added, gdb t.elf will have no warning as above

%.o: %.s
#-g to add debug symbols
	$(AS) -g -o $@ $<

run:
	qemu-system-arm -M versatilepb -m 128M -kernel $(bin) -serial mon:stdio -nographic -S -s

#if as needs -c?
#As as manual doesn't mention -c at all, it is not suggested to add -c for as!
#PLZ note the following 3 generate the same md5 ts.o
#$ arm-none-eabi-as -c ts.s -o ts-c.o
#$ arm-none-eabi-as  ts.s -o ts.o
#$ arm-none-eabi-gcc -Wall -c ts.s -o ts-gcc-c.o
#$ md5sum ts*.o
#658a423be7099acc0545e7204388ff20  ts-c.o
#658a423be7099acc0545e7204388ff20  ts-gcc-c.o
#658a423be7099acc0545e7204388ff20  ts.o

#as -Wall can emit mixed machine for disassembly. UNDOCUMENTED!
#$ arm-none-eabi-as -Wall -c ts.s
#ARM GAS  ts.s                   page 1
#
#
#   1                    /*ts.s file of C2.1*/
#   2
#   3                    .text
#   4                            .global start
#   5                    start:
#   6 0000 0100A0E3              mov r0, #1<--0100a0e3 is the same as following mem dump at 0x10000
#   7 0004 0210A0E3              mov r1, #2<--ditto
#   8 0008 001081E0              add r1, r1, r0<--ditto
#   9 000c 04209FE5              ldr r2, =result @both ts.o and ts.elf: ldr r2, [pc, #4], see comment 1<--ditto
#  10 0010 001082E5              str r1, [r2]<--ditto

#(gdb) x/8x 0x10000
#0x10000 <start>:        0xe3a00001      0xe3a01002      0xe0811000      0xe59f2004
#0x10010 <start+16>:     0xe5821000      0xeafffffe      0x0001001c      0x00000000
