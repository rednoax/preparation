#TO TEST:
#a c program can dump all its args/envs to verify the cmd line var/file var/env var of makefile

VAR3=file

ifeq (1,1)
VAR1=1
VAR4=file4
export VAR1 VAR4
else
#don't use export VAR1=1 VAR4=file4, which means VAR1's value is "1 VAR4=file4" and VAR4 is not defined at all!
export VAR1=1 VAR4=file4
#the output will be:
#2:1: VAR2=2
#3:VVVVARVVV
#22:18: VAR0=0
#29:25: VAR1=1 VAR4=file4
#32:28: MAKEFLAGS= -- VAR4=4 VAR0=0<--the MAKEFLAGS contains the flags specified on the cmd line
#36:32: VAR4=4
endif

#VAR5=5 will be env while VAR2=2 is argument
all:;@VAR5=5 ./fakemake.out VAR2=2

#note there is no VAR3 among the following output, if a var is nither exported nor defined at cmd line, it will nither be seen in
#shell process's env nor args.

#rednoah@lucia:~/preparation/make/TGMB$ make -f u2.mk VAR0=0 VAR4=4 | grep -n VAR
#2:1: VAR2=2<--VAR2 is arg, will not be shown in env
#3:VVVVARVVV<--separator, the above is args and the below is env
#15:11: MAKEFLAGS= -- VAR4=4 VAR0=0<--the flags from cmd line
#21:17: VAR0=0<--command line arg becomes an env
#22:18: VAR1=1<--exported var becomes an env
#28:24: VAR4=4<--if one var is both from cmd line and file's exported var, then cmd line takes precedence.
#31:27: VAR5=5<--the var definination before elf is env!
