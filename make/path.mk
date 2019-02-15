ROOT=$(shell pwd)
iter=$(foreach i, $1, $(warning $i=$($i)))
$(call iter, ROOT CURDIR)

#1&2 is equvalent except the 2nd will stay the sub directory make/ when finished
#1. make -C make/ -f path.mk
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
#make: Leaving directory '/home/rednoah/preparation/make'
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



.PHONY:all
all:
	@echo $(CURDIR)
	@echo `pwd`