#TO TEST:
#target specific variable
#target specific exported var

FOO=file
all0 all1:
	@echo $$FOO
#the above will be transfered to shell as "echo $FOO"

all0:export FOO=all0
all1:FOO=all1#

all2 all3 all4:
	@echo $(FOO)
#the above $(FOO) will be replaced by make then transfered to shell

all3:FOO=all3
all4:override FOO=all4

#$ make -f u3.mk all0 all1 all2 all3 all4
#all0
#<--empty, exported and cmd arg will all becomes envp of shell process!unexported var in file will never be seen in shell process
#file
#all3<--as all3 has override FOO as all3
#all4<--file var with override takes precedence to command line var

#$ make -f u3.mk all0 all1 all2 all3 all4 FOO=arg
#arg<---var from cmd line can be seen in shell process's env by the 3rd argument of int main(,,char **envp)
#arg
#arg
#arg
#all4<--file var with override takes precedence to command line var

#$ FOO=1 make -f u3.mk all1
#all1<--FOO is env inherited by shell process that runs "echo $FOO"