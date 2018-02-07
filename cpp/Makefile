FLAGS:=--std=c++11 -Wall
.PHONY:all
all:
	g++ $(FLAGS) list.cpp -o list.out -lpthread
	g++ $(FLAGS) main.cpp -o main.out
	g++ $(FLAGS) util.cpp -o util.out
	g++ $(FLAGS) poll.c -o poll.out
	g++ $(FLAGS) cli.cpp -o cli.out
