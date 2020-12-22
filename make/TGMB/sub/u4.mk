#TO TEST:
#FOO=env TEMP=temp make -f u4.mk
#export FOO=env; TEMP=temp make -f u4.mk
#THE ABOVE 2 ARE THE EQUIVALENT

FOO=file
export BAR=filetoo
all:;@./fakemake.out $$FOO $$BAR

#rednoah@lucia:~/preparation/make/TGMB$ FOO=env TEMP=temp make -f u4.mk<--compare with the following cmd, so export of the 2nd cmd is not needed at all!
#0: ./fakemake.out
#1: file
#2: filetoo
#VVVVARVVV
#...
#15: TEMP=temp<--TEMP is env from cmd line, not overlayed as no file var will overlay it
#....
#19: BAR=filetoo<--exported by make
#20: FOO=file<---FOO is env from cmd line then it is overlayed by the same file var
#...
#23: MAKELEVEL=1<--sub process launched by make will show MAKELEVEL with 1 rather than 0
#...
#28: SHELL=/bin/bash
#...
#39: MFLAGS=
#...

#rednoah@lucia:~/preparation/make/TGMB$ export FOO=env; TEMP=temp make -f u4.mk<--the output is equal to the above
#0: ./fakemake.out
#1: file
#2: filetoo
#VVVVARVVV
#...
#11: MAKEFLAGS=
#...
#15: TEMP=temp
#...
#19: BAR=filetoo
#20: FOO=file
#...
#23: MAKELEVEL=1
#...
#28: SHELL=/bin/bash
#...
#39: MFLAGS=
#40: LC_PAPER=zh_CN.UTF-8


