#this makefile is to check variable from its parent make
all:

check_vars=$(foreach v,$1,$(info $v=$($v)	$(origin $v)))
vars=MAKE MAKEFLAGS MAKELEVEL MAKEFILES SHELL VAR0 VAR1 VAR2 VAR3 VAR4 VAR5
$(call check_vars,$(vars))
