#include <stdio.h>
#define report_func() printf("%s: %p\n", __func__, this)
#define report_line() printf("%s: %d\n", __func__, __LINE__)
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
};

Base getTemporyBase() {
	Base tmp;
	return tmp;//when return a non reference object, a temp obj will be created
}
/*
g++ -Wall no_elide_constructor.cpp -fno-elide-constructors
Base: 0x7ffca3eaa027<--Base tmp calls ctor
Base: 0x7ffca3eaa057<--return tmp will call copy ctor to construct a temp obj
from 0x7ffca3eaa027<--the copt ctor uses @tmp as source
~Base: 0x7ffca3eaa027<--@tmp's dtor
Base: 0x7ffca3eaa056<--Base a's copy ctor
from 0x7ffca3eaa057<--a's copy ctor uses the returned temp obj as souces
~Base: 0x7ffca3eaa057<--returned temp obj is dtor-ed
test fin
~Base: 0x7ffca3eaa056<--a's dtor

g++ -Wall no_elide_constructor.cpp
Base: 0x7fffeb6a89c7<--the stack temp obj is used as returned @a directly,Base() is called
test fin
~Base: 0x7fffeb6a89c7<--the stack temp obj's dtor
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
int main()
{
	test();
	return 0;
}

