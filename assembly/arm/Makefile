CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld# $(CC) will link libc.a
CFLAGS=-g -Wall
LDFLAGS=
target:=illegal_instruction label

_all:

%.out:%.o
	$(LD) $(LDFLAGS) -o $@ $^
%.o:%.S
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o *.out
_all:$(addsuffix .out,$(target))
