#VBoxManage convertfromraw $1 $2 --format VHD
#usage: ./vhd.sh c08_mbr.bin c08.bin
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
if [ ! -z $2 ];then
	dd if=$2 of=8086.vhd seek=100 conv=notrunc
fi
dd if=$mbr of=bootimage-0.11-fd.img conv=notrunc
#ln -s $mbr c.img
