FOO=file
all0 all1:
	@echo $$FOO
all0:export FOO=all0
all1:FOO=all1

all2 all3:
	@echo $(FOO)
all3:FOO=all3

#make all0 all1 all2 all3
#all0
#<--empty, exported and cmd arg will all becomes envp of shell process!unexported var in file will never be seen in shell process
#file
#all3<--as all3 has override FOO as all3

#make all0 all1 all2 all3 FOO=arg
#arg<---var from cmd line can be seen in shell process's env by the 3rd argument of int main(,,char **envp)
#arg
#arg
#arg