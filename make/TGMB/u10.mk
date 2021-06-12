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
 
$(info (${findstring 	1,1 2}))#there is a space and a \t before the 1st arg of findstring, they're all ignored so the real 1st\
 argument is just one character '1'
$(info ($(findstring 1 ,2 1)))#print () as the trailing space of 1st arg is preserved as value of $1
$(info ($(findstring 1	,1	2)))#print (1\t),the 1st arg has a trailing \t,the 2nd arg is "1\t2" so findstring recognize it is matched
all: