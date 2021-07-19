SUBDIRS=tools \
	examples \
	api_examples
#.PHONY:$(SUBDIRS)#no difference
u-boot:depend $(SUBDIRS)
	: $@
depend:
	: $@
#the following depend will not run! Because it has run as the 1st prerequisite of u-boot?
$(SUBDIRS):depend autoconf.mk
	: $@
autoconf.mk:
	: $@