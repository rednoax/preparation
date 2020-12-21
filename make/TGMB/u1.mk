a0=0

#value will be:var0, strlen 4
varname=var0# # 

#value wil be var0<space>, strlen==5
varname2=var0 #
#value:var3<space>a=b<space>
varname3=var3 a=b #

$(info [$(varname)] [$(varname2)] [$(varname3)]$(shell pwd))
all all0:
	@echo ${var} $(a0) $$var
	make -C sub -f sub.mk
$(varname):;
#$$PPID is this make's PID, and $$$$ is the latched child process echo
$(info $(shell echo $$PPID $$$$) $(CURDIR) $(PWD) $(shell pwd) $(MAKELEVEL))#$(CURDIR) is the same as pwd result
include dummy.mk

#rednoah@lucia:~/preparation/make$ pstree -AUp|grep make
#           │            ├─sshd(5678)───sshd(5798)───bash(5801)───make(6244)───make(6249)───sleep(6252)<--the 2nd make is the child process launched by top make

#rednoah@lucia:~/preparation/make$ make -C TGMB/ -f u1.mk
#make: Entering directory '/home/rednoah/preparation/make/TGMB'
#[var0] [var0 ] /home/rednoah/preparation/make/TGMB
#6244 6246 /home/rednoah/preparation/make/TGMB /home/rednoah/preparation/make /home/rednoah/preparation/make/TGMB 0
#0
#make -C sub -f sub.mk
#make[1]: Entering directory '/home/rednoah/preparation/make/TGMB/sub'
#sub.mk:4: ""
#''
#6249 6250 /home/rednoah/preparation/make/TGMB/sub /home/rednoah/preparation/make/TGMB/sub /home/rednoah/preparation/make 1
#make[1]: 'all' is up to date.
#make[1]: Leaving directory '/home/rednoah/preparation/make/TGMB/sub'
#make: Leaving directory '/home/rednoah/preparation/make/TGMB'
