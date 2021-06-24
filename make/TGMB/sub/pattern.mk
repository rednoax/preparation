.PHONY:all
all:.out;

#comment A:if disable the following ifeq block\
$ make -f pattern.mk\
make: *** No rule to make target '.out', needed by 'all'.  Stop.
#so % can't match zero char, % means >=1 chars is a must;\
 if `mv .txt a.txt` then all:a.txt, and keeps the following ifeq disabled: this makefile works for `make all`
%.out:%.txt
	cp $< $@

ifeq (1,1)#if disable the following, see comment A
#$ make -f pattern.mk\
make: Circular pattern.mk.out <- pattern.mk dependency dropped.\
make: Circular .txt.out <- .txt dependency dropped.\
cp .txt .out
.out:.txt
	cp $< $@
endif
clean:
	rm -rf .out *.out