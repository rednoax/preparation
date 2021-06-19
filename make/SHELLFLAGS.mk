#make -p|less -N \
...\
16 # default \
17 .SHELLFLAGS := -c \
...\
34 # makefile\
35 SHELL = /bin/sh

$(info $(origin SHELL) $(origin .SHELLFLAGS) $(origin CC))#file default default

wold=${SHELL}
rold:=${SHELL}
SHELL=/bin/dash
$(info ${SHELL}<=${wold}) #/bin/dash<=/bin/dash
$(info ${SHELL}<=${rold}) #/bin/dash<=/bin/sh
all:
	@echo $${SHELL} #/bin/bash is from original environment, rather than make's setting

#export SHELL=/bin/dash#if enabled, the above echo-ed is:/bin/dash