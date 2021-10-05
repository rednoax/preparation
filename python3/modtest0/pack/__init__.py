#init.py has 2 functions:
#1.convert a folder of scripts into an importable package of module(before Python3.3)
#2.run package initialization code
print(__file__)
class BPF:
    print("BPF")
    def __init__(self, str):
        self.str = str
    def print(self):
        print(self.str)
str = 'pack'