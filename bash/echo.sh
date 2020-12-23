echo [$0,$1,$2]

#-c is to read command from string and assign any arguments to the positional parameters
#-e is to run shell script

#rednoah@lucia:~/preparation/bash$ bash -c echo.sh<--bash -c and source filename all uses PATH
#bash: echo.sh: command not found

#rednoah@lucia:~/preparation/bash$ bash -c "./echo.sh 1 2 3"
#[./echo.sh,1,2]
#rednoah@lucia:~/preparation/bash$ bash -c ./echo.sh 1 2 3
#[./echo.sh,,]<--1 2 3 is not recognised as argument
#rednoah@lucia:~/preparation/bash$ bash -e echo.sh 1 2 3, note $PATH has been double checked and there is NO cur directory in it!
#[echo.sh,1,2]<-- -e is to run shell script directly
#rednoah@lucia:~/preparation/bash$ bash -e ./echo.sh 1 2 3
#[./echo.sh,1,2]<-- -e is to run shell script directly
