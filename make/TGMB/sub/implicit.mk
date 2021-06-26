$(info $(LINK.c),$^,$(LOADLIBES),$(LDLIBS))
$(info $(COMPILE.c),$(OUTPUT_OPTION))
#Each time running `make all`, : will AL be run.\
REASON:a.out's rule has no prerequisite, and the target of the rule, ie a.out,is a nonexisting\
file. So its reipes,even empty, is run, then make imagines the target a.out to have been updated.\
This implies that all targets depending on a.out will AL have their recipe run.
all:a.out
	:
a.out:;

#Each time running `make all2`,: will AL be run.See above for the reason.\
Here Force's rule's recipe is empty, which is equivalent to FORCE:;
all2:FORCE
	:
FORCE: