.PHONY:all
all:.out;

#comment A:if disable the following ifeq block\
$ make -f pattern.mk\
make: *** No rule to make target '.out', needed by 'all'.  Stop.
#so % can't match zero char, % means >=1 chars is a must;\
 if `mv .txt a.txt` then all:a.txt, and keeps the following ifeq disabled: this makefile works for `make all`
ifeq (1,1)
%.out:%.txt
	cp $< $@
endif
ifeq (1,1)#if disable the following, see comment A
#$ make -f pattern.mk\
make: Circular pattern.mk.out <- pattern.mk dependency dropped.\
make: Circular .txt.out <- .txt dependency dropped.\
cp .txt .out
.out: .txt
	cp $< $@
else
endif
clean:
	rm -rf .out *.out

var=a=b c=d a b c=d #[a=b c=d a b c=d ]
$(info [${var}])
hash != find . -name "*.mk"
$(info $(.SHELLSTATUS))#0, find AL return 0 no matter if the specified pattern can be found or not
hash != find . -name "*.C"
$(info $(.SHELLSTATUS))#0, the reason see ditto
hash != find . -name *.mk
$(info $(.SHELLSTATUS))#1, as *.mk is expanded to a list of .mk found before giving *.mk to find: find: paths must precede expression: `nqt.mk'
