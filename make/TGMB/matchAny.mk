MAKEFLAGS+=-rR

ifeq ($(MAKECMDGOALS),all)
#$ make -f matchAny.mk all\
:\
make: *** No rule to make target 'c', needed by 'all'.  Stop.
#$ make -f matchAny.mk all -d<--show 'Avoiding implicit rule recursion' for c<=c.c<=c.c.c
all:b c
%: %.c
	:
endif

ifeq ($(MAKECMDGOALS),all1)
#$ make -f matchAny.mk all1 -d\
 ...\
 Looking for a rule with intermediate file 'foo.c.c'.\
    Avoiding implicit rule recursion.\
 ...
#$ make -f matchAny.mk all1\
make: *** No rule to make target 'foo.c', needed by 'all1'.  Stop.
all1:foo.c
%: %.c
	:
endif

ifeq ($(MAKECMDGOALS),all2)
#$ make -f matchAny.mk all2\
:\
make: *** No rule to make target 'c', needed by 'all2'.  Stop.<--result exactly the same as `make all`
all2:b c
%::%.c
	:
endif

ifeq ($(MAKECMDGOALS),all3)
#there is a.c in current dir,so a<=a.o<=a.c
#$ make -f matchAny.mk all3\
cp a.c a.o\
:\
rm a.o<--see comment A
all3:a
%: %.o#comment A:so the %.o can be intermediate, which is rm-ed once a is generated
	:
%.o: %.c
	cp $< $@
endif

ifeq ($(MAKECMDGOALS),all4)
#a.c exists in the current dir but a.o not
#$ make -f matchAny.mk all4\
make: *** No rule to make target 'a', needed by 'all4'.  Stop.\
$ touch a.o\
$ make -f matchAny.mk all4\
:\
$ make -f matchAny.mk all4<--a not exist so its recipe AL runs\
:\
$ rm -rf a.o\
$ make -f matchAny.mk all4\
make: *** No rule to make target 'a', needed by 'all4'.  Stop.
all4:a
%::%.o#comment B:double colons marks the prerequisite as non imtermidiate;this rule only applies when the \
 prerequisite actually exists.No chains considered on double colon's prerequisite
	:
%.o: %.c
	cp $< $@
endif

ifeq ($(MAKECMDGOALS),all5)
#when there is no FORCE in current dir:\
$ make -f matchAny.mk all5\
make: *** No rule to make target 'a', needed by 'all5'.  Stop.
all5:a
%::FORCE
	:
endif
ifeq ($(MAKECMDGOALS),all6)
#$ make -f matchAny.mk all6\
:\
:\
:
#to see more info:\
$ make -f matchAny.mk all6 -d\
...\
 Must remake target 'matchAny.mk'.\
:\
 Successfully remade target file 'matchAny.mk'.\
...\
  Must remake target 'a'.\
:\
  Successfully remade target file 'a'.\
 Finished prerequisites of target file 'all6'.\
Must remake target 'all6'.\
:
all6:a
%::FORCE
	:
.PHONY:FORCE
endif

ifeq ($(MAKECMDGOALS),all7)
#$ make -f matchAny.mk all7\
:
all7:a
%::FORCE
	:
.PHONY:FORCE
#prevent updating this makefile self and all7
%.mk:;
all%:;
endif

ifeq ($(MAKECMDGOALS),all8)
# make -f matchAny.mk all8\
make: *** No rule to make target 'foo', needed by 'all8'.  Stop.<-no foo.c
all8:foo
%: %.c
	:
endif

ifeq ($(MAKECMDGOALS),all9)
#$ make -f matchAny.mk all9<--note there is no matchAny.mk\
foo.c\
foo\
all9.c\
all9
all9:foo
%:%.c;@echo $@
%.c:;@echo $@
endif

ifeq ($(MAKECMDGOALS),all10)
#$ make -f matchAny.mk all10<--compare with all8\
foo\
all10
all10:foo
%:%.c
	@echo $@
%::;@echo $@
endif