MAKEFLAGS+=-rR
SUBDIRS=tools \
	examples \
	api_examples

OBJS=start.o
#.PHONY:$(SUBDIRS)#no difference
u-boot:depend $(SUBDIRS) $(OBJS)
	: $@
depend:
	: $@
#the following depend will not run! Because it has run as the 1st prerequisite of u-boot?
$(SUBDIRS):depend autoconf.mk
	: $@
autoconf.mk:
	: $@

$(OBJS):depend autoconf.mk
	: $@
#to emulate u-boot's `make -C /home/rednoah/opt/src/mini2440/uboot/mini2440/board/mini2440/ u-boot.lds` for:\
$(LDSCRIPT):	depend $(obj)include/autoconf.mk\
		$(MAKE) -C $(dir $@) $(notdir $@)\
$ make rules.mk<--there is rules.mk in $(CURDIR)\
make: Nothing to be done for 'rules.mk'.<-when a target doesn't exist in Makefile but the target file exists, there is no error emit

#.PHONY:depend $(SUBDIRS) $(OBJS)#no matter if enabled, the result is still exactly the same. \
It seems that ALA it is the same make process, every unexisting file or PHONY target has a temp timestamp\
of itself. And Its recipe is run only once.