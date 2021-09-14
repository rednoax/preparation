
echo $$ $PPID
set -e
sleep 10
echo -n "$0,"
for i in "$@";do
	echo -n "$i,"
done
echo 
ls >/dev/null
#1.launch bcc execsnoop in one terminal. And run the following builtin cmds in another terminal. execsnoop will not emit anything.
#pwd;:;echo

#2.$ bash ls
#/usr/bin/ls: /usr/bin/ls: cannot execute binary file

#The reson is 'ls' is recognized as a shell script:
#https://unix.stackexchange.com/questions/342309/usr-bin-ls-usr-bin-ls-cannot-execute-binary-file 
#ARGUMENTS If arguments remain after option processing, and neither the -c nor the -s option has been 
#supplied, the first argument is assumed to be the name of a file containing shell commands.

#$  strace -f -o bash_ls.log bash ls <--root need not!!
#execve("/usr/bin/bash", ["bash", "ls"], 0x7fff3d81d2c8 /* 22 vars */) = 0 <=NOTE arg0 is "bash",arg1 is "ls"

#3.-c is to read command from string and assign any arguments to the positional parameters
#Let's see what's the difference between with ' and w/t '
#$ bash -c ls b c
#[mass output of a normal `ls`, ignored]
# grep exec of `strace -f -o bash_ls.log bash -c ls b c`
#18649 execve("/usr/bin/bash", ["bash", "-c", "ls", "b", "c"], 0x7fff57063068 /* 22 vars */) = 0 <--args after -c is 3 args
#18649 execve("/usr/bin/ls", ["ls"], 0x556d77b68c30 /* 22 vars */) = 0 <--2 args of 'b' 'c' NOT transfered at all!!
#
#rednoah@lucia:~/preparation/bash$ bash -c ./echo.sh 1 2 3
#./echo.sh,<--1 2 3 is not recognised as argument

#$ bash -c 'ls b c'
#ls: cannot access 'b': No such file or directory 
#ls: cannot access 'c': No such file or directory 
# grep exec of `strace -f -o bash_ls.log bash -c 'ls b c'` 
#18656 execve("/usr/bin/bash", ["bash", "-c", "ls b c"], 0x7ffdc9bd53c8 /* 22 vars */) = 0<--args after -c is ONLY one whole arg
#18656 execve("/usr/bin/ls", ["ls", "b", "c"], 0x55972214dc30 /* 22 vars */) = 0<--2 args of 'b' 'c' normally transfered!
#
#rednoah@lucia:~/preparation/bash$ bash -c "./echo.sh 1 2 3"
#./echo.sh,1,2,3,
#
#`bash -c 'cmd1;cmd2'` if different from `bash -c cmd1;cmd2` as the latter is `bash -c cmd1` amd `cmd2`
#
#4.rednoah@lucia:~/preparation/bash$ bash -c echo.sh<--bash -c and source filename all uses PATH
#bash: echo.sh: command not found

#5.-e is to exit if one cmd fails
#rednoah@deb11:~/opt/preparation/bash$ bash -c 'set -e;a;b'
#bash: line 1: a: command not found<--b is not run
#rednoah@deb11:~/opt/preparation/bash$ bash -c 'a;b'
#bash: line 1: a: command not found
#bash: line 1: b: command not found<--b is still run
# another example:
#rednoah@deb11:~$ ls a
#ls: cannot access 'a': No such file or directory
#rednoah@deb11:~$ set -e<--then any command return non zero will exit current shell
#rednoah@deb11:~$ ls a<--or try to change it to `false`
#ls: cannot access 'a': No such file or directory

#rednoah@lucia:~/preparation/bash$ bash -e echo.sh 1 2 3<--note $PATH has been double checked and there is NO cur directory in it!
#strace:                                   /|\ must be a script.It can ignore ./ as bash will search from current even PATH has no such
#execve("/usr/bin/bash", ["bash", "-e", "echo.sh", "1", "2", "3"], 0x7ffc60156238 /* 22 vars */) = 0
#openat(AT_FDCWD, "echo.sh", O_RDONLY)   = 3
#output:
#./echo.sh,1,2,3,<-- w/t -c is to run shell script directly
