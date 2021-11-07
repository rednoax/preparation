#usage: ./qemu.sh 8086.vhd -S -s
#gdb -ex 'target remote:1234' -ex 'set architecture i8086' -ex 'break *0x7c00' -ex 'layout regs' -ex 'layout src'
#NOTE:
#1. simplified 'set arch i8086' is equivalent to 'set architecture i8086'
#2.'set architecture i8086' should be AFTER 'target remote:1234', there is warning otherwise.
#3. display/i $pc is not right 8086 assembly and eip w/t cs is used as break point address
qemu-system-i386 -m 16M  -boot c -hda $*
#the above can be simplified to: qemu-system-i386 -hda $*

#-enable-kvm option makes 'b *0x7c00' not take effect, the breakpoint is never hit.
#qemu-system-i386 -m 16M -boot c -enable-kvm -hda $*

