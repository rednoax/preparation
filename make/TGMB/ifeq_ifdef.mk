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

ifneq ($(foo),)
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

ifeq ($(v2),)#can't run the following
$(info v2 is actually empty)
endif

ifdef $(v2)#the arg will be expanded until no $, at last it is 'v' and v is not defined
$(info $(v2) defined)
else
$(info $(v2) !defined)#
endif
all:
