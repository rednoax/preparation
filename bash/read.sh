#the following can't be [3]<input.txt:
#./read.sh: line 1: [3]: command not found
#./read.sh: line 3: read: 3: invalid file descriptor: Bad file descriptor

exec 3<input.txt
count=0
while read -u 3 var
do
	((count++))
	echo "Line $count:$var"
done
echo -e "fin: Line# is $count"
exec 3>&-
