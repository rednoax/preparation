#!/bin/bash
func() {
	[ -n "$ran" ] || {
		echo f
	}
#echo
#echo
	. sourced.sh
	echo "in func sourced=$sourced"
}

[ -n "$1" ] && {
	echo 1
}
[ -n "$1" ] || {
	echo 2
}
echo ==$1
func
echo =next
ran=
func
echo ==next
ran=0
func

func2() {
	echo $1,$2,$3,$4,$5
	echo $*
	ifname=eth0.1
	local ifname1=eth0.2
}

func2 "PREINIT=$PREINIT" "$ran SOURCED" $ran sourced
echo "$0,$1,$*,$@, sourced"

#set_state() { :; . sourced.sh 2 }
echo "out of func:sourced=$sourced"
echo $ifname [$ifname1]