FLAGS:=--std=c++11 -Wall
CSRCS:=list.cpp main.cpp util.cpp poll.c cli.cpp socketinfo.cpp derived.cpp fd_trans.cpp \
		sp.cpp unique.cpp unique2.cpp auto_des.cpp
OUTS:=$(patsubst %.cpp, %.out, $(CSRCS))
.PHONY:all
all:$(OUTS)
%.out:%.cpp
	g++ $(FLAGS) $< -o $@ -lpthread
unique2.out:FLAGS=--std=c++14 -Wall