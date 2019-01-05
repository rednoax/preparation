C_SOURCE=$(wildcard *.c)
GCC:=/home/rednoah/OP-TEE/toolchains/aarch32/bin/arm-linux-gnueabihf-gcc
OBJDUMP:=/home/rednoah/OP-TEE/toolchains/aarch32/bin/arm-linux-gnueabihf-objdump
C_OUT=$(patsubst %.c,%.out,$(C_SOURCE))
.PHONY:all
all:$(C_OUT)

#once -Os, try_lock cannot emit error
#CFLAGS:=-pthread -static -Wall -Os
CFLAGS:=-pthread -static -Wall
%.out:%.c Makefile
	$(GCC) $(CFLAGS) $< -o $@
	$(GCC) $(CFLAGS) $< -S
	$(OBJDUMP) -dx $@ > $@.dump.s