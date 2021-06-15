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

