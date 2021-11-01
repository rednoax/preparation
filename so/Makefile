targets:=static shared
all:$(targets)
ifdef GCC
CC=gcc
else
CC=clang
endif
export CC
#override var+=1#if no override, this can't add to var if var is defined in cmd line
#$(error var=$(var))

#export CFLAGS=$(cflags)#fail to reference $(cflags)
#$(error $($(CFLAGS)))

cmd = $(MAKE) -f Makefile.include OUTPUT=$@objs/

$(targets):
	@echo "###$@###"
	$(cmd) 
shared:export CFLAGS = -fpic

clean:
	rm -rf *.{o,bc,ll,lst,.a,.out,so,s} $(addsuffix objs/,$(targets))
ifeq (0,1)
CROSS_COMPILE=/usr/bin/ccache /home/rednoah/opt/src/op-tee/build/../toolchains/aarch32/bin/arm-linux-gnueabihf-
CLANG_ARCH_ARGS = --target=$(notdir $(patsubst %-,%,$1))
$(error $(call CLANG_ARCH_ARGS,$(CROSS_COMPILE)) [$(call CLANG_ARCH_ARGS,/home/rednoah/opt/src/op-tee/build/../toolchains/aarch32/bin/arm-linux-gnueabihf-)])
endif
