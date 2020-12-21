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

all:;@./fakemake.out VAR2=2

#note there is no VAR3 among the following output, if a var is nither exported nor defined at cmd line, it will not be seen in
#shell process's env/arg.

#rednoah@lucia:~/preparation/make/TGMB$ make -f u2.mk VAR0=0 VAR4=4 | grep -n VAR
#2:1: VAR2=2<--VAR2 is arg, will not be shown in env
#3:VVVVARVVV<--separator, the above is args and the below is env
#22:18: VAR0=0<--command line arg becomes an env
#29:25: VAR1=1<--exported var becomes an env
#32:28: MAKEFLAGS= -- VAR4=4 VAR0=0<--the flags from cmd line
#36:32: VAR4=4<--if one var is both from cmd line and file's exported var, then cmd line takes precedence.
