#include <stdio.h>
//initializer list test: why it is preferred by assignment in the constructor body?
/*
source is based on:https://www.geeksforgeeks.org/output-of-c-program/ ;
It is used to test why initializer list is necessary in point6 of
https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/
PLZ REMEMBER the following!!!:
6) For Performance reasons:
It is better to initialize all class variables in Initializer List instead of assigning values inside body. 
*/
/*
the full function name by g++/clang
http://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
*/
#define REPORT_FUNC() printf("%30s: %p\n", __PRETTY_FUNCTION__, this)
class A {
public:
	A()
	{
		REPORT_FUNC();
	}
	A(const A &a)
	{
		REPORT_FUNC();
	}
	A& operator=(const A &a)
	{
		if (this == &a)
			return *this;
		REPORT_FUNC();
		return *this;
	}
	~A()
	{
		REPORT_FUNC();
	}
};

class B {
	A a;
public:
	B(A &a)
/*
class member "a" has been constructed before running the following function body!
That's why "this" can be used in the function body(Actually, "this" can be used in initializer list as well)
the constructor of class B is not efficient as member 'a' has finished construction with default constructor
before ctor's {}; and then the values from the parameter are copied using assignment operator.
In general, it is a good idea to use Initializer List to initialize all members of a class, because it saves one extra assignment of members. 
*/
	{
		REPORT_FUNC();
		this->a = a;
	}
};

class B1 {
	A a;
public:
	/*
	The constructor of class B1 now uses initializer list to initialize its member ‘a’.
	When Initializer list is used, the member ‘a’ of class B is initialized directly from the parameter.
	So a call to A’s constructor is reduced.
	**PAY ATTENTION: we don't study when the memory is allocated(in initializer list?). We
	just needs to know that class member cons and base class cons can be put in initializer list
	to avoid their default cons, if you don't use initializer list;
	*/
	B1(A &a): a(a)
	{
		REPORT_FUNC();
	}
};

class C: public A{
	A a;
public:
	C(A &a)//initializer list should be ClassName(...) for base class, membername(...) for class member
	{
		this->a = a;
		REPORT_FUNC();
	}
};

class D: public A {
	A a;
public:
	D(A &a): a(a), A(a)//warning will emit, the real cons sequnce is still "A(a), a(a)"
	{
		this->a = a;
		REPORT_FUNC();
	}
};

class E{
private:
	A* const m_pA;
public:
/*
7.5.1.cpp: In constructor 'E::E(A*)':
7.5.1.cpp:93:2: error: uninitialized const member in 'class A* const' [-fpermissive]
  E(A* pA)
  ^
7.5.1.cpp:91:11: note: 'A* const E::m_pA' should be initialized
  A* const m_pA;
		   ^~~~
7.5.1.cpp:95:10: error: assignment of read-only member 'E::m_pA'
   m_pA = pA;
		  ^~
	//wrong initialization that emits error of the above
	E(A* pA)
	{
		m_pA = pA;
	}
	//
*/
/*
https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/
1.For initialization of non-static const data members:const data members must be initialized
using Initializer List.
*/
	E(A* pA):m_pA(pA){}//right
};

int main()
{
	A a;
	/*4 times
					   A::A(): 0x7ffe33a4dd06
					 B::B(A&): 0x7ffe33a4dd06
	A& A::operator=(const A&): 0x7ffe33a4dd06
					  A::~A(): 0x7ffe33a4dd06
	*/
	{
		B b(a);
	}
	/*2 times:
                A::A(const A&): 0x7ffde70bc005
                    B1::B1(A&): 0x7ffde70bc005
	*/
	B1 b1(a);
	/*object c's base part address == c's address but c's member address != c's address
                        A::A(): 0x7fff94061166<--class C and A should be of the same "this"
                        A::A(): 0x7fff94061167<--- member a's ctor report member a's this
     A& A::operator=(const A&): 0x7fff94061167<--- member a's member func whose reported "this" should==member a's this
                      C::C(A&): 0x7fff94061166<--class C and A should be of the same "this"
	*/
	C c(a);
	/*
					A::A(const A&): 0x7ffe7114fdf6
					A::A(const A&): 0x7ffe7114fdf7
		 A& A::operator=(const A&): 0x7ffe7114fdf7
						  D::D(A&): 0x7ffe7114fdf6
	*/
	D d(a);
	return 0;
}
