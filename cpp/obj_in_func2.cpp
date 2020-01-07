#include <stdio.h>
#define report_func() printf("%s %p\n", __PRETTY_FUNCTION__, this)

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

int main()
{
	func1();
	return 0;
}
