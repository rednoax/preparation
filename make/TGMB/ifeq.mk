#if no recipe for all:$ make -f ifeq.mk \
make: Nothing to be done for 'all'.

all:
ifeq ($(MAKELEVEL),0)
	$(MAKE) -f ifeq.mk all
endif
ifeq ($(MAKELEVEL),1)
	:$(info $(MAKEFLAGS))
endif