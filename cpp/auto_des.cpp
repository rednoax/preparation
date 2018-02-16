#include <stdio.h>
#include <vector>
#include <string>
class MyClass {
public:
	MyClass(const MyClass& src)
	{
		printf("%s(const MyClass& %p): %p<=%p\n", __func__, &src, this, &src);
	}
	MyClass()
	{
		printf("%s(): %p\n", __func__, this);
	}
	~MyClass()
	{
		printf("%s: %p\n", __func__, this);
	}
};

class test {
public:
	test(const test& src)
	{
		printf("%s(const test& %p): %p<=%p\n", __func__, &src, this, &src);
	}
	test()
	{
		printf("%s(): %p\n", __func__, this);
	}
	~test()
	{
		printf("%s: %p\n", __func__, this);
	}
};

class Parser {
public:
	~Parser()
	{
		printf("%s: %p\n", __func__, this);
	}
	Parser()
	{
		printf("%s(): %p\n", __func__, this);		
	}
private:	
	MyClass m_obj;
};

class MyClass2: public MyClass {
public:
	MyClass2()
	{
		printf("%s(): %p\n", __func__, this);
	}
	~MyClass2()
	{
		printf("%s: %p\n", __func__, this);
	}	
};

class MyClass3: public MyClass {
	test m_obj;
public:
	MyClass3()
	{
		printf("%s(): %p\n", __func__, this);
	}
	~MyClass3()
	{
		printf("%s: %p\n", __func__, this);
	}
};

int main()
{
	printf("member is class:\n");
	Parser parser;
	printf("public inherit:\n");
	MyClass2 obj;
	printf("public inherit+class member:base cons then mem cons then whole class cons\n");
	MyClass3 obj2;
	printf("before return:\n");
	return 0;
}