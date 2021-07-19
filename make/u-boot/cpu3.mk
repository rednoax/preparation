#$ rm -rf .depend &&make -f cpu.mk\
rules.mk:3: rules.mk included\
cpu.mk:4: cpu.mk included<--even rules.mk has told how to generate .dpend but "sinclude .depend" will not try to generate .depend until the whold cpu.mk is parsed\
echo '$(warning .depended included)'>.depend\
rules.mk:3: rules.mk included\
.depend:1: .depended included\
cpu.mk:4: cpu.mk included\
make: 'all' is up to date.

#$ make -f cpu3.mk all\
rules.mk:3: rules.mk included\
cpu3.mk:13: cpu.mk included\
cpu3.mk:12: .depend2: No such file or directory<--if the included file not exist, include xx will stop make's parse at once so there is no info like the 2nd line prined\
make: *** No rule to make target '.depend2'.  Stop.\

ifeq ($(MAKECMDGOALS),all)
all:;
include rules.mk
include .depend2
$(warning cpu.mk included)
endif

#$ make -f cpu3.mk all2\
rules.mk:3: rules.mk included\
cpu3.mk:28: cpu.mk included<--no rule to generate .depend2 but the make will go on\
make: 'all2' is up to date.

ifeq ($(MAKECMDGOALS),all2)
all2:;
include  rules.mk
sinclude .depnd2
$(warning cpu.mk included)
endif