/*
1.
objdump -D t.o
a/b in .data, c in .bss
.data:
a: .word 0x80
b. .word 0x8080
hexdump -C t.bin
b is the last byte in t.bin, no c in t.bin!

2. c is not in t.elf too as .bss not exist in ELF, let's show elf has no .bss:
readelf -S:
  [ 2] .data             PROGBITS        00010064 010064 000008 00  WA  0   0  4
  [ 3] .bss              NOBITS          0001006c 01006c 000004 00  WA  0   0  4<--.bss SEEMS immediately after .data, but only in VMA, NOT in elf file!
  [ 4] .ARM.attributes   ARM_ATTRIBUTES  00000000 01006c 000026 00      0   0  1<--section [4] is immediately after .data too, but only in ELF file rather than VMA!
$ arm-none-eabi-objdump -s t.elf

t.elf:     file format elf32-littlearm

Contents of section .text:
 10000 1cd09fe5 1c409fe5 000094e5 18409fe5  .....@.......@..
  ......
 10060 1eff2fe1                             ../.
Contents of section .data:
 10064 80000000 80800000                    ........		<--
Contents of section .ARM.attributes:
 0000 41250000 00616561 62690001 1b000000  A%...aeabi......	<--
 0010 05345400 06020801 09011204 14011501  .4T.............
 0020 17031801 1a01                        ......
Contents of section .comment:
 ......
$ hexdump -C t.elf
......
00010060  1e ff 2f e1 80 00 00 00  80 80 00 00 41 25 00 00  |../.........A%..|	<-- 41 25 00 00(.ARM.attributes) after .data
00010070  00 61 65 61 62 69 00 01  1b 00 00 00 05 34 54 00  |.aeabi.......4T.|
......
*/
int a = 0x80, b = 0x8080, c = 0;
int sum(int x, int y)
{
    return x + y;
}