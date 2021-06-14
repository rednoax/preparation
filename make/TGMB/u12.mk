patchsearch=$(firstword $(wildcard $(addsuffix /$1,$(subst :, ,$(PATH)))))
$(info $(call patchsearch,ls))

o=$(call map,origin,o map MAKE)
map=$(foreach a,$2,$(call $1,$a))
$(info [$o])
#to prove that conditional directives is handled in phase 1, so auto variables(which is not set until the recipe for that rule\
 is invoked) like $@ can't be used in any phase 1 process.
all:
ifdef o
#the following can show $@ as expected.
	@echo "o defined:$@"
$(info "o defined:$@")#will be displayed before the last line's "phase 1 fin", auto variables\
 can't be displayed as they only exists in phase2 when recipes is handled by make.
else
	echo "o !defined"
$(info "o !defined")
endif
$(info "##phase 1 fin##")#then recipe began to be expanded and run


#make -f u12.mk all2
define func
if [ ! -z "$o" ];then \
	echo "o defined:$@";\
else \
	echo "o !defined:$@";\
fi
endef
all2:
#conditional derectives are parsed at once in phase 1.This means, for example, that automatic variables \
 can't be used in conditional directives, as automatic variables are not set until the recipe for that \
 rule is invoked. i.e. the auto variables exists only in phase 2.\
 If you need to use auto variables in a conditional directives u must move the condition into the recipe \
 and use shell conditional syntax instead.
	$(func)