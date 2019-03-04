#include <stdio.h>
#define report_func() printf("%30s: %p\n", __PRETTY_FUNCTION__, this)
#define report_pos() printf("%s: %d\n", __func__, __LINE__)
class MyString {
public:
	MyString()
	{
		report_func();
	}
	MyString(const MyString &ref)
	{
		printf("%30s: %p<=%p\n", __PRETTY_FUNCTION__, this, &ref);
	}
	MyString& operator=(const MyString &ref)
	{
		printf("%30s: %p=%p\n", __PRETTY_FUNCTION__, this, &ref);
		return *this;
	}
	MyString(const char *name)// = "")//its default arg must be removed to avoid ambiguous with the 1st default cons
	{
		printf("%30s:%p (\"%s\")\n", __PRETTY_FUNCTION__, this, name);
	}
#if 0
//if no const char* costructor:
string.cpp:27:32: error: could not convert '""' from 'const char [1]' to 'const MyString&'
  Message(const MyString &str = ""): contents(str)
                                ^~
Makefile:14: recipe for target 'string.out' failed
make: *** [string.out] Error 1
#endif
	~MyString()
	{
		report_func();
	}
};

class Message {
public:
	Message(const MyString &str = ""): contents(str)
	{
		report_func();
	}
private:
	MyString contents;
};

class Message2 {
public:
	Message2(const MyString &str = "")
	{
		report_func();
		this->contents = str;
	}
private:
	MyString contents;
};

int main()
{
	/*
	When there is the following 2 constructors at the same time:
	MyString(const char *name = "");
	MyString();
	1. define one MyString object like "MyString str;" will emit build error:
string.cpp: In function 'int main()':
string.cpp:51:11: error: call of overloaded 'MyString()' is ambiguous
  MyString str;
           ^~~
string.cpp:19:2: note: candidate: MyString::MyString(const char*)
  MyString(const char *name = "")
  ^~~~~~~~
string.cpp:5:2: note: candidate: MyString::MyString()
  MyString()
  ^~~~~~~~
Makefile:14: recipe for target 'string.out' failed
make: *** [string.out] Error 1
	2. If no object defination like step1's "MyString str;", no build error will emit even
	these 2 ambiguous constructors have been defined. At the beginning of the test, "MyString str();"
	is just a function declaraion and no any object of myString is defined, so there will be no
	build error!!!
	*/
	//MyString str();//no ouput, for it's a function declaration
	MyString str;
	//Message message();//no output at all for it is a function declaraion.
	report_pos();
	/*2 steps:
	1. one temp MyString object is instantiated from "const char*"
	2. MyString's copy constructor is called
	*/
	Message message;
	report_pos();
	/*3 steps:
	1. one temp MyString object is instantiated from "const char*";
	2. .contents is constructed using default constructor
	3. assignment constructor
	*/
	Message2 message2;
	report_pos();
	//3 steps:1. a default cons; 2. one temp cons; 3. assign cons
	MyString str1;
	str1 = "x";
	report_pos();
	//NOT 2 steps like:1. one temp MyString; 2. copy constructor
	//but only 1 step: cons with const char*
	MyString str2("y");
	report_pos();
	/*
	Cannot be 1 step: cons from const char*;
	but 2 step that is exactly the same as "Message message;"
	*/
	Message message3("");
	report_pos();
	return 0;
}