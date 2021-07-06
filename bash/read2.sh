count=0
cat input.txt|while read -u 0 var
do
	((count++))
	echo "Line $count:$var"
done
#fin: Line# is 0, pipe's right is a process launched by |, which exits after it finished(while finished),so its count disappeared after\
it exits.The following count is the 1st line's count.
echo -e "fin: Line# is $count"
