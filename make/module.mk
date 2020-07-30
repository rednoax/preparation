iterator=$(foreach v, $1, $(warning $v==$($v)))
myenv0=0
myenv1=1
export myenv1
$(warning myenv2 from "$(origin myenv2)") #the 2nd round run:make -f module.mk test will still show myenv2 from "command line"
#ifeq ($(origin myenv2),command line)
ifneq ($(MAKECMDGOALS),test)
$(shell ./dump_env.out dummy>out.out)#the non built-in cmd running will show its parent is not make, but make's child process:sh
#so $(shell ...) is actually a make's fork then exec("sh",...), if the cmd to be run is not built in, then the child process "sh"
#will fork then exec to run this command
#$(shell cat /tmp/out.out)
endif

$(warning ==$(shell echo "pid $$$$, ppid $$PPID")==)#$PID AL empty, use $$ instead! $$PPID is actually ==(make's pid(by ps aux))!
#make <-make's own pid is displayed in its child process's $PPID
#  |->child process sh, the echo is builtint so sh directly run cmd in $(shell ...) <-child pid is displayed via $$

#make -f module.mk myenv2=2,there is myenv1/2 but no myenv0 displayed
#so top make's cmd var and its -f specified file's exported var can be found in any level's child process launched by top make

.PHONY: all
all:
	pstree -aps $$$$ #use tree to show make's own pid, the cmd is first fork+exec "sh" then fork+exec "pstree"
	./dump_env.out #make will launch a child process to run dump_env.out and pass myenv1 and myenv2 as env variable to this child
	make -f module.mk test #this make is a child process of "sh" which is fork+exec from top make
$(call iterator, myenv0 myenv1 myenv2)#myenv1 & myenv2 can pass to any level's child process so they has value to display
test:
	ps -f $$$$ #top make fork+exec "sh", which fork+exec "ps ..." so  $$$$ here is actually the "sh"'s pid,NOT "ps ..."'s pid;
	pstree -aps $$$$ #U can prove this make is NOT the lowest level's make's child process, but the lowest make's child "sh"'s child process
	@echo myenv2 is $(myenv2) from $(origin myenv2)
	echo $$$$ $$PPID #echo is builtin cmd so it is run directly by sh that is fork+exec by lowest level's make?
	./dump_env.out #there is no "sh" between ./dump_env.out and the lowest level's make, DON'T KNOW WHY!
#
#summarization
#1.shell cmd has 2 forms 1.$(shell ...) 2.cmds in recipes;
#these shell cmd is launched by make. And they can be make's direct child or this make's child process "sh"'s direct child process;
#We need not to care if the running cmd is make's direct child but just remember it is a child process(can be direct/indirect child) of make that launched the cmd
#2.top level's make can generate sub process make, they are in a process tree and the higher is parent and the lower is child(can be direct/indirect, we don't care if it is direct but just remember the child is at lower level and it is launched by top make)
#eg:
#                  `-make,8283 -f module.mk myenv2=2
#                      `-sh,8297 -c make -f module.mk test #this make is a child process of "sh" which is fork+exec from top make
#                          `-make,8298 -f module.mk test
#the sub lower make will inherit top make's exported vars and cmd line vars!
#