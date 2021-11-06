MAKEFLAGS+=-rR
#CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld# $(CC) will link libc.a and emit other error, so ld MUST be used
CFLAGS=-g -Wall -m32# if replacing -m32 with -m16, no difference for `file label.o label.out` output
#if no '-m elf_i386':ld: i386 architecture of input file `label.o' is incompatible with i386:x86-64 output
LDFLAGS=-Ttext 0 -e _start -m elf_i386
OBJDUMP=objdump
OBJCOPY=objcopy
target0=label ch4
target1=$(addsuffix .asm,$(target0)) $(addsuffix .S,$(target0))
target2=$(wildcard $(target1))
target=$(sort $(patsubst %.S,%.out,$(filter %.S,$(target2))) $(patsubst %.asm,%.asm.o,$(filter %.asm,$(target2))))

_all:$(target)

%.out:%.o
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin
#$(OBJDUMP) -M i8086 -d $@ #>$@.lst

%.o:%.S $(MAKEFILE_LIST)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.{o,out,lst,bin}

FILE?=label.out
debug:
	qemu-x86_64 -g 1234 $(FILE)

%.asm.o:%.asm
#label2.out: ELF 32-bit LSB relocatable, Intel 80386, version 1 (SYSV), not stripped
#objdump -d label2.out can fully display the right 8086 instructions as there is prefix 66 before each instruction,
#while label.out 's .text instructions have no 66 prefixed
	nasm -f elf -o $@ $<
	nasm -f bin -o $(patsubst %.asm,%.bin,$<) $<

.PRECIOUS:%.o