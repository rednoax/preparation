#if no recipe for all:$ make -f ifeq.mk \
make: Nothing to be done for 'all'.

all:
ifeq ($(MAKELEVEL),0)
	$(MAKE) -f ifeq.mk all
endif
ifeq ($(MAKELEVEL),1)
	:$(info $(MAKEFLAGS))
endif

#$ make -f ifeq.mk all2 \
haha
#there is no all2 so make try generating prerequisite with 'echo haha'. At last there is no prerequisite still\
so the next time to run all2, there is still 'haha' echo-ed
all2:prerequisite;
prerequisite:;@echo haha

#each time after running `make all3`:make: 'all3' is up to date
all3:;
#each time after running `make all4`:make: make: Nothing to be done for 'all4'
all4:
#each time after running `make all5`: :
all5:
	:
#`make all6` is the equivalent to `make all5`
all6:;:

#`make all7`:Nothing to be done for 'all7'
all7:all3