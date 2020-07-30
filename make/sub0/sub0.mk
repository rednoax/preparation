$(warning V=[$(V)] MAKE1=$(MAKE1) MAKE2=$(MAKE2))
#even the upper layer explicitely export iter, the following will not work
$(call iter, V)
.PHONY:all
all:
#1. so only the 1st -f will be used? and the last -C will be used
#rednoah@lucia:~/preparation$ make -C make -f macro.mk -C ../ -f Makefile<--make/ has macro.mk and ./ has Makefile,the 2nd -C use ../ rather than ./ since the 1st -C has change CURDIR to preparation/make/, to return to the parent dir preparation/:-C .. rather than -C . shoud be used
#make: Entering directory '/home/rednoah/preparation'<--it means only the last -C's dir is used to find make file
#make: macro.mk: No such file or directory<---it means only the 1st -f specified file is used if there is multiple -f
#make: *** No rule to make target 'macro.mk'.  Stop.
#make: Leaving directory '/home/rednoah/preparation'

#2.there is Makefile under preparation/pthread/ and preparation/link-script/ but the following will fail for the 1st -C has changed CURDIR to preparation/pthread/, there is no link-script/ under pthread/ so the following will fail;
#rednoah@lucia:~/preparation$ make -C pthread/ -C link-script/
#make: *** link-script/: No such file or directory.  Stop.<---there is no preparation/pthread/link-script/

#3.when there are multiple -C(suppose no -f used so only file named "Makefile" is searched), only the last -C 's Makefile is used!
#rednoah@lucia:~/preparation$ make -C pthread/ -C ../link-script/<--the 2nd -C shuld use ../link-script rather than ./link-script, see 2 for the reason
#make: Entering directory '/home/rednoah/preparation/link-script'<--the last -C's dir is used to searche "Makefile"
#arm-linux-gcc -c -Wall start.S -o start.o
#make: arm-linux-gcc: Command not found
#Makefile:11: recipe for target 'start.o' failed
#make: *** [start.o] Error 127
#make: Leaving directory '/home/rednoah/preparation/link-script'
#rednoah@lucia:~/preparation$

