#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define report_func() printf("%s: %p\n", __PRETTY_FUNCTION__, this)
#if 0
dtor of RefBase is claimed to be virtual, and at last the dtor of BpServiceManager is virtual too;
virtual in dtor of child can be ignored.It is virtual by default.
So the dtor function address in RefBase is actually the dtor of BpServiceManager.
delete some ptr of some base class, here the base class selects IServiceManager, will call dtor
of BpServiceManager.

BpServiceManager--->BpInterface<IServiceManager>-------------------------->BpRefBase--->RefBase
                                                |                                         /|\
                                                |_\IServiceManager_\IInterface_____________|
							                      /                /
#endif
class RefBase
{
protected:
	RefBase(int i = 0)	{ report_func(); printf("%d\n", i);}
	virtual ~RefBase() { report_func(); }
};
//even BpRefBase and IInterface has explicitely use RefBase() in initializer list, the
//virtual base class RefBase is only constructed once
class BpRefBase: public virtual RefBase
{
protected:
	BpRefBase(): RefBase(1)     {report_func();}
	virtual ~BpRefBase() {report_func();}
};

template <typename INTERFACE>
class BpInterface: public INTERFACE, public BpRefBase
{
public:
	BpInterface():RefBase(2) {report_func();}
	virtual ~BpInterface() {report_func();}
};

class IInterface: public virtual RefBase
{
public:
	IInterface():RefBase(3) {report_func();}
//protected:
	virtual ~IInterface() {report_func();}
};

class IServiceManager: public IInterface
{
public:
	IServiceManager()/*:RefBase(4)*/ {report_func();}//will call RefBase(0) by default
	~IServiceManager() {report_func();}//no virtual but it is actually virtual
};

class BpServiceManager: public BpInterface<IServiceManager>
{
public:
	BpServiceManager():RefBase(5) {report_func();}
	~BpServiceManager() {report_func();}//no virtual but it is actually virtual
};
int multi_test()
{
	printf("cons start\n");
/*
1.https://www.learncpp.com/cpp-tutorial/128-virtual-base-classes/
if a class inherits >=1 classes that have virtual parents, the most derived class is
responsible for constructing the virtual base class. this is true even in a single
inheritance chain. The other cons in middle class will be completely ignored.
2. However, if we were to create an instance of middle class, those constructor calls would be used,
and normal inheritance rules apply.
*/
	IServiceManager *intr = new BpServiceManager();//malloc mem, then ctor from base class to derived class
	printf("cons end, delete start\n");
	delete intr;//dtor from derived class to base class, then free mem
	printf("delete end\n");
	intr = new IServiceManager();//a single chain example:IServiceManager is the most derived class so its cons for virtual base class will take effect, IInterface's cons for RefBase is ignored
	delete intr;
	printf("delete end2\n");
	IInterface *p = new IInterface();//IInterface's cons for RefBase will take effect.
	delete p;
	return 0;
}

int main()
{
	multi_test();//n1
	return 0;
}
/*
n1:
cons start
RefBase::RefBase(): 0x55c805eb3280
IInterface::IInterface(): 0x55c805eb3280
IServiceManager::IServiceManager(): 0x55c805eb3280
BpRefBase::BpRefBase(): 0x55c805eb3288
BpInterface<INTERFACE>::BpInterface() [with INTERFACE = IServiceManager]: 0x55c805eb3280
BpServiceManager::BpServiceManager(): 0x55c805eb3280
cons end, delete start<---the following dtors are mirrorred with the above ctors
virtual BpServiceManager::~BpServiceManager(): 0x55c805eb3280
BpInterface<INTERFACE>::~BpInterface() [with INTERFACE = IServiceManager]: 0x55c805eb3280
virtual BpRefBase::~BpRefBase(): 0x55c805eb3288
virtual IServiceManager::~IServiceManager(): 0x55c805eb3280
virtual IInterface::~IInterface(): 0x55c805eb3280
virtual RefBase::~RefBase(): 0x55c805eb3280
delete end

*/
