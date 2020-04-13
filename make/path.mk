ROOT=$(shell pwd)
iter=$(foreach i, $1, $(warning $i=$($i)))
$(call iter, ROOT CURDIR)
#$(error "test if -C can return normally")#test prove that it CAN
#1&2 is equvalent except the 2nd will stay the sub directory make/ when finished
#1. make -C make/ -f path.mk<--will AL return to org dir no matter if path.mk encounters error
#2. cd make && make -f path.mk
#3. make -f make/path.mk

#note the following ../ in "O=" is abs necessary! It is equvalent to cd kernel && make ...
#make -C kernel O=../out/target/product/msm8909/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE=~/msm8909/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi- $1

#result:
#1~3:
#rednoah@ubuntu:~/preparation$ make -C make -f path.mk
#make: Entering directory '/home/rednoah/preparation/make'
#path.mk:3: ROOT=/home/rednoah/preparation/make
#path.mk:3: CURDIR=/home/rednoah/preparation/make
#/home/rednoah/preparation/make
#/home/rednoah/preparation/make
#make: Leaving directory '/home/rednoah/preparation/make'<-------
#rednoah@ubuntu:~/preparation$ cd make&& make -f path.mk
#path.mk:3: ROOT=/home/rednoah/preparation/make
#path.mk:3: CURDIR=/home/rednoah/preparation/make
#/home/rednoah/preparation/make
#/home/rednoah/preparation/make
#rednoah@ubuntu:~/preparation/make$ cd ..<----seems the only difference between 1 and 2;
#rednoah@ubuntu:~/preparation$ make -f make/path.mk
#make/path.mk:3: ROOT=/home/rednoah/preparation
#make/path.mk:3: CURDIR=/home/rednoah/preparation
#/home/rednoah/preparation
#/home/rednoah/preparation
#rednoah@ubuntu:~/preparation$

$(call iter, V)
MAKE1=make1
MAKE2=make2

export iter MAKE1
.PHONY:all
all:
	@echo $(CURDIR)
	@echo `pwd`
	make -C sub0 -f sub0.mk

#make -C make/ -f path.mk V=2
#make: Entering directory '/home/rednoah/preparation/make'<---
#path.mk:3: ROOT=/home/rednoah/preparation/make
#path.mk:3: CURDIR=/home/rednoah/preparation/make
#path.mk:35: V=2
#/home/rednoah/preparation/make
#/home/rednoah/preparation/make
#make -C sub0 -f sub0.mk
#make[1]: Entering directory '/home/rednoah/preparation/make/sub0'<---
#sub0.mk:1: V=[2] MAKE1=make1 MAKE2=<---MAKE2 will not be shown for it is not exported, MAKE1 is shown for it is exported; command line variables from upper make will show(no export needed)
#make[1]: Nothing to be done for 'all'.
#make[1]: Leaving directory '/home/rednoah/preparation/make/sub0'<---
#make: Leaving directory '/home/rednoah/preparation/make'<---
