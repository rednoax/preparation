#$ make -f busybox_make.mk menuconfig
#busybox_make.mk:1: *** mixed implicit and normal rules: deprecated syntax
#make: *** No rule to make target 'menuconfig'.  Stop.

#make aconfig will emit the similar error as the above.

#$ make -f busybox_make.mk config
#busybox_make.mk:4: *** mixed implicit and normal rules: deprecated syntax
#config

config %config:
	@echo $@
