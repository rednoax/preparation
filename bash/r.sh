test()
{
	if [ -r $1 ]; then
		echo $1 can be read
	else
		echo $1 can NOT read
	fi
}

test /dev/console
test noSuch
test r.sh
func(){
	local timer=10
	{
		while [ $timer -gt 8 ];do
			timer=$(($timer-1))
			echo -e "\ta: $timer"
			sleep 1
		done
	}
	echo "middle $timer"
	{
		while [ $timer -gt 3 ];do
			echo "b $timer"
			sleep 1
			timer=$(($timer-1))
		done
	}&
	sleep 10
	echo "final $timer"
}

func