#VBoxManage convertfromraw $1 $2 --format VHD
file0=1.bin
tail=tail.bin
mbr=mbr.bin
dd_option="bs=510 count=1 conv=notrunc"
dd if=/dev/zero of=$file0 bs=510 count=1
echo -e -n '\x55\xaa' >$tail
cat $file0 $tail >$mbr
rm -rf $file0 $tail

dd if=$1 of=$mbr $dd_option
dd if=$mbr of=8086.vhd conv=notrunc
