preinit_net_echo() {
	[ -n "$pi_ifname" ] && {
		{
			[ "$pi_preinit_net_messages" = "y" ] || {
				[ "$pi_failsafe_net_message" = "true" ] &&
					[ "$pi_preinit_no_failsafe_netmsg" != "y" ]
			}
		} && echo "$1"
	}
}

pi_ifname=1
pi_preinit_net_messages=
pi_failsafe_net_message=true
pi_preinit_no_failsafe_netmsg=
preinit_net_echo $0

set_state() { :; }
set_state preinit

do_sysinfo_generic() {
	echo "1"
	[ -d /proc ] || return
	echo "2"
	mkdir -p /tmp/sysinfo
	[ -e /tmp/sysinfo/board_name ] || \
		echo "$(strings /proc/device-tree/compatible | head -1)" > /tmp/sysinfo/board_name
	[ -e /tmp/sysinfo/model ] || \
		echo "$(cat /proc/device-tree/model)" > /tmp/sysinfo/model
}

do_sysinfo_generic

echo "=function in function="
reset_cb() {
#function in function is just defination, whose body willnot run unless this function is called
	config_cb() { echo config_cb;return 0; }
	option_cb() { echo option_cb;return 0; }
	list_cb() { echo list_cb;return 0; }
	echo reset_cb
	list_cb
}
reset_cb
#config_cb can be called after its nested defination
config_cb
debug () {
	echo "==[$@] [$DEBUG] [${DEBUG:-:}]="
#: is nop and the arguments passed to it by $@ are actually useless
	${DEBUG:-:} "$@"
}
#$ DEBUG=echo ./test2.sh 1 2
#$./test2.sh 1 2
debug
debug $@
