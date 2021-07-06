count=0
while read -u 0 var
do
	((count++))
	echo "Line $count:$var"
done<input.txt
#fin: Line# is 0
echo -e "fin: Line# is $count"
