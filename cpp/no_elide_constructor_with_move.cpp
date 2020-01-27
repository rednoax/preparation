#include <stdio.h>
#define report_func() printf("%s: %p\n", __PRETTY_FUNCTION__, this)
#define report_line() printf("%s: %d\n", __PRETTY_FUNCTION__, __LINE__)
class Base
{
public:
	Base()
	{
		report_func();
	}
	~Base()
	{
		report_func();
	}
	Base(const Base &o)
	{
		report_func();
		printf("from %p\n", &o);
	}
	Base(Base&& o)
	{
		report_func();
	}
	Base& operator=(Base&& o)
	{
		report_func();
		return *this;
	}
	Base& operator=(Base *o)
	{
		report_func();
		printf("from %p\n", o);
		return *this;
	}
};

Base getTemporyBase() {
	Base tmp;
	return tmp;//when return a non reference object, a temp obj will be created
}
/*
g++ -Wall no_elide_constructor_with_move.cpp -fno-elide-constructors
Base::Base(): 0x7ffe761a9987<--Base tmp's ctor
Base::Base(Base&&): 0x7ffe761a99b7<--uses move ctor to cons the returned temp obj
Base::~Base(): 0x7ffe761a9987<--@tmp's dtor
Base::Base(Base&&): 0x7ffe761a99b6<--Base a's cons uses move ctor
Base::~Base(): 0x7ffe761a99b7<--the returned temp obj is dtor-ed
test fin
Base::~Base(): 0x7ffe761a99b6<--@a's dtor

g++ -Wall no_elide_constructor_with_move.cpp
Base::Base(): 0x7fff7fdee3c7<--stack temp object is used as @a directly
test fin
Base::~Base(): 0x7fff7fdee3c7<--a's dtor
*/
void test() {
	Base a = getTemporyBase();
	printf("test fin\n");
}
/*
-fno-elide-constructors
 The C++ standard allows an implementation to omit creating a temporary that is only used to initialize another object of the same type.
 Specifying this option disables that optimization, and forces G++ to call the copy constructor in all cases.  This option also causes G++
 to call trivial member functions which otherwise would be expanded inline.

 In C++17, the compiler is required to omit these temporaries, but this option still affects trivial member functions.
*/

void _test(const Base &o)
{
	report_line();
	printf("from %p\n", &o);
}

void test2()
{
	printf("test2 start\n");
	_test(getTemporyBase());
	printf("test2 end\n");
}

Base getTemporyBase2() {
	Base tmp;
	tmp = new Base();
	return tmp;//when return a non reference object, a temp obj will be created
}

void test3()
{
	printf("%s start\n", __func__);
	_test(getTemporyBase2());
	printf("%s end\n", __func__);
}

int main()
{
	test();
	test2();
	test3();
	return 0;
}

