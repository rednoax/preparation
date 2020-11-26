var0=
echo [${var0:+-1}]
echo [${var1:+-2}]
var1=1
echo ${var1:+-3}
set -x
#empty is the same effect as undefined
#NO_EXPORT=1 ./uci.sh; NO_EXPORT ./uci.sh will output different result
export ${NO_EXPORT:+-n} CONFIG_NUM_SECTIONS=0
CONFIG_SECTION=
CONFIG_func() {
	a=$CONFIG_NUM_SECTIONS
}
export -f CONFIG_func
export |grep CONFIG
#(export |grep CONFIG)

for VAR in $CONFIG_LIST_STATE;do
	echo $VAR
done
package() {
	echo $1
}
eval package system
func()
{
	func_var=a
}
echo $func_var
func
echo $func_var

[ -n $old ]
old=
[ -n $old ]

for _dev in /sys/class/ieee80211/*;do
	[ -e $_dev ] || continue
done
:
