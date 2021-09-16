$(info .DEFAULT_GOAL=$(.DEFAULT_GOAL))#.DEFAULT_GOAL is empty

#1.if .DEFAULT_GOAL is empty, it will be assigned to the 1st target it encounters
foo0 foo1:;@echo $@
$(info .DEFAULT_GOAL=$(.DEFAULT_GOAL))#.DEFAULT_GOAL=foo0,ie the 1st of targets
#2.once .DEFAULT_GOAL is !empty, it will not be assigned even there is new rules
bar:;@echo $@
$(info .DEFAULT_GOAL=$(.DEFAULT_GOAL))#.DEFAULT_GOAL is still foo0

#3.clear .DEFAULT_GOAL forcely, then it will be assigned to the 1st target it encounters. SAME AS 1.
.DEFAULT_GOAL=
foobar:;@echo $@
$(info .DEFAULT_GOAL=$(.DEFAULT_GOAL))#.DEFAULT_GOAL becomes foobar after .DEFAULT_GOAL is explicitely cleared

#NOW .DEFAULT_GOAL is foobar so `make` w/t target will run .DEFAULT_GOAL ie foobar
#make\
.DEFAULT_GOAL=\
.DEFAULT_GOAL=foo0\
.DEFAULT_GOAL=foo0\
.DEFAULT_GOAL=foobar\
foobar

#.DEFAULT_GOAL = foo0 foo1#wrong!make: *** .DEFAULT_GOAL contains more than one target.  Stop.<--the right value# can ONLY be <=1