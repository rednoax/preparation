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

#make,2861 -f u2.mk VAR0=0 VAR4=4 all \
VAR5=5 will be env while VAR2=2 is argument, note make will not lauch a middle shell to run fakemake.out \
                  └─make,2861 -f u2.mk VAR0=0 VAR4=4 all \
                      └─fakemake.out,2862 VAR2=2
all:;@VAR5=5 ./fakemake.out VAR2=2

#$ make -f u2.mk all33 \
MAKELEVEL=0 MAKEFLAGS=<-top make process phase 1's dump in prerequisite of all44 \
make[1]: Entering directory '/home/rednoah/opt/preparation/make/TGMB'<-all33 generate child make of L1,even in the same directory \
MAKELEVEL=1 MAKEFLAGS=w<-child make process phase 1's dump in prerequisite of all4,"MAKEFLAGS is turned on auto when using -C", but here no -C.So -w auto on ALA in sub-make? \
VAR5=5 make VAR2=2 -C sub -f a.mk <-a level2 child make launched!\
make[2]: Entering directory '/home/rednoah/opt/preparation/make/TGMB/sub'<-all3 fork+exec a child make\
MAKE=make       default \
MAKEFLAGS=w     file <-w passed down since L1 child make\
MAKELEVEL=2     environment<--note it is 2, not 1, L1 child make is on the above\
MAKEFILES=      default \
SHELL=/bin/sh   file \
VAR0=0  command line<--from L1 make's cmd line, transfered via MAKEFLAGS \
VAR1=1  environment<--from exported of L1 make \
VAR2=2  command line<--from level2 make's cmd line \
VAR3=   undefined<--for VAR3 is neither exported by L1 make nor in the cmd line of L1 make or L2 make \
VAR4=4  command line<--both exported by L1 make and MAKEFLAGS(for L1 make's cmd line has VAR4), use later(L1 make's cmd line) \
VAR5=5  environment<--from real environment of L2 make \
make[2]: Nothing to be done for 'all'.\
make[2]: Leaving directory '/home/rednoah/opt/preparation/make/TGMB/sub'\
make[1]: Leaving directory '/home/rednoah/opt/preparation/make/TGMB'

#all3:;VAR5=5 make VAR2=2 -C sub -f a.mk$(info $(MAKELEVEL)-${MAKEFLAGS})
all3:;VAR5=5 make VAR2=2 -C sub -f a.mk
all33:;@make -f u2.mk VAR0=0 VAR4=4 all3

#`make -f u2.mk all31`'s result is exactly the same as `make -f u2.mk all33`. But it should be used as \
 recusive make should uses $(MAKE), not "make"
all30:;VAR5=5 ${MAKE} VAR2=2 -C sub -f a.mk
all31:;@${MAKE} -f u2.mk VAR0=0 VAR4=4 all30

ifneq ($(findstring all4,$(MAKECMDGOALS)),)
MAKEFLAGS+=--no-print-directory VAR3=file#MAKEFLAGS is passed down to child make automatically.
export SHELL=/bin/dash
endif

#for all4: $ make -f u2.mk all4 \
MAKELEVEL=0 MAKEFLAGS= --no-print-directory VAR3=file<--phase 1 of L0 make;VAR3 will be removed and put into environment in phase2.phase2's MAKEFLAGS only has --no-print-directory as no cmd var in L0 make\
MAKELEVEL=1 MAKEFLAGS= --no-print-directory<--phase 1 of L1 make, L0's VAR3 is env var of L1 make;VAR0&VAR4 will be put in MAKEFLAGS in phase 2 as they're defined in the cmd line\
VAR5=5 make VAR2=2 -C sub -f a.mk<--there will be no Entering/Leaving directory pair any more.L2 make starts: \
MAKE=make       default \
MAKEFLAGS= --no-print-directory file <--no VAR3=file, which has been removed by L0 make \
MAKELEVEL=2     environment \
MAKEFILES=      default \
SHELL=/bin/sh   file<--not changed even SHELL is replaced and exported by parent make \
VAR0=0  command line<--the same as the above's all33 \
VAR1=1  environment<--the same as the above's all33 \
VAR2=2  command line <--the same as the above's all33 \
VAR3=file       environment<--variables added explicitely in MAKEFLAGS becomes child make's environment \
VAR4=4  command line<--the same as the above's all33 \
VAR5=5  environment<--the same as the above's all33 \
make[2]: Nothing to be done for 'all'.
all4:all33
all44:$(info MAKELEVEL=${MAKELEVEL} MAKEFLAGS=$(MAKEFLAGS));@VAR5=5 ./fakemake.out VAR2=2 $(info MAKEFLAGS=$(MAKEFLAGS))

#for all44: $ make -f u2.mk VAR0=0 VAR4=4 all44|grep -n 'VAR\|MAKE\|SHELL'\
1:MAKELEVEL=0 MAKEFLAGS= --no-print-directory VAR3=file<--L0's phase1 value\
2:MAKEFLAGS= --no-print-directory -- VAR4=4 VAR0=0<--L0's phase 2 value:VAR3 is removed and cmd line variables are added \
4:1: VAR2=2 <--VAR2 is in sub-make's cmd line, so it should not be in parent transfered MAKEFLAGS \
5:VVVVARVVV pid 4784,ppid 4783 \
16:10: MAKEFLAGS= --no-print-directory -- VAR4=4 VAR0=0 <--a real L1 make's phase1 $(info $(MAKEFLAGS)) show only --no-print-directory, see all4's cooresponding line\
17:11: MAKE_TERMERR=/dev/pts/0 \
23:17: VAR0=0 <--L0 make will make cmd line variable exist in both env and MAKEFLAGS\
24:18: VAR1=1 <--L0 make exported\
26:20: VAR3=file <--L0 make removes VAR3 from MAKEFLAGS and put it into env of child process\
28:22: MAKELEVEL=1 <--L0 make will inc its MAKELEVEL to 1 then pass down it as env to child process\
30:24: VAR4=4 <--L0 make export VAR4, which also exists on L0 make cmd line. The later takes precedence and VAR4 is handled as cmd line variable so it exists in both env and MAKEFLAGS\
32:26: VAR5=5 <--env variable given when lauch child process\
34:28: MAKEOVERRIDES=${-*-command-variables-*-} \
35:29: SHELL=/bin/dash<-changed after L0 make's explicitely exported,but sub-make still use its own file variable "SHELL=/bin/sh"(`make -p` can show it)

#for a shell script, make will launch a shell to handle it.\
                  └─make,2850 -f u2.mk VAR0=0 VAR4=4 all2 \
                      └─sh,2851 -c sleep 30;[ -1 == 0 ] \
                          └─sleep,2852 30
all2:;@ sleep 30;[ -1 == 0 ]

#note there is no VAR3 among the following output, if a var is nither exported nor defined at cmd line, it will nither be seen in
#shell process's env nor args.Besides, there is no CURDIR in child process's env
#$ make -f u2.mk VAR0=0 VAR4=4 all|grep 'VAR\|MAKE\|SHELL\|CURDIR' \
MAKELEVEL=0 MAKEFLAGS= \
1: VAR2=2<--VAR2 is arg, will not be shown in env \
VVVVARVVV pid 2996,ppid 2994<--separator, the above is args and the below is env \
0: VAR5=5<--the var definination before elf is env! \
1: SHELL=/bin/bash<--make will not replace it with its own /bin/sh \
5: MAKEOVERRIDES=${-*-command-variables-*-} \
14: MAKEFLAGS= -- VAR4=4 VAR0=0<--the flags from cmd line \
19: MAKE_TERMERR=/dev/pts/0 \
22: MAKELEVEL=1<--auto +1 even the child process is not a real make, so any child process will get MAKELEVEL from envrionment \
24: VAR4=4<--if one var is both from cmd line and file's exported var, then cmd line takes precedence. \
25: VAR0=0<--command line arg becomes an env \
26: VAR1=1<--exported var becomes an env

