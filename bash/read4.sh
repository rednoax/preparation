count=0
while read -u 0 var
do
((count++))
echo line $count:$var
done<input2.txt
echo "fin: line# is $count"

count=0
while read -u 0 -r var
do
((count++))
echo line $count:$var
done<input2.txt
echo "fin: line# is $count"
