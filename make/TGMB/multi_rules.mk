#$ make -f multi_rules.mk z228_config\
: 1\
$ make -f multi_rules.mk mini2440_config\
: 2
#more specific rule wins

#$make -f multi_rules.mk\
: 2

#$make -f multi_rules.mk _config \
make: *** No rule to make target '_config'.  Stop.\
\
shell return 2
#So the % in target can ONLY match >=1 chars
#$make -f multi_rules.mk 1_config\
: 1
%_config:
	: 1
mini2440_config:
	: 2
