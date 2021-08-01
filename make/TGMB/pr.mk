$(info $(MAKECMDGOALS))#empty if `make`, all of `make all`
.PHONY:all
all:../../string.o
#$ make -f pr.mk all\
[../../string.o,../..,string.o]\
[../../string.c,../..,string.c]
ifeq ($(MAKECMDGOALS),all)
%.o: %.c#will completely remove old implicit %.o: %.c rules
	@echo [$@,$(@D),$(@F)]
	@echo [$<,$(<D),$(<F)]
endif

#$ make -f pr.mk\
\
cc    -c -o ../../string.o ../../string.c\
$ make -f pr.mk\
\
make: Nothing to be done for 'all'.
