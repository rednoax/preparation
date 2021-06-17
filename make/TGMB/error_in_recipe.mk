#all funcX use a single sub-shell to run the one line shell "script". \
 so the followings all just test the failure in one of these cmds in a \
 single sub-shell.
 
#all cmds will be run no matter if there is any error 
define func0
	echo 1;\
	(echo 2;ls unexist;ls unexist2);\
	echo 3
endef

#$- will be inherited by the sub shell in (...). There is no further cmds once one cmd fails
define func1
	set -e;echo "1:$$-";\
	(echo "2:$$-";ls unexist;ls unexist2);\
	echo 3
endef

#$- in child process (...) will not be inherited by its parent.If there is any cmds failure, it just affects \
 the child process.
define func2
	echo "1:$$-";\
	(set -e;echo "2:$$-";ls unexist;ls unexist2);\
	echo "3:$$-"
endef

#1 \
2 \
ls: cannot access 'unexist': No such file or directory \
ls: cannot access 'unexist2': No such file or directory \
3
all0:
	@$(func0)

#1:ehBc \
2:ehBc \
ls: cannot access 'unexist': No such file or directory \
make: *** [error_in_recipe.mk:39: all1] Error 2
all1:
	@$(func1)

#1:hBc \
2:ehBc \
ls: cannot access 'unexist': No such file or directory \
3:hBc

all2:
	@$(func2)

#for multi subshell, each will run one line shell "script", once one subshell fails, make exits and there is\
 no later sub-shell to run the remaining lines
define multi-subshell
	echo 1
	ls unexist
	echo 2
endef

#even -k is added for launching make, there is no difference in output
#$ make -f error_in_recipe.mk all3 \
1 \
ls: cannot access 'unexist': No such file or directory \
make: *** [error_in_recipe.mk:62: all3] Error 2

#to make all cmds in each subshell run even after some of them fail, use -i
#$ make -f error_in_recipe.mk -i all3 \
1 \
ls: cannot access 'unexist': No such file or directory \
make: [error_in_recipe.mk:65: all3] Error 2 (ignored) \
2

all3:
	@$(multi-subshell)

#the foo bar is at the same level, foo's build failure will stop the make process so there is no bar's build
#$ make -f error_in_recipe.mk all4 \
foo \
[ 3 == 2 ] \
make: *** [error_in_recipe.mk:79: foo] Error 1 

#to make bar's building goes on even foo build has failed.
#$ make -f error_in_recipe.mk all4 -k \
foo \
[ 3 == 2 ] \
make: *** [error_in_recipe.mk:85: foo] Error 1 \
bar \
make: Target 'all4' not remade because of errors. \

all4:foo bar
	@echo $@
foo:
	@echo $@
	[ 3 == 2 ]
bar:
	@echo $@
#1. when there is no a.o, b.o, ab.out:$ make -f error_in_recipe.mk all5 -n \
touch a.o \
touch b.o \
cat a.o b.o > ab.out \

#2. After building once with `make -f error_in_recipe.mk all5`:\
$ make -f error_in_recipe.mk all5 -n \
make: Nothing to be done for 'all5'.

#3. touch a.c to make it is newer than a.o, then: \
$ make -f error_in_recipe.mk all5 -n \
touch a.o \
cat a.o b.o > ab.out

#4.after build fin: $ make -f error_in_recipe.mk all5 \
make: Nothing to be done for 'all5'. \
note the following -W b.c will not change b.c's timestamp, but the make works like b.c has been touched \
$ make -f error_in_recipe.mk all5 -W b.c \
touch b.o \
cat a.o b.o > ab.out


all5: ab.out
ab.out: a.o b.o
	cat $^ > $@
a.o: a.c
#	[ 1 == 0 ]
	touch $@
b.o: b.c
	touch $@