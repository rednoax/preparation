CROSS_TOOLCHAIN=~/arm-2013.11/bin/arm-none-eabi-
AS=$(CROSS_TOOLCHAIN)as
LD=$(CROSS_TOOLCHAIN)ld
OBJCOPY=$(CROSS_TOOLCHAIN)objcopy

bin=t.bin
all:$(bin)

$(bin):t.elf
	$(OBJCOPY) -O binary $< $@
	
t.elf:ts.o t.ld
	$(LD) -T t.ld -o $@ $<

%.o: %.s
	$(AS) -g -o $@ $<

clean:
	rm -rf *.{o,elf,bin}
run:
	qemu-system-arm -M versatilepb -m 128M -kernel $(bin) -serial mon:stdio -nographic -S -s