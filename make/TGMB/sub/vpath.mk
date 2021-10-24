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

###HOW to remove implicit rule %.o:%.c;$(COMPILE.c) $(OUTPUT_OPTION) $<###
#when NO all3 in MAKECMDGOALS\
# Implicit Rules\
\
../%.o: %.c<--will not remove implicit %.o:%.c \
#  recipe to execute (from 'vpath.mk', line 102):\
        $(show_auto);rm -rf $@\
\
obj/%.o: %.c<--will not remove implicit %.o:%.c \
#  recipe to execute (from 'vpath.mk', line 107):\
        $(show_auto);rm -rf $@\
\
%.o: %.c<--implicit %.o:%.c \
#  recipe to execute (built-in):\
        $(COMPILE.c) $(OUTPUT_OPTION) $<\

#when all3(to REMOVE a specific implicit rule, both target and prerequisite must be exactly the same):\
%.o: %.c<--will remove implicit %.o:%.c \
#  recipe to execute (from 'vpath.mk', line 85):\
        $(show_auto)\
\
../%.o: %.c<--will not remove implicit %.o:%.c \
#  recipe to execute (from 'vpath.mk', line 102):\
        $(show_auto);rm -rf $@\
\
obj/%.o: %.c<--will not remove implicit %.o:%.c \
#  recipe to execute (from 'vpath.mk', line 107):\
        $(show_auto);rm -rf $@\
###FIN###
#$ make -f vpath.mk all\
cc    -c -o getline.o ../../../DM-verity/getline.c\
cc    -c -o abort.o /home/rednoah/opt/preparation/abort.c\
rm -rf getline.o abort.o
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
2.Besides, getline.c is at preparation/DM-vrity, if no the 2nd part, vpath will NEVER search\
 any sub directory of ~/opt/preparation. So the sub directory DM-verity MUST be added explictly.
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

all2:weak.o $(objs)#note all generated %.o is in current dir!Not its org dir!
#cc    -c -o getline.o ../../../DM-verity/getline.c<--note its dir uses exactly the one vpath specified.\
cc    -c -o abort.o /home/rednoah/opt/preparation/abort.c\
rm -fr weak.o getline.o abort.o
	rm -fr $^

#If more than one pattern rule meets, make will choose the rule with the shortest stem(that is, the pattern\
that matches most specifically)
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
#the following pattern rule will REMOVE the implicit %.o:%.c from `make -p`, and there\
 is only one %.o:%.c recipe defined by me in `make -p`:\
%.o: %.c\
#  recipe to execute (from 'vpath.mk', line 76):\
        $(show_auto)
%.o: %.c
	$(show_auto)
endif

#$ make -f vpath.mk all4\
cc    -c -o abort.o /home/rednoah/opt/preparation/abort.c<--my rule ../%.o:%.c not work,implicit rule %.o:%.c takes effect\
$ make -f vpath.mk all4\
make: Nothing to be done for 'all4'.<-as ./abort.o has exists. The all4:abort.o has no recipe so no recipe is run

#note the following 3 rules from [all4,all6] will not remove implicit rule %.o:%.c since their target is\
 not the same as the implicit rule target %.o;\
 implicit rule %.o:%.c and these 3 rule co-exist, which can be verified by `make -p`
all4:abort.o
../%.o: %.c
	$(show_auto)

obj=..
#$ make -f vpath.mk all5\
[../abort.o] [..] [abort.o]<--both prerequisite and target has %, but their expanded % is not the same, the former will add dir found by vpath\
[/home/rednoah/opt/preparation/abort.c] [/home/rednoah/opt/preparation] [abort.c]
all5:$(obj)/abort.o
$(obj)/%.o:%.c#comment A:can be used to put all .o files into a separated dir like make O=xx in linux
	$(show_auto);rm -rf $@

obj=obj
#$ make -f vpath.mk all6<--similar to all5 except the object dir is changed from ../ to obj/\
[obj/abort.o] [obj] [abort.o]\
[/home/rednoah/opt/preparation/abort.c] [/home/rednoah/opt/preparation] [abort.c]
all6:$(obj)/abort.o
$(obj)/%.o:%.c#see comment A
	$(show_auto)
	@mkdir -p $(obj)
	@gcc -c $< -o $@
	@rm -rf $@
ifeq (1,0)
#this branch will make vpath search in $(obj)/%.o not take effect. reason unknown
abort.c: verify_target_bpf
	$(show_auto)
verify_target_bpf:
	$(show_auto)
else
#this branch will not be run when `make all6` and vpath will still take effect in the above '$(obj)/%.o:%.c' rule ;\
vpath will not append the following 'abort.c' in target.But make manual says:make uses VPATH as a search list for both\
prerequisite and target of rules.
abort.c:
	$(show_auto)
endif

#fail: make: *** No rule to make target 'pthread//arm_test.c', needed by 'all7'.  Stop.
all7:pthread//arm_test.c
	$(show_auto)

#normal
all8:pthread/arm_test.c
	$(show_auto)

.PHONY:all6