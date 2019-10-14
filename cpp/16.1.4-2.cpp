#include <stdio.h>

template <typename T0, typename T1>

#define report_func() printf("%30s: %p\n", __PRETTY_FUNCTION__, this)
class sp0
{
public:
	sp0(T0 &other)
	{
		report_func();
	}
	sp0(T1 &other)
	{
		report_func();
	}
};

template <typename T>
class sp1
{
public:
	sp1(T &other)
	{
		report_func();
	}
/*
The following statement 1 seems NOT be the truth! But 2 makes sense:sp1(U&) is only used when sp1(T&) cannot be matched
https://en.cppreference.com/w/cpp/language/member_template
1. Destructors and copy constructors cannot be templates. If a template constructor is declared which could be instantiated with the
type signature of a copy constructor, the implicitly-declared copy constructor is
2. A non-template member function and a template member function with the same name may be declared. 
In case of conflict (when some template specialization matches the non-template function signature exactly), 
use of that name and type refers to the non-template member unless an explicit template argument list is supplied. 
*/
	template<typename U> sp1(U &other)//one template function in template class
	{
		report_func();
	}
};

int main()
{
	int i = 0;
	double d = 0.1;
/*
	sp0<int,int> v0(i);//err for sp0<int,int>'s 2 cons are exactly the same!
The wrong info:
template.cpp:37:19:   required from here
template.cpp:13:2: error: 'sp0<T0, T1>::sp0(T1&) [with T0 = int; T1 = int]' cannot be overloaded
  sp0(T1 &other)
  ^~~
template.cpp:9:2: error: with 'sp0<T0, T1>::sp0(T0&) [with T0 = int; T1 = int]'
  sp0(T0 &other)
  ^~~
*/
	sp0<int, double> v1(i);//ok, you must specify two type at the same time; Not flexible!
/*
ok, you need not to specify its U's type(in template function) & T's type at the same time, only T's type needs to be given.
type U need not to be given by programer. The template function will be specialized from the function parameter when dynamically
running. It is more flexible than sp0<T, U> that you must explicitely specify type U with T at the same time.
*/
	sp1<int> v2(i);//ok: will match sp1<T>::sp1(T&) [with T = int] rathen then sp1(U&), see statement 2 of the above comments
	sp1<int> v3(d);//ok: sp1<T>::sp1(U&) [with U = double; T = int], it match sp1(U &):sp1(U&), which is ONLY used when sp1(T&) cannot be matched
	return 0;
}