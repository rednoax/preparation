targets:=static shared
all:$(targets)

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
	rm -rf *.o *.lst *.a *.out *.so $(addsuffix objs/,$(targets))
