CROSS_TOOLCHAIN=~/arm-2013.11/bin/arm-none-eabi-
GCC=$(CROSS_TOOLCHAIN)gcc
LD=$(CROSS_TOOLCHAIN)ld
NM=$(CROSS_TOOLCHAIN)nm
OBJDUMP=$(CROSS_TOOLCHAIN)objdump
OBJCOPY=$(CROSS_TOOLCHAIN)objcopy
CFLAGS=-Wall -g
BIN=t.bin
ELF=t.elf
all: $(BIN)
$(BIN):$(ELF)
	$(OBJCOPY) -O binary $< $@
#objdump -d proves:the following sequence takes effect when t.ld does't specify sequence like:
#ts.o(.text) *(.text)
$(ELF): ts.o t.o
	$(LD) -T t.ld -o $@ $^

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(GCC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.{o,elf,bin}

#nm to show address of c, then xp/8x to check it content in qemu monitor
run:
	$(NM) $(ELF)
	qemu-system-arm -M versatilepb -m 128M -nographic -serial mon:stdio -kernel $(BIN)
telnet:
	qemu-system-arm -M versatilepb -m 128M -nographic -serial telnet::1234,server -kernel $(BIN)
#before "make pts", do sleep 1000000 on pts3
pts:
	qemu-system-arm -M versatilepb -m 128M -nographic -serial /dev/pts/3 -kernel $(BIN)

#objdump -x or readelf -a to show symbol tables