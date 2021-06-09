#The comments can cross multi lines \
TO TEST: \
1. there is = and <space> in value \
2. one make process launch a child make process

a0=0

#value will be:var0, strlen 4, the chars from the first # will all be recognised as comments
varname=var0# # 

#value wil be var0<space>, strlen==5, the chars from the first # will all be recognised as comments
varname2=var0 #
#value:var3<space>a=b<space>, the chars from the first # will all be recognised as comments
varname3=var3 a=b #

$(info [$(varname)] [$(varname2)] [$(varname3)]$(shell pwd))

all all0:
	@echo ${var} $(a0) $$var
	make -C sub -f sub.mk
#the above ${var} is equivalent to $(var), which will be replaced by make before transfering to shell
#the $$var will be replaced with $var by make(2$ will becomes 1$), then shell will see "echo 0 $var"
$(varname):;
#$$PPID is this make's PID, and $$$$ is the launched child process echo;
#note that # below after $(info ...) will be recognized as comments
$(info $(shell echo $$PPID $$$$) $(CURDIR) $(PWD) $(shell pwd) $(MAKELEVEL))#$(CURDIR) is the same as pwd result
include dummy.mk

#rednoah@lucia:~/preparation/make$ pstree -AUp|grep make
#           │            ├─sshd(5678)───sshd(5798)───bash(5801)───make(6244)───make(6249)───sleep(6252)<--the 2nd make is the child process launched by top make, the 2nd make will paused at sleep 100 so I can use pstree to find it.

#rednoah@lucia:~/preparation/make$ make -C TGMB/ -f u1.mk
#make: Entering directory '/home/rednoah/preparation/make/TGMB'
#[var0] [var0 ] /home/rednoah/preparation/make/TGMB
#6244 6246 /home/rednoah/preparation/make/TGMB /home/rednoah/preparation/make /home/rednoah/preparation/make/TGMB 0
#0
#make -C sub -f sub.mk
#make[1]: Entering directory '/home/rednoah/preparation/make/TGMB/sub'
#sub.mk:4: ""<--$(warning ...) has file name and line# while $(info ...) has no such
#''<--$(info ...) has no file name/line#
#6249 6250 /home/rednoah/preparation/make/TGMB/sub /home/rednoah/preparation/make/TGMB/sub /home/rednoah/preparation/make 1
#make[1]: 'all' is up to date.
#make[1]: Leaving directory '/home/rednoah/preparation/make/TGMB/sub'
#make: Leaving directory '/home/rednoah/preparation/make/TGMB'
