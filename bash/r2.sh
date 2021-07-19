timer=5
{
#todo:kill the bash pid in ps will kill sleep as well, why?
#   PID TTY          TIME CMD
# 76934 pts/3    00:00:00 bash
# 77798 pts/3    00:00:00 bash<--kill it will make 77882 exits too! why?
# 77882 pts/3    00:00:00 sleep
# 77883 pts/3    00:00:00 ps

	while [ $timer -le 8 ];do
		echo -e "\ta $timer"
		timer=$(($timer+1))
		sleep 1
	done
}&

{
	while [ $timer -gt 0 ];do
		echo "b $timer"
		sleep 1
		timer=$(($timer-1))
	done
}
sleep 5
timer=$(($timer-1))
echo "final $timer"