//#include <string>
#include <stdio.h>
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
class string
{
public:
	string(const char* name)
	{
		printf("%s(\"%s\"): %p\n", __func__, name, this);
		//report_func();
	}
	string(const string &o)
	{
		printf("%s from %p, %p\n", __PRETTY_FUNCTION__, &o, this);
	}
private:
};

class Sales_data
{
public:
	Sales_data()// = default;
	{
		printf("%s(): %p\n", __func__, this);
	}
#if 0
explicit.cpp: In function 'void test()':
explicit.cpp:62:24: error: no matching function for call to 'Sales_data::combine(string&)'
  item.combine(null_book);
                        ^
explicit.cpp:31:22: note: candidate: void Sales_data::combine(const Sales_data&)
  void/*Sales_data &*/combine(const Sales_data & from)//const is must
                      ^~~~~~~
explicit.cpp:31:22: note:   no known conversion for argument 1 from 'string' to 'const Sales_data&'
#endif
	/*explicit*/ Sales_data(string &s)//there is no const here so the string it refed to:1.can be used as a lvalue;2.should be a lvalue
	{
		printf("%s(%p): %p\n", __PRETTY_FUNCTION__, &s, this);
		//report_func();
	}
	void/*Sales_data &*/combine(const Sales_data & from)//const is must
	{
		printf("%s(%p): %p\n", __func__, &from, this);
	}
private:
	//string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};
/*
string("9-999-99999-9"): 0x7ffd55219acf
Sales_data(): 0x7ffd55219ad0
Sales_data::Sales_data(string&)(0x7ffd55219acf): 0x7ffd55219ae0<--a temp Sales_data generated from @null_book by Sales_data(string&), the string referenced by string& should be(can be used as) a lvalue, here null_book is a lvalue so it is ok
combine(0x7ffd55219ae0): 0x7ffd55219ad0<--combine's argument is a temp Scales_data object, which is rvalue and can not be lvalue, so combine's argument must be "const ..."
*/
void test()
{
	string null_book = "9-999-99999-9";
	Sales_data item;
#if 0//if removing const limit on combine's argument,build error!
explicit.cpp: In function 'void test()':
explicit.cpp:50:24: error: cannot bind non-const lvalue reference of type 'Sales_data&' to an rvalue of type 'Sales_data'
  item.combine(null_book);
                        ^
explicit.cpp:26:2: note:   after user-defined conversion: Sales_data::Sales_data(string&)
  Sales_data(string &s)//there is no const here so the string it refed to:1.can be used as a lvalue;2.should be a lvalue
  ^~~~~~~~~~
explicit.cpp:31:22: note:   initializing argument 1 of 'void Sales_data::combine(Sales_data&)'
  void/*Sales_data &*/combine(Sales_data & from)
                      ^~~~~~~
#endif
	item.combine(null_book);
}

int main()
{
	test();
	return 0;
}
