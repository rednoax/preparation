#$ rm -rf .depend &&make -f cpu.mk\
rules.mk:3: rules.mk included\
cpu.mk:4: cpu.mk included<--even rules.mk has told how to generate .dpend but "sinclude .depend" will not try to generate .depend until the whold cpu.mk is parsed\
echo '$(warning .depended included)'>.depend\
rules.mk:3: rules.mk included\
.depend:1: .depended included\
cpu.mk:4: cpu.mk included\
make: 'all' is up to date.

#$ rm -rf .depend&&make -f cpu2.mk<--there is no warning in make 4.2.1 if the included file in 1st parse has not existed\
rules.mk:3: rules.mk included\
cpu2.mk:13: cpu.mk included\
echo '$(warning .depended included)'>.depend\
rules.mk:3: rules.mk included\
.depend:1: .depended included\
cpu2.mk:13: cpu.mk included\
make: 'all' is up to date.

all:;
include rules.mk
include .depend
$(warning cpu.mk included)