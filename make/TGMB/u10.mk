rev0:=$2 $1#fail as := is expanded immediately
rev1=$2 $1
$(info [$(call rev0,a,b)] [${call rev1,a,b}])
foo=$(call rev0,a,b)
bar=${call rev1,a,b}
$(info [${foo}] [${bar}])
dump=[$1][$2][$3]
#each preceding/trailing \t or space in each argument is preserved as the parameter value!
$(info $(call dump,	1 ,2 , 3))#the 1st arg has a preceding \t and a trailing space, all of them is preserved as the value of \
 the 1st argument.
$(info $(call dump, 1	, 2,3 ))#the 1st arg has a preceding space and a trailing \t,all of them is preserved as the value of \
 the 1st argument.
all: