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
};

class B {
	A a;
public:
	B(A &a)
	/*
	class member "a" has been constructed before running the following function body!
	That's why "this" can be used in the function body;
	the constructor of class B is not efficient as member ‘a’ is first constructed with default constructor,
	and then the values from the parameter are copied using assignment operator.
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
int main()
{
	A a;
	/*3 times
                        A::A(): 0x7fff94061165
                      B::B(A&): 0x7fff94061165
     A& A::operator=(const A&): 0x7fff94061165
	*/
	B b(a);
	/*2 times:
                A::A(const A&): 0x7ffde70bc005
                    B1::B1(A&): 0x7ffde70bc005
	*/
	B1 b1(a);
	/*object c's base part address == c's address but c's member address != c's address
                        A::A(): 0x7fff94061166
                        A::A(): 0x7fff94061167
     A& A::operator=(const A&): 0x7fff94061167
                      C::C(A&): 0x7fff94061166
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
