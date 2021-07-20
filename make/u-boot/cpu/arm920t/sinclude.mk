ifeq (1,0)
ENVVAR+=file
CMDVAR+=file
#$(info $(origin MAKEFLAGS))#file(so "MAKEFLAGS+=" can append new value)
$(error {$(ENVVAR),$(CMDVAR)})#ENVVAR=env make CMDVAR=cmd:{env file,cmd}
endif
MAKEFLAGS+=-rR
$(warning ###)
all:start.o
start.o:start.c
	gcc -c $< -o $@

.depend:start.c
	rm -rf $@
	g=$(<:.c=.o);gcc -M -MQ $$g $< >>$@
	echo '$$(warning $@ included)'>>$@
#when there is rule to generate the file that is sinclued, the behivor of "sinclude xx" \
 is identical to "include xx"
sinclude .depend
clean:
	rm -rf start.o .depend
$(warning ###Makefile finish)
