myenv0=0
myenv1=1
export myenv1

$(warning ==$(shell echo "$$PID,$$PPID")==)#$PID AL empty and $$PPID is actually ==(make's pid),DON'T know why!

#make -f module.mk myenv2=2,there is myenv1/2 but no myenv0 displayed
#so top make's cmd var and its -f specified file's exported var can be found in any level's child process launched by top make
.PHONY: all
all:
	./dump_env.out

