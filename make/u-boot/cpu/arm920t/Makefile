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
include .depend
clean:
	rm -rf start.o .depend
$(warning ###Makefile finish)

#$ make clean\
Makefile:8: ###\
Makefile:20: ###Makefile finish\
rm -rf .depend\
g=start.o;gcc -M -MQ $g start.c >>.depend\
echo '$(warning .depend included)'>>.depend\
Makefile:8: ###\
.depend:22: .depend included\
Makefile:20: ###Makefile finish\
rm -rf start.o .depend\
$ make\
Makefile:8: ###\
Makefile:20: ###Makefile finish\
rm -rf .depend\
g=start.o;gcc -M -MQ $g start.c >>.depend\
echo '$(warning .depend included)'>>.depend\
Makefile:8: ###\
.depend:22: .depend included\
Makefile:20: ###Makefile finish\
gcc -c start.c -o start.o

#once .depend exists and start.c is not newer than .depend:\
$ make\
Makefile:8: ###\
.depend:22: .depend included\
Makefile:20: ###Makefile finish\
make: Nothing to be done for 'all'.


#once .depend exists and start.c is newer than .depend:\
$ make -W start.c\
Makefile:8: ###\
.depend:22: .depend included\
Makefile:20: ###Makefile finish\
rm -rf .depend\
g=start.o;gcc -M -MQ $g start.c >>.depend\
echo '$(warning .depend included)'>>.depend<--.depend is remade since its prerequisite start.c is newer\
Makefile:8: ###\
.depend:22: .depend included\
Makefile:20: ###Makefile finish\
gcc -c start.c -o start.o

#once .depend exists, start.c is old than .depend but some header file in .depend is newer than start.c:\
$ make -W /usr/include/stdc-predef.h\
Makefile:8: ###\
.depend:22: .depend included\
Makefile:20: ###Makefile finish\
gcc -c start.c -o start.o<--start.o is remade while .depend will NOT be remade since it doesn't depend the chaned header file

#to summarize, when .depend is generated:\
either there is no .depend at all or one of its prerequisite is newer than itself;\
once a newer .depend is generated, the whole Makefile is read for a second time.\
If .depend need not to be updated, the whold Makefile is read only once, no the 2nd parsing.