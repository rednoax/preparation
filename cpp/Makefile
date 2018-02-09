FLAGS:=--std=c++11 -Wall
CSRCS:=list.cpp main.cpp util.cpp poll.c cli.cpp socketinfo.cpp derived.cpp
OUTS:=$(patsubst %.cpp, %.out, $(CSRCS))
.PHONY:all
all:$(OUTS)
%.out:%.cpp
	g++ $(FLAGS) $< -o $@ -lpthread