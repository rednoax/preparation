#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>//build error otherwise
#define report_func() printf("%s %p\n", __PRETTY_FUNCTION__, this)
#define report_line() printf("%s %d\n", __PRETTY_FUNCTION__, __LINE__)
template<typename T>
class sp
{
public:
	sp(): m_ptr(NULL)
	{
		report_func();
	}
	sp(const sp<T>& other):m_ptr(other.m_ptr)
	{
		report_func();
		if (m_ptr)
			m_ptr->incStrong(this);
	}
	~sp()
	{
		report_func();
		if (m_ptr)
			m_ptr->decStrong(this);
	}
	bool operator!=(const T* o) const
	{
		report_func();
		return m_ptr != o;
	}
	sp<T>& operator=(T* other)
	{
		report_func();
		if (other != NULL)
			other->incStrong(this);
		if (m_ptr != NULL)
			m_ptr->decStrong(this);
		m_ptr = other;
		return *this;
	}
	T* operator->() const
	{
		report_func();
		return m_ptr;
	}
	T* get() const {return m_ptr;}
private:
	T *m_ptr;
};

class ProcessState
{
public:
	static sp<ProcessState> self();
	ProcessState(): mCount(0)
	{
		report_func();
	}
	~ProcessState()
	{
		report_func();
	}
	void incStrong(const void* id)
	{
		int org = mCount;
		mCount += 1;
		printf("inc %d=%d\n", org, mCount);
	}
	void decStrong(const void *id)
	{
		int org = mCount;
		mCount -= 1;
		printf("dec %d=%d\n", org, mCount);
		if (mCount == 0)
			delete this;
	}
	void getContextObject()
	{
		report_func();
	}
private:
	int mCount;
};

class IPCThreadState
{
public:
	IPCThreadState():mProcess(ProcessState::self())
	{
		report_func();
	}
	~IPCThreadState()
	{
		report_func();
	}
	static int mVar;//=1 is error:ISO C++ forbids in-class initialization of non-const static member 'IPCThreadState::mVar'
	enum
	{
		is_pointer = 3,//it is just like static var, which can be accessed even no IPCThreadState object is instantiated
	};//build error if no ';'
private:
	sp<ProcessState> mProcess;
};
int IPCThreadState::mVar = 1;
sp<ProcessState> gProcess;
sp<ProcessState> ProcessState::self()
{
	if (gProcess != NULL)
		return gProcess;
	gProcess = new ProcessState;
	return gProcess;
}

/*
sp<T>::sp() [with T = ProcessState] 0x5583fd8c4020<--global gProcess ctor
bool sp<T>::operator!=(const T*) const [with T = ProcessState] 0x5583fd8c4020<--overloaded !=
ProcessState::ProcessState() 0x5583fe289280<--ProcessState's ctor
sp<T>& sp<T>::operator=(T*) [with T = ProcessState] 0x5583fd8c4020<--overloaded = of sp<T>
inc 0=1
sp<T>::sp(const sp<T>&) [with T = ProcessState] 0x7ffe3b531fc0<--a temp sp<ProcessState> object is constructed by copy ctor
inc 1=2<---the copy ctor for the temp object will inc ProcessState's mCount
T* sp<T>::operator->() const [with T = ProcessState] 0x7ffe3b531fc0<--overloaded "->" whose this==the temp object's this
void ProcessState::getContextObject() 0x5583fe289280<--ProcessState's getContextObject is called
sp<T>::~sp() [with T = ProcessState] 0x7ffe3b531fc0<--the destruction of the temp obj, when main exits
dec 2=1
sp<T>::~sp() [with T = ProcessState] 0x5583fd8c4020<--the destruction of the global obj, when main exits
dec 1=0
ProcessState::~ProcessState() 0x5583fe289280<--when mCount drop to 0, "delete this" will trigger ProcessState's dtor
*/
void func1()
{
	ProcessState::self()->getContextObject();
}

/*
IPCThreadState* func3() 138
bool sp<T>::operator!=(const T*) const [with T = ProcessState] 0x561fab317020
sp<T>::sp(const sp<T>&) [with T = ProcessState] 0x561fac4a52a0<--a temp obj is constructed with copy cons and "the temp object" is used as member @mProcess
inc 1=2<--the gProcess object's mCount will be incremented by 1 because of copy ctor
IPCThreadState::IPCThreadState() 0x561fac4a52a0<---IPCThreadState's ctor, whose this should==the 1st member @mProcess's this
IPCThreadState::~IPCThreadState() 0x561fac4a52a0<---IPCThreadState's dtor
sp<T>::~sp() [with T = ProcessState] 0x561fac4a52a0<--the mProcess obj is destructed with dtor
dec 2=1<---~sp() will decrement ProcessState object pointed by mProcess
*/
IPCThreadState* func3()
{
	report_line();
	IPCThreadState* p = new IPCThreadState;
	delete p;
	return NULL;
}

void func2()
{
	sp<ProcessState> &ref = *(&gProcess);
	sp<ProcessState> &ref2 = gProcess;
	sp<ProcessState> *p0 = &ref, *p1 = &ref2;
	printf("%p, %p %p; %p %p\n", &gProcess, p0, p1, &ref, &ref2);//all==&gProcess
}

void func4()//enum test
{
	printf("mVar %d, is_pointer %d\n", IPCThreadState::mVar, IPCThreadState::is_pointer);
}

/*
0x55778201e2a0 (nil)<--storage is the same as placement new's this
sp<T>::sp(const sp<T>&) [with T = ProcessState] 0x55778201e2a0<--this is the same as below, and @storage
inc 1=2
0x55778201e280==0x55778201e280
sp<T>::~sp() [with T = ProcessState] 0x55778201e2a0<--this is the same as the above, and @storage
dec 2=1
*/
void splat_type()
{
	int size = sizeof(sp<ProcessState>);
	void * storage = malloc(size);
	memset(storage, 0, size);
	printf("%p %p\n", storage, static_cast<sp<ProcessState>*>(storage)->get());//should be null
	new (static_cast<sp<ProcessState>*>(storage)) sp<ProcessState>(gProcess);//copy cons
	printf("%p==%p\n", static_cast<sp<ProcessState>*>(storage)->get(), gProcess.get());
	static_cast<sp<ProcessState>*>(storage)->~sp();//You are also solely responsible for destructing the placed object. This is done by explicitly calling the destructor:
	free(storage);
}

struct handle_entry
{
	int i;
	handle_entry()
	{
		report_func();
	}
	~handle_entry()
	{
		report_func();
	}
};
/*
handle_entry::handle_entry() 0x7fff4b302624
handle_entry::~handle_entry() 0x7fff4b302624
*/
void struct_ctor()
{
	handle_entry e;
}

int main()
{
	func1();
	func2();
	func3();
	func4();
	splat_type();
	struct_ctor();
	return 0;
}
