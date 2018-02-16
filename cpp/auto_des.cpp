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
int main()
{
	printf("member is class:\n");
	Parser parser;
	printf("public inherit:\n");
	MyClass2 obj;
	printf("before return:\n");
	return 0;
}