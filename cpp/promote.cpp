#include <stdio.h>
#define report_func() printf("%s: %p\n", __PRETTY_FUNCTION__, this)
class sp
{
public:
	sp()
	{
		report_func();
	}
	~sp()
	{
		report_func();
	}
	sp(const sp& other):m_var(other.m_var)
	{
		report_func();
	}
	sp& operator=(const sp& other)
	{
		report_func();
		m_var = other.m_var;
		return *this;
	}
	void print_var()
	{
		printf("sp %p:%d\n", this, m_var);
	}
private:
	friend class wp;
	void set_var(int v)
	{
		m_var = v;
	}
	int m_var;
};

class wp
{
public:
	wp(int i)
	{
		report_func();
		m_var = i;
	}
	~wp()
	{
		report_func();
	}
	sp promote() const
	{
		sp result;
		report_func();
		if (m_var)
			result.set_var(m_var);
		return result;
	}
	sp promote2() const
	{
		return sp();
	}
	void print_var()
	{
		printf("wp:%d\n", m_var);
	}
private:
	int m_var;
};

/*
the result of "g++ -Wall promote.cpp", you can add -O0 or --std=c++14 but no any difference:

wp::wp(int): 0x7ffc5b085c58<--@wpA's ctor
sp::sp(): 0x7ffc5b085c5c<-----sp's ctor called in wp::promote, whose this==@spA; no further copy ctor/assignment call for @spA
sp wp::promote() const: 0x7ffc5b085c58
wp:-1
sp 0x7ffc5b085c5c:-1
sp::sp(): 0x7ffc5b085c60<-----"wpA.promote()" that has no assignment's stack var
sp wp::promote() const: 0x7ffc5b085c58
sp::~sp(): 0x7ffc5b085c60<----stack var dtor called when promote() fin;
sp::sp(): 0x7ffc5b085c64<-----@spA2's ctor, no argument
sp wp::promote() const: 0x7ffc5b085c58
sp 0x7ffc5b085c64:-1<-----@spA2's this is the same as the sp object's this in wp::promote
sp::~sp(): 0x7ffc5b085c64<----@spA2's dtor
sp::~sp(): 0x7ffc5b085c5c<----@spA's dtor
wp::~wp(): 0x7ffc5b085c58<----@wpA's dtor

*/
void func0()
{
	wp wpA(-1);
/*
when promote()'s returned stack object is used for spA, the following process doesn't exist:
1. a temp sp object's ctor
2. spA's copy cons called with temp sp object in 1
3. temp sp object in 1 is destructed.
But:
1. sp() ctor is called in promote and it is used as @spA object directly
2. no ~sp() called even promote exits;
*/
	sp spA = wpA.promote();
	wpA.print_var();
	spA.print_var();
	wpA.promote();
	sp spA2(wpA.promote());//equivalent to sp spA2=wpA.promote();
	spA2.print_var();
}
/*
wp::wp(int): 0x7ffc7cb4b440
sp::sp(): 0x7ffc7cb4b444<--promote2() return a temp object, which is equivalent to stack var in promote();
sp 0x7ffc7cb4b444:-1<--the temp object's this==@spA3's this
sp::~sp(): 0x7ffc7cb4b444
wp::~wp(): 0x7ffc7cb4b440
*/
void func1()
{
	wp wpA(9);
	sp spA3 = wpA.promote2();//equivalent to sp spA3(wpA.promote2());
	spA3.print_var();
}

int main()
{
	func0();
	func1();
	return 0;
}
