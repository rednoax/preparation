#if no file "force" and it isnot .PHONY, each run of `make -f force.mk` will AL run `touch a.c`.\
However, if there is a file named 'force' and it's older than a.c:\
$ touch force\
$ touch a.c\
$ ll a.c force\
-rw-rw-r-- 1 rednoah rednoah 0 Jun 27 17:52 a.c\
-rw-rw-r-- 1 rednoah rednoah 0 Jun 27 17:51 force\
$ make -f force.mk\
make: 'a.c' is up to date.
a.c:force
#if `make -f forcce.mk -s`, @ is added automatically even there is no @ before recipe
	touch $@
force:;

.PHONY:force#to ask `make -f force.mk` to `touch a.c` unconditionally in each run even there is a file named force

#BASH is not env in shell, declare -p BASH show 'declare -- BASH="/bin/bash"'
$(info $(origin BASH) $(shell echo $$BASH))#undefined /bin/sh

$(info /$(shell echo *.c)/)#a.c b.c
$(info /$(shell echo sub2/*.c)/)#sub2/*.c
$(info /$(wildcard *.c)/)#a.c b.c
$(info /$(wildcard sub2/*.c)/)#<empty>