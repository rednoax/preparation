#$ make -f multi_rules.mk z228_config\
: 1\
$ make -f multi_rules.mk mini2440_config\
: 2
#more specific rule wins
%_config:
	: 1
mini2440_config:
	: 2