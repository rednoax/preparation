ROOT=$(shell pwd)
iter=$(foreach i, $1, $(warning $i=$($i)))
$(call iter, ROOT CURDIR PWD)
#$(error "test if -C can return normally")#test prove that it CAN
#1&2 is equvalent except the 2nd will stay the sub directory make/ when finished
#1. make -C make/ -f path.mk<--will AL return to org dir no matter if path.mk encounters error
#2. cd make && make -f path.mk
#3. make -f make/path.mk

#note the following ../ in "O=" is abs necessary! It is equvalent to cd kernel && make ...
#make -C kernel O=../out/target/product/msm8909/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE=~/msm8909/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi- $1

#result:
#1~3:

#rednoah@lucia:~/preparation$ make -C make/ -f path.mk
#make: Entering directory '/home/rednoah/preparation/make'
#path.mk:3: ROOT=/home/rednoah/preparation/make
#path.mk:3: CURDIR=/home/rednoah/preparation/make
#path.mk:3: PWD=/home/rednoah/preparation<--PWD is an env var(printenv|grep PWD) so it is inherited from environment
#path.mk:35: V=
#/home/rednoah/preparation/make<--CURDIR
#/home/rednoah/preparation/make<--`pwd` has no differece with CURDIR
#make -C sub0 -f sub0.mk
#make[1]: Entering directory '/home/rednoah/preparation/make/sub0'
#sub0.mk:1: V=[] MAKE1=make1 MAKE2=
#make[1]: Nothing to be done for 'all'.
#make[1]: Leaving directory '/home/rednoah/preparation/make/sub0'
#make: Leaving directory '/home/rednoah/preparation/make'

#rednoah@lucia:~/preparation$ cd make/ && make -f path.mk
#path.mk:3: ROOT=/home/rednoah/preparation/make
#path.mk:3: CURDIR=/home/rednoah/preparation/make
#path.mk:3: PWD=/home/rednoah/preparation/make
#path.mk:44: V=
#/home/rednoah/preparation/make
#/home/rednoah/preparation/make
#make -C sub0 -f sub0.mk
#make[1]: Entering directory '/home/rednoah/preparation/make/sub0'
#sub0.mk:1: V=[] MAKE1=make1 MAKE2=
#make[1]: Nothing to be done for 'all'.
#make[1]: Leaving directory '/home/rednoah/preparation/make/sub0'
#rednoah@ubuntu:~/preparation/make$ cd ..<----seems the only difference between 1 and 2;

#rednoah@lucia:~/preparation$ make -f make/path.mk
#make/path.mk:3: ROOT=/home/rednoah/preparation
#make/path.mk:3: CURDIR=/home/rednoah/preparation
#make/path.mk:3: PWD=/home/rednoah/preparation
#make/path.mk:52: V=
#/home/rednoah/preparation
#/home/rednoah/preparation
#make -C sub0 -f sub0.mk
#make[1]: *** sub0: No such file or directory.  Stop.
#make/path.mk:59: recipe for target 'all' failed
#make: *** [all] Error 2


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
#path.mk:3: PWD=/home/rednoah/preparation
#path.mk:58: V=2
#/home/rednoah/preparation/make
#/home/rednoah/preparation/make
#make -C sub0 -f sub0.mk
#make[1]: Entering directory '/home/rednoah/preparation/make/sub0'<---
#sub0.mk:1: V=[2] MAKE1=make1 MAKE2=<---MAKE2 will not be shown for it is not exported, MAKE1 is shown for it is exported; command line variables from upper make will show(no export needed)
#make[1]: Nothing to be done for 'all'.
#make[1]: Leaving directory '/home/rednoah/preparation/make/sub0'<---
#make: Leaving directory '/home/rednoah/preparation/make'<---
