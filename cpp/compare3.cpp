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
	return NULL;//gProcess;
}
/*
sp<T>::sp() [with T = ProcessState]: 0x5638d2716020
int main(): 88
bool sp<T>::operator!=(const T*) const [with T = ProcessState]: 0x5638d2716020
ProcessState::ProcessState(): 0x5638d383c280
sp<T>& sp<T>::operator=(T*) [with T = ProcessState]: 0x5638d2716020
static sp<ProcessState> ProcessState::self(): 71
sp<T>::sp(T*) [with T = ProcessState]: 0x7ffd91f0f560<--NULL is used as (const ProcessState*) to cons a temp object
sp<T>::~sp() [with T = ProcessState]: 0x7ffd91f0f560<--the temp object dtor is called when exiting self();
sp<T>::~sp() [with T = ProcessState]: 21
int main(): 90
sp<T>::~sp() [with T = ProcessState]: 0x5638d2716020<--global sp<ProcessState>'s dtor is called when exiting main
ProcessState::~ProcessState(): 0x5638d383c280
sp<T>::~sp() [with T = ProcessState]: 21
*/
int main()
{
	report_line();
	ProcessState::self();
	report_line();
	return 0;
}
