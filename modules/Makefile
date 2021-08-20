# To build modules outside of the kernel tree, we run "make"
# in the kernel source tree; the Makefile these then includes this
# Makefile once again.
# This conditional selects whether we are being included from the
# kernel Makefile or not.

# O=XX seems no effect
# make O=~/O_versatile/ ARCH=arm CROSS_COMPILE=arm-linux- KERNELDIR=~/kernel/ V=[0|1|2]
#make O=~/A10M/out/target/product/A10_32/obj/kernel/msm-3.18 KERNELDIR=~/A10M/kernel/msm-3.18 \
	ARCH=arm CROSS_COMPILE=~/A10M/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
KERNELDIR=/home/rednoah/opt/src/op-tee/linux
ARCH=arm
CROSS_COMPILE="/home/rednoah/opt/src/op-tee/build/../toolchains/aarch32/bin/arm-linux-gnueabihf-"
export KERNELDIR ARCH CROSS_COMPILE
ifeq ($(KERNELRELEASE),)

    # Assume the source tree is where the running kernel was built
    # You should set KERNELDIR in the environment if it's elsewhere
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build
    # The current directory is passed to sub-makes as argument
    PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

.PHONY: modules modules_install clean

else
    # called from kernel build system: just declare what our modules are
    #obj-m := hello.o hellop.o seq.o jit.o jiq.o sleepy.o complete.o \
             silly.o faulty.o kdatasize.o kdataalign.o
#obj-m := debug_files.o
obj-m:=ptdump.o
ptdump-y:=dump.o ptdump_debugfs.o
endif


