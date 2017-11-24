iter=$(foreach v, $1, $(warning $v='$($v)'))
VARS:=DEBUG BOOTLOADER_OUT
$(call iter, $(VARS))
.PHONY:all
all:
	echo current dir is $(shell pwd)
