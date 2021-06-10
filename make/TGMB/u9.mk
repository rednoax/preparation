#make will keep all # in define block intact in variable's defination. When the variable is expanded, they will \
 either be treated as make comments or as recipe text, depending on the context where the variable is evaluated.
 
#one line var is like a single bash shell script and it will be run by a sub-shell, so the following 2 $$ is the same
define one-line
echo -e "#\n$$$$: $1"; \
date;\
echo -e "$$$$: $1\n#"
endef

#each line of multi line var is a single bash shell script that will be run by a sub-shell, so each line is run in \
 different sub-shells, so the following 2 $$ is not the same.
 In recipe execution, each line of a canned sequence is treated just as if the line appeared on its own in the rule,\
 preceded by a tab. In paticular, make invokes a separate sub-shell for each line.
define multi-line
echo -e "#\n$$$$: $1"
date
echo -e "$$$$: $1\n#"
endef

.PHONY:all
all:
	@$(call one-line, 1)
	@$(call multi-line, 2)