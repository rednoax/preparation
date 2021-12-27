CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld# $(CC) will link libc.a
CFLAGS=-g -Wall -mcpu=arm920t
LDFLAGS=
target:=illegal_instruction label ldr_data

_all:

%.out:%.o
	$(LD) $(LDFLAGS) -o $@ $^
%.o:%.S
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o *.out
_all:$(addsuffix .out,$(target))

qemu:
	qemu-arm -g 1234 $(file)