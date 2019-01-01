C_SOURCE=$(wildcard *.c)
GCC:=/home/rednoah/OP-TEE/toolchains/aarch32/bin/arm-linux-gnueabihf-gcc
C_OUT=$(patsubst %.c,%.out,$(C_SOURCE))
.PHONY:all
all:$(C_OUT)

%.out:%.c
	$(GCC) -pthread -static -Wall $< -o $@
