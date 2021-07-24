CROSS_TOOLCHAIN=arm-none-eabi-
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
#to make the following sequence takes effect, t.ld will not specify sequence<-proved by objdump -d
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
	qemu-system-arm -M realview-pbx-a9 -nographic -serial /dev/null -kernel $(BIN)

#objdump -x or readelf -a to show symbol tables