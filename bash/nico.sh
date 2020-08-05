tmp=__tmpfile
files=$*
declare -i count r1
for file in $files
do
	youtube-dl -F $file
	r1=1
	count=1
	while (( $r1 != 0 ))
	do
		echo "###count $count"
		r0=`youtube-dl -f best $file`
		echo "$r0" | tee $tmp
		echo -e "###\t$count fin\n"
		grep 100% $tmp
		r1=$?
		echo "------result $r1------"
		(( count+=1 ))
	done
done
