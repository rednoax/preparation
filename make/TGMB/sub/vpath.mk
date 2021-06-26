ifeq (0,1)
#the following format is WRONG, no =; that's why make can't find associated .c:\
$ make -f vpath.mk -k\
make: *** No rule to make target 'getline.o', needed by 'all'.\
make: *** No rule to make target 'abort.o', needed by 'all'.\
make: Target 'all' not remade because of errors.
vpath=%.c ~/opt/preparation
endif

ifeq (1,0)
srcs=geline.c abort.c#note it should be getline.c rather than geline.c\
so:\
$ make -f vpath.mk\
make: *** No rule to make target 'geline.o', needed by 'all'.  Stop.

#let's try -k to show its effect, it makes make go on tring other prerequisites even the 1st prerequisite fails:\
$ make -f vpath.mk -k\
make: *** No rule to make target 'geline.o', needed by 'all'.\
make: *** No rule to make target 'abort.o', needed by 'all'.<-- -k will got on make other prerequisite\
make: Target 'all' not remade because of errors.
endif

srcs=getline.c abort.c
objs=$(srcs:.c=.o)
#the above 2 files can only be found in:\
$ find ~/opt/preparation/ -name getline.c\
/home/rednoah/opt/preparation/DM-verity/getline.c\
$ find ~/opt/preparation/ -name abort.c\
/home/rednoah/opt/preparation/abort.c

all:$(objs)
#rm the generated .o, "Nothing to be done for 'all'" otherwise for the 2nd `make all` once $(objs) are generated.
	rm -rf $(objs)
#there is no extra %.o:%.c rules since we use implict rule %.o:%.c that can be shown by make -p:\
%.o: %.c\
#  recipe to execute (built-in):\
        $(COMPILE.c) $(OUTPUT_OPTION) $<
ifeq (1,1)
vpath %.c /home/rednoah/opt/preparation:../../../DM-verity#\
1.~/opt/preparation can't work at all, use complete home name instead.\
2.Besides, getline.c is at preparation/DM-vrity, if no the 2ns part, vpath will NEVER search\
 the sub directory of ~/opt/preparation. So the sub directory DM-verity MUST be added explictly.
else
vpath %.c ~/opt/preparation:/home/rednoah/opt/preparation/DM-verity
#another way to show -k option effect:\
the above can build getline.o but fail to build abort.o for the vpath'1st part use a unworkable ~,\
which make ~/opt/preparation/abort.c can't be found.\
$ make -f vpath.mk\
cc    -c -o getline.o /home/rednoah/opt/preparation/DM-verity/getline.c\
make: *** No rule to make target 'abort.o', needed by 'all'.  Stop.\
$ make -f vpath.mk -k\
make: *** No rule to make target 'abort.o', needed by 'all'.\
make: Target 'all' not remade because of errors.<-- -k generate one more error compared with the following w/t -k\
$ make -f vpath.mk\
make: *** No rule to make target 'abort.o', needed by 'all'.  Stop.
endif

all2:weak.o $(objs)
#cc    -c -o getline.o ../../../DM-verity/getline.c<--note its dir uses exactly the one vpath specified.\
cc    -c -o abort.o /home/rednoah/opt/preparation/abort.c\
rm -fr weak.o getline.o abort.o
	rm -fr $^
weak.o:weak.c#this rule will not affect other .o, implicit rule %.o:%.c works for all .o except weak.o
#[weak.o] [.] [weak.o]<--note $@ has no dir\
[/home/rednoah/opt/preparation/weak.c] [/home/rednoah/opt/preparation] [weak.c]<--$< has the found dir
	$(show_auto)

show_auto=@echo -e "[$@] [$(@D)] [$(@F)]\n[$<] [$(<D)] [$(<F)]"

#$ make -f vpath.mk all3\
[getline.o] [.] [getline.o]<-$@ has no dir, since they are generate in current dir, if any,\
[../../../DM-verity/getline.c] [../../../DM-verity] [getline.c]<--the dir use exactly the one vpath specified.
all3:getline.o
ifeq ($(MAKECMDGOALS),all3)
#the following pattern rule will remove the implicit %.o:%.c from `make -p`, and there\
 is only one %.o:%.c recipe in `make -p`:\
%.o: %.c\
#  recipe to execute (from 'vpath.mk', line 76):\
        $(show_auto)

%.o: %.c
	$(show_auto)
endif