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
	//return NULL;//gProcess;
}
/*
sp<T>::sp() [with T = ProcessState]: 0x55ec4b456020<--global sp() is called before starting main
int main(): 84
bool sp<T>::operator!=(const T*) const [with T = ProcessState]: 0x55ec4b456020<--overloaded "!=" is called
ProcessState::ProcessState(): 0x55ec4bd84280<--ProcessState's ctor
sp<T>& sp<T>::operator=(T*) [with T = ProcessState]: 0x55ec4b456020<--gProcess has existed so operator= rather than copy cons is used
static void ProcessState::self(): 71
int main(): 86
sp<T>::~sp() [with T = ProcessState]: 0x55ec4b456020<--global sp() dtor is called when main exits, which will delete ProcessState object
ProcessState::~ProcessState(): 0x55ec4bd84280
sp<T>::~sp() [with T = ProcessState]: 21
*/
int main()
{
	report_line();
	ProcessState::self();
	report_line();
	return 0;
}
