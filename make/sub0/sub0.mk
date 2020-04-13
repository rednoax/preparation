$(warning V=[$(V)] MAKE1=$(MAKE1) MAKE2=$(MAKE2))
#even the upper layer explicitely export iter, the following will not work
$(call iter, V)
.PHONY:all
all:
