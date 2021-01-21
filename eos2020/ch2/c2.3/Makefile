CROSS_TOOLCHAIN=~/arm-2013.11/bin/arm-none-eabi-
GCC=$(CROSS_TOOLCHAIN)gcc
LD=$(CROSS_TOOLCHAIN)ld
OBJCOPY=$(CROSS_TOOLCHAIN)objcopy
CFLAGS=-Wall -g
BIN=t.bin
all: $(BIN)
$(BIN):t.elf
	$(OBJCOPY) -O binary $< $@
#not ts.o t.o, to prove the sequence specified in .ld take precedence
t.elf: t.o ts.o
	$(LD) -T t.ld -o $@ $^
%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(GCC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.{o,elf,bin}
run:
	qemu-system-arm -M versatilepb -m 128M -nographic -serial mon:stdio -kernel $(BIN) -S -s