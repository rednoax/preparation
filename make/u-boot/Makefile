SUBDIRS=tools \
	examples \
	api_examples
#.PHONY:$(SUBDIRS)#no difference
u-boot:depend $(SUBDIRS)
	: $@
depend:
	: $@

$(SUBDIRS):depend autoconf.mk
	: $@
autoconf.mk:
	: $@