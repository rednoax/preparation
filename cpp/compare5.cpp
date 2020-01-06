//use count to solve the ProcessState object is destructed twice in compare4.cpp
#include <stdio.h>
#define report_function() printf("%s: %p\n", __PRETTY_FUNCTION__, this)
#define report_line() printf("%s: %d\n", __PRETTY_FUNCTION__, __LINE__)

template<typename T>
class sp
{
public:
	inline sp():m_ptr(0)
	{
		report_function();
	}
	sp(T* other): m_ptr(other)//not "const T* other" as typeof(m_ptr) is T* rather than (const T*)
	{
		report_function();
	}
	sp(const sp<T>& other): m_ptr(other.m_ptr)
	{
		report_function();
		if (m_ptr != NULL)
			m_ptr->incStrong();
	}
	~sp()
	{
		report_function();
		//delete m_ptr;
		if (m_ptr)
			m_ptr->decStrong();
		report_line();
	}
	sp& operator=(T* other)
	{
		if (other)
			other->incStrong();
		if (m_ptr)
			m_ptr->decStrong();
		m_ptr = other;
		report_function();
		return *this;
	}
	inline bool operator !=(const sp<T>& o) const {
		report_function();
		return m_ptr != o.m_ptr;
	}
	inline bool operator !=(const T* o) const {
		report_function();
		return m_ptr != o;
	}
	template<typename U>
	inline bool operator !=(const sp<U>& o) const {
		report_function();
		return m_ptr != o.m_ptr;
	}
	template<typename U>
	inline bool operator !=(const U* o) const {
		report_function();
		return m_ptr != o;
	}
private:
	T *m_ptr;
};

class ProcessState
{
public:
	static sp<ProcessState> self();
	ProcessState()
	{
		report_function();
		mCount = 0;
	}
	~ProcessState()
	{
		report_function();
	}
	void incStrong()
	{
		int org = mCount;
		mCount += 1;
		printf("inc %d=>%d\n", org, mCount);
	}
	void decStrong()
	{
		int org = mCount;
		mCount -= 1;
		printf("dec %d=>%d\n", org, mCount);
		if (mCount == 0)
			delete this;
	}
private:
	int mCount;
};

sp<ProcessState> gProcess;
sp<ProcessState> ProcessState::self()
{
	if (gProcess != NULL)//NULL will recognised as (const ProcessState*) when reloading "!="
		return gProcess;
	gProcess = new ProcessState;//when fin, ProcessState's mCount is 1
	report_line();
	return gProcess;
}
/*
sp<T>::sp() [with T = ProcessState]: 0x55d521d2b020<--global sp<ProcessState> ctor
int main(): 122
bool sp<T>::operator!=(const T*) const [with T = ProcessState]: 0x55d521d2b020
ProcessState::ProcessState(): 0x55d523376280<--new ProcessState
inc 0=>1<---sp's operator= will inc ProcessState's mCount from 0 to 1
sp<T>& sp<T>::operator=(T*) [with T = ProcessState]: 0x55d521d2b020<---sp<ProcessState>'s operator=
static sp<ProcessState> ProcessState::self(): 101
sp<T>::sp(const sp<T>&) [with T = ProcessState]: 0x7fff0e1ba3b0<--return gProcess  will cons a temp sp<ProcessState> object using copy ctor
inc 1=>2<-----------------temp object's copy ctor will inc ProcessState's mCount
sp<T>::~sp() [with T = ProcessState]: 0x7fff0e1ba3b0<--the temp object's dtor is called, which will dec ProcessState object's mCount to 1
dec 2=>1<--ProcessState's count is 1, which will not trigger ProcessState object' dtor
sp<T>::~sp() [with T = ProcessState]: 30
int main(): 124
sp<T>::~sp() [with T = ProcessState]: 0x55d521d2b020<---global sp<ProcessState> dtor will make ProcessState object mCount to be 0,which will trigger release of ProcessState object
dec 1=>0
ProcessState::~ProcessState(): 0x55d523376280<--ProcessState dtor is called only once, which fix compare4.cpp's bug
sp<T>::~sp() [with T = ProcessState]: 30
*/
int main()
{
	report_line();
	ProcessState::self();
	report_line();
	return 0;
}
