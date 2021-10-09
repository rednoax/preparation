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

define echos1#[1 2]
echo 1;
echo 2
endef
define echos2#[1 echo 2]
echo 1
echo 2
endef
varname4=$(shell $(echos1))
v5=$(shell $(echos2))

$(info [$(varname4)] [$(v5)])#[1 2] [1 echo 2]
$(info [$(shell echo 3;echo 4)])#[3 4]
$(info [$(shell echo 4 echo 5)])#[4 echo 5]

all all0:
	echo ${var} $(a0) $$var
	make -C sub -f sub.mk
#the above ${var} is equivalent to $(var), which will be replaced by make before transfering to shell
#the $$var will be replaced with $var by make(2$ will becomes 1$), then shell will see "echo 0 $var"
$(varname):;
#$$PPID is this make's PID, and $$$$ is the launched child process echo;
#note that # below after $(info ...) will be recognized as comments
$(info $(shell echo $$PPID $$$$) $(CURDIR) $(PWD) $(shell pwd) $(MAKELEVEL))#$(CURDIR) is the same as pwd result,$(PWD) is from env
$(info **1)
MAKEFLAGS+=-rR
#1.dummy.mk recipe will not run to 'update' dummy.mk as ni prerequisite!
# even dummy.mk is newer than u1.mk, eg touch dummy.mk,there is no 2nd parsse of u1.mk.
#dummy.mk:; touch $@
#2.dummy.mk recipe is AL run during each make process(due to FORCE is prerequisite)but no 
# the 2nd parsing, ie no **1/1.5/2 emit for the 2nd time. Even dummy.mk is newer than u1.mk before `make -f u1.mk` and
# the following `echo $@` runs, but there is no 2nd parse of u1.mk
#So it seems: when the included file exists, here it is'dummy.mk', make will record
# the included file's timestamp when including it. Then if there is some rules to update the file, 
# after 'updating' dummy.mk, ie a pure echo that will not change dummy.mk's timestamp, make will compare 2 timestamps. If the latter is
# not newer than the 1st, no 2nd reparse of make at all.
#dummy.mk:FORCE;echo $@
#3.It causes infinite loops!
#dummy.mk:FORCE;touch $@;sleep 1
#4. a sane way
dummy.mk:u1.mk;touch $@
#if u1.mk is newer than dummy.mk, the dummy.mk is 'updated' to be newer than u1.mk then 2nd parse happens(**1/1.5/2 emitted)
#if u1.mk is older than dummy.mk, the dummy.mk is not touched and there is no 2nd parse at all.
#5. keep 4 open then `touch dummy.mk` to make it newer than u1.mk then enable the following 1 line. Then `make -f u1.mk`:\
 the dummy.mk is touched once indeed then but no 2nd parse of u1.mk at all as dummy.mk's ts is not changed in its recipe?
#$(info =>$(shell touch dummy.mk))
#6. keep 4 open then `touch u1.mk` to make it newer than dummy.mk then enable the above 1 line. Then `make -f u1.mk`:\
 the dummy.mk is touched so it becomes newer than u1.mk, the recipe in 4 will not run so there will be no 2nd parse of u1.mk.
.PHONY:FORCE
include dummy.mk
#when the following line enabled, no matter if dummy.mk is newer than u1.mk before `make -f u1.mk`:\
  dummy.mk is touched to be newer than u1.mk, there will be no 2nd parse of u1.mk
#$(warning $(shell touch dummy.mk))

#1~6 summarization\
u1.mk contains 'include dummy.mk'. When dummy.mk has existed before `make -f u1.mk`. \
Whether there is 2nd parse of u1.mk is NOT triggered by: \
1. The condition that dummy.mk is newer than u1.mk.\
2. The dummy.mk ts is changed to become newer, but the change occurs out of dummy.mk rule's recipe, not by dummy.mk's recipe.\
  eg $(shell touch dummy.mk)\
3. No matter if dummy.mk is newer than u1.mk: dummy.mk has rules but whose recipe will not change dummy.mk's ts after the recipe fins.\
Whether there is 2nd parse of u1.mk is triggered by:\
1. dummy.mk must own a recipe to run to change the dummy.mk's ts.\
2. The result of recipe must really change dummy.mk's timestamp.\
3. The key seems to compare 2 timestamp, the ts when dummy.mk is included and the ts after its recipe fins.\

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
$(info **2)
t1 t2:
	@echo $@