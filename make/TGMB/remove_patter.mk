ifeq (1,1)
MAKEFLAGS+=-rR#work for this L0 makefile at once. In `make -p` output:\
1. no ANY implicit rules(pattern or suffix rule) displayed,`make -p`'s total line# is only <200 lines\
2. SUFFIX:=<empty> and no ".SUFFIXES: .out .a .ln .o .c .cc .C .cpp..."<-now\
SUFFIXES := .out .a .ln .o .c .cc .C .cpp .p .f .F .m .r .y .l .ym .yl .s .S... <--before -rR added\
.SUFFIXES: .out .a .ln .o .c .cc .C .cpp .p .f .F .m .r .y .l .ym .yl .s .S... <--before -rR added
all:
%.o:%.c
	@touch $@

%.o:%.c#if no the rules after this one, this rule will make the above rule disappear in `make -p`
	@echo haha

ifeq (1,1)
#%.o: %.c\
#  recipe to execute (from 'remove_patter.mk', line 14):\
        :
%.o:%.c#this rule will make all the above 2 disappear in `make -p`
	:
endif

file.o:file.c
	:
#for non pattern rule, redefine is not suggested
#remove_patter.mk:24: warning: overriding recipe for target 'file.o'\
remove_patter.mk:22: warning: ignoring old recipe for target 'file.o'
file.o:file.c
	echo
else
#a rule with the same target & prerequisite will override the default implicit rule
all:
%.o: %.c#this line definitely remove implicit %.o:%.c,`make -p` proves it
## Implicit Rules\
%.o: %.c \
#  recipe to execute (from 'remove_patter.mk', line 32):\
        :
	:
endif