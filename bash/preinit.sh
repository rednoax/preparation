#!/bin/bash
boot_hook_init() {
	local hook=${1}_hook
	echo ==$$==
	echo PI_STACK_LIST=[${PI_STACK_LIST}]
	export -n "PI_STACK_LIST=${PI_STACK_LIST:+$PI_STACK_LIST }$hook"
#do'not use the following as there AL a space precedes PI_STACK_LIST's value
#	export -n "PI_STACK_LIST=${PI_STACK_LIST:+$PI_STACK_LIST} $hook"
	export -n "$hook="
	echo PI_STACK_LIST=[${PI_STACK_LIST}]
	echo ["$hook="]
}

boot_hook_add() {
	local hook="${1}_hook${PI_HOOK_SPLICE:+_splice}"
	local func="${2}"
	echo -e "local hook=$hook"
	[ -n "$func" ] && {
		local v; eval "v=\$$hook"
		export -n "$hook=${v:+$v }$func"
	}
	local v;eval "v=\$$hook"
	echo $hook="$v"
}

boot_hook_shift() {
	local hook="${1}_hook"
	local rvar="${2}"
	echo shfit hook=$hook rvar=$rvar
	local v; eval "v=\$$hook"
	echo $hook=$v-
	[ -n "$v" ] && {
		local first="${v%% *}"
		echo first=$first,${v#* }=
		[ "$v" != "${v#* }" ] && \
			export -n "$hook=${v#* }" || \
			export -n "$hook="
		echo "=>$hook=${v#* }<="
		export -n "$rvar=$first"
		return 0
	}

	return 1
}

boot_run_hook() {
	local hook="$1"
	local func

	while boot_hook_shift "$hook" func; do
		local ran; eval "ran=\$PI_RAN_$func"
		echo ran=$ran
		if [ -n "$ran" ];then
			echo [[$ran]]
		else
			export -n "PI_RAN_$func=1"
			echo "=>[$func $1 $2]"
#			$func "$1" "$2"
		fi
	done
}

boot_hook_init preinit_essential
boot_hook_init preinit_main
boot_hook_init failsafe
boot_hook_init initramfs
boot_hook_init preinit_mount_root

boot_hook_add preinit_main preinit_ip
boot_hook_add preinit_main pi_indicate_preinit


boot_run_hook preinit_essential

pi_mount_skip_next=false
pi_jffs2_mount_success=false
pi_failsafe_net_message=false

boot_run_hook preinit_main

test_append(){
	local b=2
	b2=2
}
#both are not defined
file=./dump_env.out
echo "$b,$b2"
test_append
#the local b in test_append cannot be seen;b2 can be seen after defined in test_append
echo "$b,$b2"

myenv=1
$file myenv
$file PI_STACK_LIST

export myenv=1
$file myenv
#if one exported var is "export -n" then it is only exist in current pid, not in child process
export -n myenv
$file myenv

export myenv=1
$file myenv
export -n myenv=
$file myenv

export myenv=1
$file myenv
export -n myenv=x
$file myenv

echo $PI_STACK_LIST,$myenv

__debug() {
	if [ -e /dev/kmsg ];then
		sudo echo $1 >>/dev/kmsg
	else
		sudo echo $1
	fi
}

__debug "PREINIT=$PREINIT"
