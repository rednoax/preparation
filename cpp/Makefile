FLAGS:=--std=c++11 -Wall
#CSRCS:=list.cpp main.cpp util.cpp poll.c cli.cpp socketinfo.cpp derived.cpp fd_trans.cpp \
		sp.cpp unique.cpp unique2.cpp auto_des.cpp
CSRCS:=$(wildcard *.cpp)
OUTS:=$(patsubst %.cpp, %.out, $(CSRCS))

#export CROSS_COMPILE=~/OP-TEE/toolchains/aarch32/bin/arm-linux-gnueabihf-
#to objdump, "export PATH=~/OP-TEE/toolchains/aarch32/bin/:$PATH" in bash
GPP=$(CROSS_COMPILE)g++
OBJDUMP=$(CROSS_COMPILE)objdump
.PHONY:all
all:$(OUTS)
%.out:%.cpp
	$(GPP) $(FLAGS) -S $<
	$(GPP) $(FLAGS) $< -o $@ -lpthread
	$(OBJDUMP) -D $@ >$@.s
auto_des.out unique2.out:FLAGS=--std=c++14 -Wall
