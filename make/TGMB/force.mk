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
	touch $@
force:;

.PHONY:force#to ask `make -f force.mk` to `touch a.c` unconditionally in each run even there is a file named force

$(info $(origin BASH) $(shell echo $$BASH))#undefined /bin/sh