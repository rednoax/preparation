empty=
space=$(empty) #
tab=$(empty)	#
v=$(empty)$(tab)$(space)$(tab)$(tab)
#if a variable consisting of only whitespace, $(strip ...) will make it empty
$(info [$v] =strip=> [$(strip $v)])
all:
ifeq ($(strip $v)$(info handled in phase 1),)#ifeq/ifdef's line is AL expanded in phase 1
	@echo $@$(info handled in phase 2)#all single $ like $(xx) are all expanded before transfering to shell
endif
ifeq ($(strip $v),  	  	)#several space and \t after ',' do't matter
	@echo $@ 2
endif
ifeq ($(strip $v ),)#SPACE after $v is removed by $(strip
	@echo "can still arrive"
endif
ifeq ($(strip $v ) 		 ,)#whitespace here is multiple space & \t, which consists of the left part's value
	@echo "can still arrive 2"
endif
ifeq (abc,abc )#space matters
	@echo "can't 1"
endif
ifeq (abc, 		abc)#whitespace here is multiple space & \t, which do't matter
	@echo "can 1"
endif
ifeq ( abc,abc)#space matters
	@echo "can't 2"
endif
ifeq (abc ,abc)#space not matter
	@echo "can 2"
endif
#so white space around ',' not matter but around '(' or '') matters
$(info phase 1 fin)