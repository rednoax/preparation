#1. if no a.out or fakemake.c is newer than a.out:\
mkdir sub || touch sub \
mkdir: cannot create directory ‘sub’: File exists \
gcc -Wall sub/fakemake.c -o a.out \
echo a.out \
a.out

#2. if a.out is newer than fakemake.c, $(objdir)'s recipe still runs as it is forced, and the sub/ timestamp is changed.\
 However, sub/fakemake.c 's timestamp is kept so no need to update a.out: \
mkdir sub || touch sub \
mkdir: cannot create directory ‘sub’: File exists

objdir=sub
#if removing |, then each time: \
mkdir sub || touch sub \
mkdir: cannot create directory ‘sub’: File exists \
gcc -Wall sub/fakemake.c -o a.out \
echo a.out \
a.out
#so we should insist adding | for (objdir) since it should not affect a.out's building. The $(objdir) is just to hold .c so \
 it should be run only when necessary
a.out:$(objdir)/fakemake.c  | $(objdir)
	gcc -Wall $< -o $@
	echo $@

$(objdir): FORCE
	mkdir $@ || touch $@
FORCE: