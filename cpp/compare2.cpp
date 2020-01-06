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
	static void /*sp<ProcessState>*/ self();
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
/*sp<ProcessState>*/void ProcessState::self()
{
	if (gProcess != NULL)//NULL will recognised as (const ProcessState*) when reloading "!="
		return;// gProcess;
	gProcess = new ProcessState;
	report_line();
	sp<ProcessState> local = new ProcessState;
	//return NULL;//gProcess;
}
/*
sp<T>::sp() [with T = ProcessState]: 0x562c763f3020<--global sp<ProcessState> ctor before entering main
int main(): 89
bool sp<T>::operator!=(const T*) const [with T = ProcessState]: 0x562c763f3020<--!= comparing will recognise NULL as (const ProcessState*)
ProcessState::ProcessState(): 0x562c774d7280<--ProcessState ctor
sp<T>& sp<T>::operator=(T*) [with T = ProcessState]: 0x562c763f3020<--sp's operator = is called rathen than its copy ctor, as global sp<ProcessState> has exists
static void ProcessState::self(): 71
ProcessState::ProcessState(): 0x562c774d72a0
sp<T>::sp(T*) [with T = ProcessState]: 0x7ffebf8ad820<--@local's copy ctor is called
sp<T>::~sp() [with T = ProcessState]: 0x7ffebf8ad820<--@local's dtor is called when self() exits
ProcessState::~ProcessState(): 0x562c774d72a0<--@local's dtor trigger its own ProcessState dtor
sp<T>::~sp() [with T = ProcessState]: 21
int main(): 91
sp<T>::~sp() [with T = ProcessState]: 0x562c763f3020<--global sp<ProcessState>'s dtor
ProcessState::~ProcessState(): 0x562c774d7280
sp<T>::~sp() [with T = ProcessState]: 21

*/
int main()
{
	report_line();
	ProcessState::self();
	report_line();
	return 0;
}
