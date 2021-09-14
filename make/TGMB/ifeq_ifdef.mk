foo=
bar=$(foo)
#ifdef arg can be a variable-name w/t $ or $(...)\
to pass ifdef, foo must be defined and its 1st expanded value must be non empty.(the 2nd expanded value can be empty)

ifdef foo#ifdef is like ifneq ($(foo),), but not completely the same
$(info foo defined)
else
$(info foo !defined)#
endif

ifndef foo
$(info foo !defined)#
else
$(info foo defined)
endif

ifneq ($(foo) , )#space around , is not considered
$(info foo not empty)
else
$(info foo empty)#behave like the above ifdef
endif

ifdef bar
$(info bar defined)#so ifdef is not AL the same as ifneq ($(bar),)
else
$(info bar !defined)
endif

ifndef bar
$(info bar !defined)
else
$(info bar defined)#
endif

ifneq ($(bar),)
$(info bar not empty)
else
$(info bar empty)#
endif

v0=v
v1=$(v0)
v2=$(v1)

ifeq ($(v2),)#expanded untile no $, ie ifeq(v,), so it can't run the following
$(info v2 is actually empty)
endif

ifdef $(v2)#the arg will be expanded until no $, at last it is 'v' and v is not defined
$(info $(v2) defined)
else
$(info $(v2) !defined)#
endif

ifeq ($(origin M)   ,    command line)
$(info M defined as $M from "command line")#make -f xx M=.
else
$(info "M not from command line!")#make -f xx
endif
#see man 3 realpath for more \
a symbol link:return its real file in canonical absolute name \
an unexisting file: return empty string as it doesn't exist. \
existing files:return canonical absolute file; \
a canonical name dosen't contain any . or .. components, nor any repeated path seperators(/) or symlinks.

$(info [$(realpath /usr/bin/python /usr/lib/python false /usr/bin//ls ./ifeq_ifdef.mk)])
#stdout:[/usr/bin/python2.7 /usr/bin/ls /home/rednoah/opt/preparation/make/TGMB/ifeq_ifdef.mk]

#multi @ is ok and at last actually equals to one @
all:
#equals to '@echo "$@"'
	@@@@@echo "$@"