export FOO=bar

$(info $(shell printenv|grep FOO)>)#this cannot print FOO=bar as expected for $(shell ...) function's env is the same as the make process that parsed
#this makefile, this make process is also called parent process and all child process(eg rules cmd and $(shell ...)) in this makefile is launched by this parent.
#There is no FOO when parent make is launched.

test=$(shell echo $2+$1)
$(info $(call test,FOO,printenv|grep FOO)-1)#will be empty, because:echo printenv|grep FOO+FOO
test2=$(shell echo $1+$2)
$(info $(call test2,FOO,printenv|grep FOO)-2)#will be FOO+printenv, because:echo FOO+printenv|grep FOO
test3=$(shell echo $1 $2)
$(info $(call test3,FOO,printenv|grep FOO)-3)#will be FOO printenv, because:echo FOO printenv|grep FOO
test4=$(shell echo $2)
$(info $(call test4,FOO,printenv|grep FOO)-4)#will be empty,because:echo printenv|grep FOO

env_file=/tmp/env
#1.$(shell ...)'s value: shell's output of [1] rather than [2], and serials of \n will be replaced with <space>
#2.foreach will replace $V first so you get $(shell echo export FOO=$(FOO)>>$(env_file), then shell function will do replace like $$ and $(...);
#  then fork+exec a child process to run:echo export FOO=bar>>/tmp/env;
#  In brief: $(foreach ...) does one time expanding for $V, then $(shell ...) does one time expanding for $$ and $(...);
#  So each function will ONLY do just one time, not >1 time expanding???
#3.in echo '$2' below, '' or "" is abs necessary. See above test example for the reason
#4.there is no space among env_shell defination.There are 4 groups, only the last group contributes to the value of env_shell.the former 3 are all empty
env_shell=$(shell rm -rf $(env_file))$(foreach V,$1,$(shell echo export $V=$($V)>>$(env_file)))$(shell echo '$2'>>$(env_file))$(shell bash -e $(env_file))
$(info $(shell echo 1) $(shell echo 2))
$(info $(call env_shell,FOO,printenv|grep FOO)])

all:;@printenv | grep FOO