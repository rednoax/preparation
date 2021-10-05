#!/usr/bin/python
import sys
script, i = sys.argv
i = int(i)
if 0<=i<=1:
    if i == 0:
        import packA
    elif i == 1:
        import packA.a1
#work just the same as 'import packA': both packA/__init__.py and a1.py are run once. You can see their globl print output.
#a1 is imported twice:1st in packA/__init__.py then here, a1's globl debug output show it is actually only import once,
# the 2nd import packA.a1 is passed since a1.py has been imported.
    packA.packA_func()
    packA.a1_func()
    packA.a1.a1_func()
#the sequence of files to be run in [2,6]: packA/__init__.py, packA/a1.py, packA/subA/__init__.py, packA/subA/sa1.py
# [2,6] show different ways to call helloworld() in sa1.py
elif i == 2:
    import packA as pA, packA.a1,packA.subA.sa1 as sa1
    sa1.helloworld()
elif i == 3:
    from packA.subA.sa1 import helloworld
    helloworld()
elif i == 4:
    from packA.subA import sa1
    sa1.helloworld()
elif i == 5:
    import packA.subA.sa1 as sa1 #equals to i==4
    sa1.helloworld()
elif i == 6:
    import packA.subA.sa1
    packA.subA.sa1.helloworld()