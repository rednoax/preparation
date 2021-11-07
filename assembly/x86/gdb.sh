#usage: ./gdb.sh ch4.out
gdb -ix 'gdb_init_real_mode.txt' -ex 'set tdsc filename target.xml' -ex 'target remote:1234' -ex 'b *0x7c00' $1
