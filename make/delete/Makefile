#If make gets a fatal signal while a shell is executing, it may delete target file if the target file's\
last-modification time has changed since make first checked it.
#$ make signal \
touch signal \
sleep 10 \
^\make: *** Deleting file 'signal' <====target auto deleted\
make: *** [Makefile:4: signal] Quit (core dumped)

#.PHONY:signal#if exists, then `make signal` will not delete signal file if get ^+c
signal:
	touch $@
	sleep 10
#.DELETE_ON_ERROR:#when `make signal` is interrupted by signal, target is auto deleted, no need to use .DELETE_ON_ERROR

ifeq ($(MAKECMDGOALS),delete)
#$ make clean &&make delete \
rm -rf signal delete nodelete continue\
touch delete\
false\
make: *** [Makefile:18: delete] Error 1<===\
make: *** Deleting file 'delete'

delete:
	touch $@
	false
.DELETE_ON_ERROR:
endif

#make will not delete the target and stop after 'false'.
#$ make clean &&make nodelete \
rm -rf signal delete nodelete continue\
touch nodelete\
false\
make: *** [Makefile:37: nodelete] Error 1
nodelete:
	touch $@
	false
	:
#$ make clean &&make continue \
rm -rf signal delete nodelete continue \
touch continue \
false\
make: [Makefile:42: continue] Error 1 (ignored)<==cmd after : is run continually\
:

continue:
	touch $@
	-false
	:

files=signal delete nodelete continue
clean:
	rm -rf $(files)