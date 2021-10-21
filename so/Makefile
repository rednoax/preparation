all:
include Makefile.include

all:static shared

override var+=1#if no override, this can't add to var if var is defined in cmd line
#$(error var=$(var))

cmd = CFLAGS=$(CFLAGS) $(MAKE) -f Makefile.include OUTPUT=$@objs/

static shared:
	$(cmd) 
shared:CFLAGS = -fpic