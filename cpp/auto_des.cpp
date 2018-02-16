#include <stdio.h>
#include <vector>
#include <string>
#include <map>
class MyClass {
public:
	MyClass(const MyClass& src)
	{
		printf("%s(const MyClass& %p)=>%p\n", __func__, &src, this);
	}
	MyClass(int i = 0)
	{
		printf("%s(%d): %p\n", __func__, i, this);
	}
	~MyClass()
	{
		printf("%s: %p\n", __func__, this);
	}
};

class test {
	int m_i;
public:
	test(const test& src)
	{
		m_i = src.m_i;
		printf("%s(const test& %p)=>%p\n", __func__, &src, this);
	}
	test(int i = 0)
	{
		m_i = i;
		printf("%s(%d): %p\n", __func__, i, this);
	}
	~test()
	{
		printf("%s: %p, %d\n", __func__, this, m_i);
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

bool operator<(const MyClass& s1, const MyClass& s2)
{
	return &s1 < &s2;
}

void map_test()
{
	printf("###map element will copy cons its element when inserting??? two stack objs:\n");
	std::map<MyClass, test> *g_map = new std::map<MyClass, test>;
	MyClass index;
	test val(99);
	printf("###insert with make_pair: stack obj=>temp obj(via copy cons)=>map element(via copy cons)\n");
	g_map->insert(std::make_pair(index, val));
	printf("###erase will not des:\n");
	g_map->erase(index);
	printf("###start delete map:\n");
	delete g_map;
	printf("###using []= to add: key: 1 time copy cons; value:default cons to instantiate=>operator=\n");
	printf("###default operator= is used here, for no user defined operator=\n");
	g_map = new std::map<MyClass, test>();//is equal to new T()
	(*g_map)[index] = val;
	printf("###delete map=>all element des, value has been changed via =\n");
	delete g_map;
	printf("###%s: auto des\n", __func__);
}

class value {
	int m_i;
public:
	value(const value& src)
	{
		m_i = src.m_i;
		printf("%s(const value& %p)=>%p\n", __func__, &src, this);
	}
	value(int i = 0)
	{
		m_i = i;
		printf("%s(%d): %p\n", __func__, i, this);
	}
	~value()
	{
		printf("%s: %p, %d\n", __func__, this, m_i);
	}
	void operator=(const value& src)
	{
		printf("%s: %p, %d=>%d\n", __func__, this, m_i, src.m_i);
		m_i = src.m_i;
	}
};
using MyMap = std::map<MyClass, value>;
void map_test2()
{
	printf("###2 stack obj cons\n");
	/*
	TODO:MyClass c() will be error, don't know why! Because it is a function named c and return MyClass
	while the following are right:
	new T();
	new T;
	both call T non-argument des
	*/
	MyClass c;
	value v(1);
	printf("###[]= will call operator =\n");
	MyMap *p = new MyMap;
	//(*p)[c];
	(*p)[c] = v;
	printf("###delete map instance\n");
	delete p;
	printf("###2 stack obj des\n");
}
int main()
{
	printf("member is class:\n");
	Parser parser;
	printf("public inherit:\n");
	MyClass2 obj;
	printf("public inherit+class member:base cons then mem cons then whole class cons\n");
	MyClass3 obj2;
	map_test();
	map_test2();
	printf("before return:\n");
	return 0;
}