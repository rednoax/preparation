#CFLAGS or CPPFLAGS is used by implicit rule but their default value is empty string \
$ make -f u10.3.mk \
CC=cc default \
CFLAGS= undefined \
make: Nothing to be done for 'all'

#built-in implicit rules uses some predefined variable, which can be overriden by command line or environment\
$ CC=gcc make -f u10.3.mk CFLAGS='-g -Wall' \
CC=gcc environment \
CFLAGS=-g -Wall command line \
make: Nothing to be done for 'all'. 

$(info CC=$(CC) $(origin CC))
$(info CFLAGS=$(CFLAGS) $(origin CFLAGS))
all: