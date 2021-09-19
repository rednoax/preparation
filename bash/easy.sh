#!/home/rednoah/opt/preparation/make/TGMB/dump.out -o arg arg
#$ ./easy.sh 1 2 3 4
#0: /home/rednoah/opt/preparation/make/TGMB/dump.out
#1: -o arg arg
#2: ./easy.sh
#3: 1
#4: 2
#5: 3
#6: 4

#!/bin/bash

#!/bin/dash
#can't run ((count++))

#!/bin/sha

echo "$$ $PPID";
if [ ! -z $1 ];then
: #sleep $1
fi

count=0
for i in "$@";do
	echo "$count:$i"
	((count++))
done
count=0
for i in "$*";do
	echo "$count:$i"
	((count++))
done
