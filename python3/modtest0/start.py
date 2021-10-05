#!/usr/bin/python3
#ab<==like c's printf("fmt" "the other fmt");
print("a" 'b')
#ab
print('a' +  'b')
#a b
print('a','b')
#ValueError: invalid literal for int() with base 10: '10.0'
#print(int("10.0"))
#1
print(len('c'))
#1 too as "c" or 'c' are all string in python
print(len("c"))
#a variable type may be changed after assignment
#https://www.pythonmorsels.com/topics/2-types-change/
a = 1
print(type(a),id(a))
a = '1'
print(type(a),id(a))

from sys import argv
name, i = argv
i = int(i)
print(f"{i}:")
if i == 0:
    import mod
#if NO 'import mod': NameError: name 'mod' is not defined
    print(mod.i)
elif i == 1:
    from packA import BPF,str
    print(str)
    bpf = BPF("bpf")
    bpf.print()
elif i == 2:
#class BPF's static print() runs still even it has not been imported
    from packA import str
    print(str)
elif i == 3:
    import packA
    print(packA.str)
    bpf = packA.BPF("bpf2")
    bpf.print()
    print(packA) #<module 'packA' from '/home/rednoah/opt/preparation/python3/modtest0/packA/__init__.py'>
elif i == 4:
    import packB
    print(packB) #<module 'packAb' (namespace)>, different from the imported packAage that has '__init__.py'
else:
    print("i!=0")
    from mod import i
    print(i)
#when a module i.e. a file is imported, no matter using 'import module' or 'from module import object',
#Python run all of the code in the module file.
#To ref object in mod, the latter need not 'module.object' like the former but only 'object'

#when a packAage i.e. a dir is imported, no matter using 'import dir' or 'from dir import object',
#Python run ALL of __init__.py if any: if some object not imported, its static method runs still.
#To ref object in dir, the latter need not 'dir.object' like the former but only 'object'
