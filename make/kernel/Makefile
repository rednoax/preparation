#note _all has 2 rules, all of whose prerequisites will be merged into one list of prerequisites\
for the target. And there can only be 1 recipe among mutliple rules. If more than 1 rule gives a\
recipe for the same file, make uses the last one given and print an error message.

#Besides if no _all is put here, all becomes the default target when `make`
_all: 0
all:
	@echo $@
_all: 1
#1
#0
#_all,1 0
	@echo $@,$^
0 1:
	@echo $@