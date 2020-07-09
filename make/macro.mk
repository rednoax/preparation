define macro
$2
$1 $3 $($4) $$5 ${$6}
endef

iterator=$(foreach v,$1,$(warning $v=$($v)))

var=$(call macro,0,1)
#the above will show:
#macro.mk:9: var=1
#0   $5

$(call iterator,var)

all:
