#$(shell ls)#sub.mk:1: *** missing separator.  Stop.

#sub.mk#sub.mk:2: *** missing separator.  Stop.

# the above are 2 wrong debug way, use info to show the output and function info will return nothing so that it has no impact to Makefile

$(warning "")#warning has file name and line number before "" but $(info '') wil just show '' w/t filename and line number
$(info '')#'' will be shown

$(info $(shell echo $$PPID $$$$) $(shell pwd) $(CURDIR) $(PWD) $(MAKELEVEL))#pwd result is the same as CURDIR

$(shell sleep 100)

all:;
