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
	sp(const sp<T>& other)
	{
		report_function();
		m_ptr = other.m_ptr;
	}
	~sp()
	{
		report_function();
		delete m_ptr;
		report_line();
	}
	sp& operator=(T* other)
	{
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
	}
	~ProcessState()
	{
		report_function();
	}
};

sp<ProcessState> gProcess;
sp<ProcessState> ProcessState::self()
{
	if (gProcess != NULL)//NULL will recognised as (const ProcessState*) when reloading "!="
		return gProcess;
	gProcess = new ProcessState;
	report_line();
	return gProcess;
}
/*
sp<T>::sp() [with T = ProcessState]: 0x56267b868020<--global ctor
int main(): 93
bool sp<T>::operator!=(const T*) const [with T = ProcessState]: 0x56267b868020
ProcessState::ProcessState(): 0x56267d3ef280
sp<T>& sp<T>::operator=(T*) [with T = ProcessState]: 0x56267b868020<---global sp<ProcessState>'s operator= is called
static sp<ProcessState> ProcessState::self(): 76
sp<T>::sp(const sp<T>&) [with T = ProcessState]: 0x7ffe2ddd5ae0<--a temp object is returned, so call copy ctor to cons it from @gProcess
sp<T>::~sp() [with T = ProcessState]: 0x7ffe2ddd5ae0<--then the temp obj is destructed when exiting self() 
ProcessState::~ProcessState(): 0x56267d3ef280<--the temp obj's ProcessState's dtor is called
sp<T>::~sp() [with T = ProcessState]: 26
int main(): 95
sp<T>::~sp() [with T = ProcessState]: 0x56267b868020<--global sp<ProcessState> dtor
ProcessState::~ProcessState(): 0x56267d3ef280<--ProcessState object is destructed twice, but no real mem ops so no segment fault
sp<T>::~sp() [with T = ProcessState]: 26
*/
int main()
{
	report_line();
	ProcessState::self();
	report_line();
	return 0;
}
