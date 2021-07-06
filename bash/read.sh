exec 3<input.txt
count=0
while read -u 3 var
do
	((count++))
	echo "Line $count:$var"
done
echo -e "fin: Line# is $count"
exec 3>&-
