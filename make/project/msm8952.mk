iter=$(foreach v, $1, $(warning $v='$($v)'))
VARS:=DEBUG BOOTLOADER_OUT __MAKECMDGOALS project-name PRJECT pattern vars
$(call iter, $(VARS))
.PHONY:all
all:
	echo current dir is $(shell pwd)
$(warning msm8952.mk:$(shell echo $$PPID))