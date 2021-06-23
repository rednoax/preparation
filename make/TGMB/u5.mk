#TO TEST:
#1.https://stackoverflow.com/questions/33159342/why-dont-environment-variables-work-in-shell-commands
#bugs: http://savannah.gnu.org/bugs/?func=detailitem&item_id=10593
#2.table A, see below
#3.no matter if $(shell ...) is located in a rule or not, it is a child process of parent process make(denoted as P),\
 eg the following 2 sites of $(shell ...) calling, P will launch a respective(各自的,分别的) child process for them.
#4.the commands of each line in rules, including sub make via make -c, is an individual child process launched by parent process make P.
#5.for all child processes in 3 and 4, parent process P make replaces $$ with $, $(var) ,then P fork+exec a child process with the replaced.
#6.$(shell ...)'s child process uses P's env list, no added nor removed.
#7.child process in rules uses P's env list(exported before P launched or defined as one time env when P launched),\
 plus cmd line var when P launched, plus exported file var in makefile. \
7.1 if a file var is not exported in makefile, to make child process see it. You can define it as env when P launched\
 so that the child process can see it with file var's value.IT'S A BIT TRICKY.\
7.2 A cmd line var of L0 make will be passed down to rule's child process, its value uses L0 make' cmd\
 line value even L0 make has overriden its value in makefile or redefined+exported it.
#8. if one var of the same name exists in both P's env/cmdLineVar and makefile's defination, whar's the var's value \
 in child process's env, refer table A for details.
#9. the same name var's precedence: override > cmd line var > -e > file var > env var.
#10. If a env var is redefined in make, even the file type variable not exported, its new value will be passed down to child process' env.
export HELLO=Hello,world
# the env used by $(shell) is exactly the same as the env of make's parent process, ie the process which launched make,
# which has no $HELLO defined
$(info $(shell echo $$$$ PPID $$PPID $(HELLO) $$HELLO))#the last is replaced by make with \
 $HELLO then passed down to the child process, which use P'env so the child process can't show $HELLO.\
 If putting $(shell ...) in recipe the result is exactly the same.
all:
	@echo $(HELLO) $$$$ PPID $$PPID
	@echo $(shell echo [$$$$ PPID $$PPID $(HELLO) $$HELLO])
	@echo [$$HELLO $$$$ PPID $$PPID]
	@./fakemake.out

e0=f0#e0 is neither exported nor overriden
export e1=f1
override e2=f2

c0=f0
export c1=f1
override c2=f2

#table A
#table content: the visbilty of child process launched(NOT via $(shell ...)) by P
#					env of P(either exported before P launched or defined as one time var when P launched)			P's cmd line var
#unvarnished file var: <-, in child process's env																	/|\,in child process's env and MAKEFLAGS
#exported file var:	   <-, in child process's env																	/|\,in child process's env and MAKEFLAGS
#overriden file var:   <-, in child process's env																	invisible in child process's env, but if child is make, the child make will recognize it as a cmd var(from MAKEFLAGS)and whose value is L0 make's cmd line.\
The reason is in MAKEFLAGS's value, see below's "MAKEFLAGS= -- c2=2 c1=1 c0=0"

#$ e0=0 e1=1 e2=2 make c0=0 c1=1 c2=2 -f u5.mk<--when make launched, it is the parent process IN THE TOP and it will read the makefile from scratch, we use P to denote it.From the following output, P's pid==3105
#3106 PPID 3105 Hello,world<--$(shell ...) function not in a rule is a child process of P, denoted as C1. make process P will REPLACE $$ and variable defination like $(var) before launch C1, whose pid is 3106.C1's env is the same as P, no added nor deleted, so $$HELLO is empty
#Hello,world 3108 PPID 3105<--each cmd in rules is a child process launched by P. make process P will REPLACE $$ and variable defination like $(var) before launch child process
#[3107 PPID 3105 Hello,world ]<--the effect and result of $(shell ...) function in a rule is the same as the above of $(shell ...) function not in a rule, so $$HELLO is empty
#[Hello,world 3110 PPID 3105]<--each cmd in rules is a child process launched by P.the child's env comes from:1.inherited from P, which can be exported before P is launched or defined as one time env when P launched, eg e0,e1,e2;2.cmd line var when P launched,eg c0;3.exported file var in makefile. See table A for var's value of the same name.
#0: ./fakemake.out
#VVVVARVVV
#0: SHELL=/bin/bash
#1: _=/usr/bin/make
#2: c0=0<---command line var takes precedence with unvarnished file var
#3: e0=f0<---unvarnished file var takes precedence with env var
#4: XDG_DATA_DIRS=/usr/local/share:/usr/share:/var/lib/snapd/desktop
#5: LESSOPEN=| /usr/bin/lesspipe %s
#6: LC_NAME=zh_CN.UTF-8
#7: SSH_CONNECTION=192.168.193.1 62294 192.168.193.128 22
#8: c1=1<--command line var takes precedence with exported file var
#9: PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
#10: e1=f1<---expported file var takes precedence with env var
#11: SSH_TTY=/dev/pts/1
#12: XDG_RUNTIME_DIR=/run/user/1000
#13: HELLO=Hello,world<--exported file var becomes env of child process
#14: LC_NUMERIC=zh_CN.UTF-8
#...
#17: HOME=/home/rednoah
#18: PWD=/home/rednoah/preparation/make/TGMB
#19: e2=f2<--override file var takes precedence with env var
#20: DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus
#21: LESSCLOSE=/usr/bin/lesspipe %s %s
#22: LOGNAME=rednoah
#23: JACK_SERVER_VM_ARGUMENTS=-Dfile.encoding=UTF-8 -XX:+TieredCompilation -Xmx4g
#24: LC_TIME=zh_CN.UTF-8
#25: SHLVL=1
#26: MAKEOVERRIDES=${-*-command-variables-*-}
#27: XDG_SESSION_ID=5
#28: USER=rednoah
#29: LC_MONETARY=zh_CN.UTF-8
#30: LC_TELEPHONE=zh_CN.UTF-8
#31: OLDPWD=/home/rednoah/preparation/bash
#32: MAKEFLAGS= -- c2=2 c1=1 c0=0<--override file var NOT take precedence with command line var but make it disappeared from child process's env list
#33: MFLAGS=
#34: SSH_CLIENT=192.168.193.1 62294 22
#35: MAIL=/var/mail/rednoah
#36: MAKE_TERMOUT=/dev/pts/1
#37: LC_PAPER=zh_CN.UTF-8
#38: LC_MEASUREMENT=zh_CN.UTF-8
#39: LC_IDENTIFICATION=zh_CN.UTF-8
#40: MAKE_TERMERR=/dev/pts/1
#41: LC_ADDRESS=zh_CN.UTF-8
#42: LANG=C
#43: TERM=xterm
#44: MAKELEVEL=1<--child process's MAKEFLAGS is 1 while parent process, ie P's MAKEFLAGS is 0


#$ make -f u5.mk all22 \
2975 PPID 2974 Hello,world \
VAR0=e0 make -f u5.mk VAR1=c1 all2_base<-L1 make process launched, no Entering/Leaving pair as L0 make has passed down --no-print-directory\
2977 PPID 2976 Hello,world Hello,world \
make -C sub -f a.mk <-L2 make process launched, the following is printed by L2 make sub/a.mk\
MAKE=make       default \
MAKEFLAGS= --no-print-directory file \
MAKELEVEL=2     environment \
MAKEFILES=      default \
SHELL=/bin/sh   file \
VAR0=f01        environment<--see below A\
VAR1=c1 command line<--see below B\
VAR2=   undefined \
VAR3=   undefined \
VAR4=   undefined \
VAR5=   undefined \
make[2]: Nothing to be done for 'all'.
ifneq ($(findstring all2,$(MAKECMDGOALS)),)
VAR0=f0${MAKELEVEL}#A:if $(origin VAR0) is environment before this assignment, then even not exported, the child process's env will\
 use the file type variable's value.
override VAR1=f1${MAKELEVEL}#B:the value after overriding a cmd line var is used only in its own make, \
 the child process will still use the value assigned in the cmd line
MAKEFLAGS+=--no-print-directory
all2_base:
	$(MAKE) -C sub -f a.mk
all2:
	VAR0=e0 $(MAKE) -f u5.mk VAR1=c1 all2_base
endif

#$ make -f u5.mk all3 \
4161 PPID 4160 Hello,world \
VAR0= \
VAR1=f10<--in L0make, only VAR1 and VAR3 among VARx are defined \
VAR2= \
VAR3=f30<--in L0make, only VAR1 and VAR3 among VARx are defined \
VAR4=f4{MAKELEVEL} \
VAR0=e0 VAR1=e1 make -e -f u5.mk VAR2=c2 VAR3=c3 VAR4=c4 all3_base \
make[1]: Entering directory '/home/rednoah/opt/preparation/make/TGMB'<--L1 make launched\
4163 PPID 4162 Hello,world Hello,world \
VAR0=e0<-VAR0 is L1 make's env from cmd line \
VAR1=e1<-VAR1 both from L1 make'e env in cmd line and makefile, -e said the former wins \
VAR2=c2<-VAR2 is L1 make's cmd line var\
VAR3=c3 f31<-VAR3 is L1 make's cmd line var, but is override += in makefile\
VAR4=c4<-VAR4 is L1 make's cmd line var, which precedences the file var(its += can't be appended to cmd line value c4\
1:ew -- VAR4=c4 VAR3=c3 VAR2=c2<-phase2 value:all cmd line var will be passed down with cmd line value, no matter if redefined/overrided in makefile \
0: ./fakemake.out \
VVVVARVVV pid 4164,ppid 4162 \
...\
2: SHELL=/bin/bash <--SHELL is special, make's own file var /bin/sh will not affect env unless exported.if the child process is a real make, which will use /bin/sh and not affected by the env's $SHELL\
...\
7: c1=f1<--parent will make its exported var be child's env var\
...\
9: e1=f1<--ditto\
...\
12: HELLO=Hello,world<-ditto\
...\
20: VAR0=e0<--one time env var defined at L1 make's cmd line\
21: SHLVL=1\
22: MAKEOVERRIDES=${-*-command-variables-*-}<--real value see above's phase 2 value:VAR4=c4 VAR3=c3 VAR2=c2\
...\
25: VAR1=e1<-one time env var defined at L1 make's cmd line wins for L1 make has -e\
...\
27: MAKEFLAGS=ew -- $(MAKEOVERRIDES)<-- note there is e, w means --print-directory\
28: MFLAGS=-ew\
...\
30: MAKE_TERMOUT=/dev/pts/0\
31: VAR4=c4<--L1 make cmd line var, which is not overridden but redefined in makefile, it also exists as env var of L2 make\
32: VAR2=c2<--L1 make cmd line var, which is not overridden or redefined in makefile so it also exists as env var of L2 make\
33: MAKE_TERMERR=/dev/pts/0\
...\
37: MAKELEVEL=2<--as "fakemake.out" is at L2 make level\
make[1]: Leaving directory '/home/rednoah/opt/preparation/make/TGMB'<--note there is no VAR3 in env, maybe due to it is overridden in L1's makefile
ifneq ($(findstring all3,$(MAKECMDGOALS)),)
${info ${MAKEFLAGS})}
VAR1=f1${MAKELEVEL}
override VAR3+=f3${MAKELEVEL}#see above B
VAR4+=f4{MAKELEVEL}#When no override, VAR4 is c4 in L1's make even claimed with +=

$(foreach v,0 1 2 3 4,$(info VAR$v=$(VAR$v)))

all3_base:
#	@bash #for interactive env dump by hand,use ^D to exit to run the following ./fakemake.out
	@./fakemake.out${info $(MAKELEVEL):$(MAKEFLAGS)}
all3:
#	@bash${info $(MAKEFLAGS)]}
	VAR0=e0 VAR1=e1 ${MAKE} -e -f u5.mk VAR2=c2 VAR3=c3 VAR4=c4 all3_base${info [$(MAKEFLAGS)]}
#a. $ `make -f u5.mk all3 VAR5=5` shows 'ew -- VAR4=c4 VAR3=c3 VAR2=c2 VAR5=5' in L1 make phase1\
VAR[2-4] from L1 make cmd line while VAR5 from L0 make cmd line. So every level's\
cmd line var,ie L[0-N] will be inherited by the L(N+1)child process via MAKEFLAGS prepared by its parent make.
#b. MAKEFLAGS in L0 make phase2 is still empty by above 2 info debug. but in L1 make phase1 it starts to be \
ew, maybe L1 make see MAKELEVEL==1 and no --no-print-directory in MAKEFLAGS, so it adds w with cmd option -e. \
So no matter if there is -C in sub-make, -w optiont is added to MAKEFLAGS automatically ALA a child make process is launched.
endif