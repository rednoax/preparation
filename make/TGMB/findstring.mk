$(info [$(findstring a,aa a)])#a
$(info [$(findstring a,aa)])#a
$(info [$(findstring s,stella)])#s
$(MAKECMDGOALS):
#make -f findstring.mk
#make: *** No targets.  Stop.\
shell returned 2
	@echo $@
	printenv | grep MAKECMDGOALS
#there will be no MAKECMDGOALS in child process's env
