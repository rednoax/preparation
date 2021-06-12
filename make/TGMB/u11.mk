# * expansion is done by make in target/prerequisite, by shell in recipe. \
 in other context of makefile, * is not expanded unless you use $(wildcard ...) \
 * expansion effect by make/bash IS the SAME when such files exist: eg in shell : \
 $echo *.c will list all xx.c files in current dir, but not '.c'\
 and $(wildcard *.c) will print exactly the same results as bash's echo result.\
 when no such files, bash return its literally string and $(wildcard ...) return empty string as the later only shows the existing\
 in shell:\
 $echo *.a will generate *.a if there is no any files suffixed with .a, \
 $ls *.a will emit 'ls: cannot access *.a: No such file or directory' if there is no files of xx.a\
 and $(wildcard *.a) will print a empty string as no such existing files
 
$(info [$(wildcard *.c)])#impossible show '.c' file even * in make means >=0 chars
$(info [$(wildcard s*.mk)])#impossible show sub/sub.mk, so empty string returned
$(info {${wildcard *.a}})#Since no *.a the empty string returned
$(info [$(wildcard sub/s*.mk)])#can show sub/sub.mk
${info 1-$(patsubst %.c,%.o,.c)}#.o
$(info 2-$(patsubst %.c.%,%.o, a.c.% .c))#a.o .c

org=the\%weired\\%pattern\\
$(info [$(org)])#[the\%weired\\%pattern\\]

#an aliternative way equivalent to $(patsubst ...)
var=a.o.o b.o 	#note there is a trailing space and \t after b.o, the following substituion can replace b.o with b.c
$(info $(var):$(var:.o=.c)])#a.o.c b.c
$(info $(var:o=c)])#a.o.c b.c
all:
#impossible show '.c' file even * in bash means >=0 chars
	@ls *.c
#impossible show sub/sub.mk, the ||: is to make the last cmd run as the 2nd cmd still succeeds.
	@ls s*.mk || :
#can show sub/sub.mk
	@ls sub/s*.mk

#make -f u11.mk all2
#since there is no .a file in current dir, the expansion fails: an oddly-name file '*.a' is used as prerequisite so make fails: \
 make: *** No rule to make target '*.a', needed by 'all2'.  Stop.
all2:*.a
	@: