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

time?=$(shell date +%N)#as `date;date` show 2 the same values
#the above is exactly equivalent to:\
ifeq ($(origin time), undefined)\
time = $(shell date)<=NOTE the assignment is not :=\
endif

$(info $(time) $(time))#different values

all:
