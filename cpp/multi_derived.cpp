#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define report_func() printf("%s: %p\n", __PRETTY_FUNCTION__, this)
#if 0
BpServiceManager--->BpInterface<IServiceManager>-------------------------->BpRefBase--->RefBase
                                                |                                           /|\
                                                |_\IServiceManager_\IInterface_______________|
							                      /                /
#endif
class RefBase
{
protected:
	RefBase()	{ report_func(); }
	virtual ~RefBase() { report_func(); }
};

class BpRefBase: public virtual RefBase
{
protected:
	BpRefBase() {report_func();}
	virtual ~BpRefBase() {report_func();}
};

template <typename INTERFACE>
class BpInterface: public INTERFACE, public BpRefBase
{
public:
	BpInterface() {report_func();}
	virtual ~BpInterface() {report_func();}
};

class IInterface: public virtual RefBase
{
public:
	IInterface() {report_func();}
protected:
	virtual ~IInterface() {report_func();}
};

class IServiceManager: public IInterface
{
public:
	IServiceManager() {report_func();}
	~IServiceManager() {report_func();}
};

class BpServiceManager: public BpInterface<IServiceManager>
{
public:
	BpServiceManager() {report_func();}
	~BpServiceManager() {report_func();}
};
int multi_test()
{
	printf("cons start\n");
	IServiceManager *intr = new BpServiceManager();//malloc mem, then ctor from base class to derived class
	printf("cons end, delete start\n");
	delete intr;//dtor from derived class to base class, then free mem
	printf("delete end\n");
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
