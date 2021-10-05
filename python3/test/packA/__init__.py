#not "from subA import a1_func" as the sys.path comes from the top invoked script location:
#'python start.py' so sys.path includes 'test/' rather than 'test/subA'
print("packA/__init__.py imported")
from packA.a1 import a1_func #this import makes a1_func accessible from packA.a1_func
def packA_func():
    print("runnning packA_func()")

