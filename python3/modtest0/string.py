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
print(i)
if i == 0:
    import mod
#if NO 'import mod': NameError: name 'mod' is not defined
    print(mod.i)
else:
    print("i!=0")
    from mod import i
    print(i)
#when a module is imported('import module' or 'from xx import object'), Python run all of the code in the module file.
