MAKEFLAGS+=-rR
CROSS_COMPILE=#arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld# $(CC) will link libc.a and emit other error, so ld MUST be used
CFLAGS=-g -Wall -m32# if replacing -m32 with -m16, no difference for `file label.o label.out` output
TEXT_OFFSET=0x7c00
#if no '-m elf_i386':ld: i386 architecture of input file `label.o' is incompatible with i386:x86-64 output
LDFLAGS=-Ttext $(TEXT_OFFSET) -e _start -m elf_i386
OBJDUMP=objdump
OBJCOPY=objcopy
needlds=c08
target0=label ch4 cp5.3 ch5 ch6 ch7 ch7.1 ch8 c08_mbr $(needlds)
target1=$(addsuffix .asm,$(target0)) $(addsuffix .S,$(target0))
target2=$(wildcard $(target1))
target=$(sort $(patsubst %.S,%.out,$(filter %.S,$(target2))) $(patsubst %.asm,%.asm.out,$(filter %.asm,$(target2))))
#$(warning $(target))
_all:$(target)

%.out:%.o
#for %.o gened from NASM: if removing '-e _start' from $(LDFLAGS), there is still warning:\
'warning: cannot find entry symbol _start; defaulting to 0000000000007c00' if no global symbol '_start' defined in ch4.asm
	$(LD) $(LDFLAGS) -o $@ $(filter %.o, $^)
	$(OBJCOPY) -O binary $@ $@.bin
#$(OBJDUMP) -M i8086 -d $@ #>$@.lst

%.o:%.S $(MAKEFILE_LIST)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.{o,out,lst,bin}

FILE?=label.out
debug:
	qemu-x86_64 -g 1234 $(FILE)

%.asm.o:%.asm $(MAKEFILE_LIST)
#$@: ELF 32-bit LSB relocatable, Intel 80386, version 1 (SYSV), not stripped
#objdump -d $@ can fully display the right 8086 instructions as there is prefix 66 before each instruction,\
 while gcc generated label.out's .text instructions have no 66 prefixed
#if no '-g3 -F dwarf': gdb x.asm.out emits: (No debugging symbols found in ch4.asm.out)
	nasm -f elf -g3 -F dwarf -o $@ $<
	nasm -f bin -o $@.bin $<

LDFLAGS2=-m elf_i386 -T $(@:.out=.lds)
c08.out:LDFLAGS=$(LDFLAGS2)
define rule
$1.out: $1.lds
endef
$(foreach i,$(needlds),$(eval $(call rule,$i)))

.PRECIOUS:%.o %.asm.o
