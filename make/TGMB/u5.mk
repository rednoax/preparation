#TO TEST:
#1.https://stackoverflow.com/questions/33159342/why-dont-environment-variables-work-in-shell-commands
#bugs: http://savannah.gnu.org/bugs/?func=detailitem&item_id=10593
#2.table A, see below
#3.no matter if $(shell ...) is located in a rule or not, it is a child process of parent process make(denoted as P), eg the following 2 sites of
#  $(shell ...) calling, P will launch a respective child process for them.
#4.the commands of each line in rules, including sub make via make -c, is an individual child process launched by parent process make P.
#5.for all child processes in 3 and 4, parent process P make replaces $$ with $, $(var) ,then P fork+exec a child process with the replaced.
#6.$(shell ...)'s child process uses P's env list, no added nor removed.
#7.child process in rules uses P's env list(exported before P launched or defined as one time env when P launched),plus cmd line var when P launched,
#  plus exported file var in makefile.
#7.1 if a file var is not exported in makefile, to make child process see it. You can define it as env when P launched so that the child process can
#  see it with file var's value.
#7.2 to prevent a cmd line var from going to rule's child process, define it as override file var in makefile.
#8. if one var of the same name exists in both P's env/cmdLineVar and makefile's defination, whar's the var's value in child process's env,
#   refer table A for details.
#9. the same name var's precedence: override > cmd line var > -e > file var > env var.

export HELLO=Hello,world
# the env used by $(shell) is exactly the same as the env of make's parent process, ie the process which launched make,
# which has no $HELLO defined
$(info $(shell echo $$$$ PPID $$PPID $(HELLO) $$HELLO))
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
#unvarnished file var: <-, in child process's env																	/|\,in child process's env
#exported file var:	   <-, in child process's env																	/|\,in child process's env
#overriden file var:   <-, in child process's env																	invisible in child process's env

#rednoah@lucia:~/preparation/make/TGMB$ e0=0 e1=1 e2=2 make c0=0 c1=1 c2=2 -f u5.mk<--when make launched, it is the parent process IN THE TOP and it will read the makefile from scratch, we use P to denote it.From the following output, P's pid==3105
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
#32: MAKEFLAGS= -- c2=2 c1=1 c0=0<--override file var takes precedence with command line var and it disappeared from child process's env list
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
