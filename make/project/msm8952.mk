iter=$(foreach v, $1, $(warning $v='$($v)'))
#the variables:
#defined in files of upper make:	Not inherited
#exported via upper make:			inherited
#defined at cmd line from caller make:	inherited
#defined at cmd line from upper(>1) caller make: inherited??

#DEBUG: defined in cmd line of caller make
#BOOTLOADER_OUT: defined in cmd line of caller make
#__MAKECMDGOALS: defined in caller makefile, NOT inherited
#project-name: defined in caller makefile, NOT inherited
#PRJECT:caller make's env
#pattern: defined in caller makefile, NOT inherited
#vars: exported from caller makefile
#OVER_ENV: envirenments variable defined both in upper makefile and caller make's env
#both: defined in caller make's env and upper make's cmdline, uses the latter

VARS:=DEBUG BOOTLOADER_OUT __MAKECMDGOALS project-name PRJECT pattern vars OVER_ENV both MAKECMDGOALS
$(call iter, $(VARS))
makepid=$(shell ps | grep make)
$(warning $(makepid))
.PHONY:all
all:
	echo current dir is $(shell pwd)
$(warning msm8952.mk:$(shell echo $$PPID))