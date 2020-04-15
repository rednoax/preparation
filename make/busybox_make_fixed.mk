#$ make -f busybox_make_fixed.mk config
#make: *** No rule to make target 'config'.  Stop.
#rednoah@ubuntu:~/preparation/make$ make -f busybox_make_fixed.mk menuconfig
#menuconfig
#rednoah@ubuntu:~/preparation/make$ make -f busybox_make_fixed.mk aconfig
#aconfig
KBUILD_VERBOSE=1
#only KBUILD_VERBOSE=1 can trigger "branch2", others will show "branch1"
$(if $(KBUILD_VERBOSE:1=), $(warning "branch1"), $(warning "branch2"))
%config:
	@echo $@
