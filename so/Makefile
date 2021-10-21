all:
include Makefile.include

all:static shared

#override var+=1#if no override, this can't add to var if var is defined in cmd line
#$(error var=$(var))

#export CFLAGS=$(cflags)#fail to reference $(cflags)
#$(error $($(CFLAGS)))

cmd = $(MAKE) -f Makefile.include OUTPUT=$@objs/

static shared:
	$(cmd) 
shared:export CFLAGS = -fpic