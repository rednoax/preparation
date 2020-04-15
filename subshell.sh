(ls unexist)
echo $?
(ls *.c)
echo $?
echo "----"
vars=`ls *.c *.t`
echo "===$vars=="

#cmd1 && cmd2: cmd2 is run iff cmd1 return 0

#cmd1 || cmd2: cmd2 is run iff cmd1 return 1

#cd, echo are builtin command in man bash
#In general, an external command in a script forks off a subprocess, [107] whereas a Bash builtin does not. For
#this reason, builtins execute more quickly and use fewer system resources than their external command
#equivalents.

echo "\$\$=$$" #2938, bash 2558 will fork a new process 2938 to run this script
echo \$BASHPID=$BASHPID #ditto
(echo \(\$BASHPID=$BASHPID\)) #2942, 2938 will for a sub shell, whose pid is 2942
echo "\$PPID=$PPID ie bash's pid" #2558 that's the bash(ps can show "2558 pts/0    00:00:00 bash") that launch the 2938 pid to run this script
sleep 1

echo -ne \$BASHPID=$BASHPID. && ./show #$BASHPID=2938.pid 2944, ppid 2938
#+current script bash pid is 2938, which is launched by the bash
#+ 2938 will fork+exec ./show, so ./show's pid >2938
(echo -ne \$BASHPID=$BASHPID. && ./show) #$BASHPID=2945.pid 2946, ppid 2945
#+current script bash pid is 2938, which is launched by the bash 2558, 2938 fork a new process 2945>2938
#+the subshell process 2945 fork+exec ./show whose pid 2946>2945

./show   #pid 2947, ppid 2938
#+ current script bash pid is 2938
(./show) #pid 2948, ppid 2938
#+ current script bash pid is 2938, it will launch a subshell 2948(note it is one more than last fork+exec process ./show), which will exec ./show directly
#+ there is no fork+exec for the sub shell again, like (echo -ne \$BASHPID=$BASHPID. && ./show)
#+ Actually, we should not care so many details about it, we just need to know some chracters of subshell, e.g.
#+ one "cd" in subshell will not effect its parent.
