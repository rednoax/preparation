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

#objdump -d proves:the following sequence "porkcar.o t.o ts.o panda.o" takes effect when t.ld does't specify
# sequence like:ts.o(.text) *(.text). If there is such specified sequence in .ld script, it will take
# precedence to the sequence in Makefile
#$(ELF): porkcar.o t.o ts.o panda.o

#EVEN t.o is before ts.o in cmd line, ld -verbose shows that ts.o is opened before t.o as ts.o is explicitely
# specified before other .o
#NOTE that the cmd line sequence and ld script's input sequence:
#1.both u-boot & linux kernel specifies all input file in ld's cmd line(ld script didn't use INPUT or some file
# names that doesn't among the ld's cmd line ) and the 1st file in cmd line is the one including startup
# assembly that should be put at the front of output file.
#2. u-boot specifed the 1st file start.o in ld script too to double make sure the right link sequence to
# put the file including startup assembly to be at the beginning of output file.
#3. linux kernel specifed a specific section defined in head.S to be put at the begining of output file. Not the
# way to use an explicite file name as u-boot.
$(ELF): t.o ts.o panda.o porkcar.o
	$(LD) -verbose -T t.ld -o $@ $^
%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(GCC) $(CFLAGS) -c $< -o $@
%.o: %.bmp
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm $< $@
clean:
	rm -rf *.{o,elf,bin}
run:
	$(NM) $(ELF)
	qemu-system-arm -M versatilepb -m 128M -serial mon:stdio -kernel $(BIN)
telnet:
	qemu-system-arm -M versatilepb -m 128M -nographic -serial telnet::1234,server -kernel $(BIN)
#before "make pts", do sleep 1000000 on pts3
pts:
	qemu-system-arm -M versatilepb -m 128M -nographic -serial /dev/pts/3 -kernel $(BIN)

#objdump -x or readelf -a to show symbol tables