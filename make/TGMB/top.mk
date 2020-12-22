#TO TEST:
#you need: FOO=env TEMP=temp make -f top.mk
#you need not:export FOO=env;TEMP=temp make -f top.mk
export TOP=top
all:;@make -C sub -f u4.mk

#rednoah@lucia:~/preparation/make/TGMB$ make -f top.mk
#make[1]: Entering directory '/home/rednoah/preparation/make/TGMB/sub'
#0: ./fakemake.out
#1: filetoo<--note there is only 2 arguments: $FOO is empty as FOO in u4.mk is not exported
#VVVVARVVV
#...
#11: MAKEFLAGS=w
#...
#18: BAR=filetoo<--BAR is exported in u4.mk
#...
#21: MAKELEVEL=2
#...
#26: SHELL=/bin/bash
#27: TOP=top<--exported TOP in top.mk will be env of submake(run u4.mk) and submake's u4.mk's child process fakemake.out will inherit submake's env.
#...
#38: MFLAGS=-w
#39: LC_PAPER=zh_CN.UTF-8
#make[1]: Leaving directory '/home/rednoah/preparation/make/TGMB/sub'

#rednoah@lucia:~/preparation/make/TGMB$ FOO=env TEMP=temp make -f top.mk<--the env set on cmd line will be transfered as env to make and ALL its child process. Besides, its value will be overriden by file var defined in makefile, if any.
#make[1]: Entering directory '/home/rednoah/preparation/make/TGMB/sub'
#0: ./fakemake.out
#1: file<--FOO is file, not env, as u4.mk has overlay FOO in file var
#2: filetoo
#VVVVARVVV
#...
#11: MAKEFLAGS=w
#...
#15: TEMP=temp<---
#...
#19: BAR=filetoo<---
#20: FOO=file<---
#...
#23: MAKELEVEL=2<---
#...
#28: SHELL=/bin/bash
#29: TOP=top<---
#...
#40: MFLAGS=-w
#41: LC_PAPER=zh_CN.UTF-8
#make[1]: Leaving directory '/home/rednoah/preparation/make/TGMB/sub'
