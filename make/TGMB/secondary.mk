.SECONDEXPANSION:
var=dummy
#no secondary $(info ..) displayed in prerequisite expansion
all:$(var)$(info 0 $@ $^);$(info 1 $@ $^)
dummy:$(info 2 $@ $^);$(info 3 $@ $^)
$(info phase 1 fin)