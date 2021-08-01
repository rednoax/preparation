ifeq ($(MAKECMDGOALS),all.out)
$(info [$(MAKE),$(MAKEFLAGS),$(CURDIR)==$(shell pwd)])
endif

.PHONY:all.out all4.out all8.out

#$ make -f nqt.mk --trace -t all.out \
[make,t --trace,/home/rednoah/opt/preparation/make/TGMB/sub==/home/rednoah/opt/preparation/make/TGMB/sub] \
nqt.mk:4: target 'all.out' does not exist \
echo  0      # ; date >timestamp \
0
#$ make -f nqt.mk -q all.out;echo $? \
[make,q,/home/rednoah/opt/preparation/make/TGMB/sub==/home/rednoah/opt/preparation/make/TGMB/sub] \
1<--$? not echo $(MAKELEVEL), the latter is not run as it is not the 1st line
all.out:
	echo 'ARCH=$(ARCH) from $(origin ARCH) HOST=$(HOST) from $(origin HOST)'
	+echo  $(MAKELEVEL)      # ; date >timestamp

#$ make -f nqt.mk --trace -t all2.out\
touch all2.out<--touch happen\
rednoah@ubuntu:~/opt/preparation/make/TGMB/sub$ make -f nqt.mk --trace -t all2.out\
make: 'all2.out' is up to date.<--touch will not happen
#$ make -f nqt.mk -q all2.out;echo $? \
1
all2.out:
	:
#$ make -f nqt.mk  -t all3.out\
make: Nothing to be done for 'all3.out'.
#$ make -f nqt.mk -q all3.out;echo $? \
0
all3.out:

#$ make -f nqt.mk  -t all4.out \
echo all4.out \
all4.out
#$ make -f nqt.mk -q all4.out;echo $? \
echo all4.out \
all4.out \
0<--the only recipe has been finished so all4.out is not needed to be updated?
all4.out:
	+echo $@

#$ make -f nqt.mk  -t all8.out\
echo\

#$ make -f nqt.mk -q all8.out;echo $?\
1<--+echo not run since it is not the 1st line.

all8.out:
	:
	+echo

#$ make -f nqt.mk  -t all9.out \
echo\
\
touch all9.out\
touch all9.out\
$ make -f nqt.mk  -t all9.out\
make: 'all9.out' is up to date.
#$ make -f nqt.mk -q all9.out;echo $? \
echo <--+echo is run for it is the 1st line\
\
1
all9.out:
	+echo
	:

#$ make -f nqt.mk  -t all5.out\
make: Nothing to be done for 'all5.out'.
#$ make -f nqt.mk -q all5.out;echo $? \
0
all5.out:nqt.mk

#$ make -f nqt.mk  -t all6.out \
touch all6.out \
$ make -f nqt.mk  -t all6.out \
make: 'all6.out' is up to date.
#$ make -f nqt.mk -q all6.out;echo $? \
1
all6.out:nqt.mk
	:

#$ make -f nqt.mk  -t all7.out \
[ 1 == 1 ]<--all7.out is not touched as expected, REASON unknown!
#$ make -f nqt.mk -q all7.out;echo $? \
[ 1 == 1 ] <--+ line is run for it is the 1st line\
0<--the only recipe has been finished so all7.out is not needed to be updated?
all7.out:
	+[ 1 == 1 ]

#what matters for -t: 1. if target is .PHONY; 2. if the recipe is empty. \
a.target is PHONY, no matter if the recipe is empty: the target will not be touched. \
b.target is not PHONY but there is no recipe: target is not touched since "Nothing to be done" for that target\
c.target is not PHONY and there is recipe: target is touched.

#what matters for -q: 1.if +line is the 1st, if not it will not be run. 2.if the target is needed to be rebuild:\
 a. no such file and the recipe non empty; b. old than prerequisite; c.target is PHONY \
 then make return 1, 0 otherwise. For 2, at present there is no way to generate it.
