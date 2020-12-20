# youtube-dl --proxy socks5://192.168.12.254:1081/ -f best https://www.nicovideo.jp/watch/sm37978391
EXE="youtube-dl --proxy socks5://192.168.12.254:1081/"

tmp=__tmpfile
files=$*
declare -i count r1
for file in $files
do
	$EXE -F $file
	r1=1
	count=1
	while (( $r1 != 0 ))
	do
		echo "###count $count"
		r0=`$EXE -f best $file`
		echo "$r0" | tee $tmp
		echo -e "###\t$count fin\n"
		grep 100% $tmp
		r1=$?
		echo "------result $r1------"
		(( count+=1 ))
	done
done
