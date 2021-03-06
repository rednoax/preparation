#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <string.h>
using namespace std;
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
	test(const char *s)
	{
		m_i = s[0];
		printf("%s(\"%s\"): %p\n", __func__, s, this);
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
	printf("value @%p\n", &(*p)[c]);
	printf("###delete map instance\n");
	delete p;
	printf("###2 stack obj des\n");
}

void map_test3()
{
	std::map<int, string> m;
	m[0] = "0";
	printf("%s: %ld, '%s'\n", __func__, m.count(0), m[0].c_str());
	m[0] = "1";
	printf("%s: %ld, '%s', '%p'\n", __func__, m.count(0), m[0].c_str(), &m[0]);
}

void InitSingleTrigger(const test& trigger)//temp obj cons
{
	printf("###%s: trigger @%p\n", __func__, &trigger);
	vector<test> name_vector{trigger};//copy cons twice from input's temp obj
	printf("###%s: fin, name_vector [0]@%p\n", __func__, &name_vector[0]);
}

void InitSingleTrigger2(const test trigger)//obj instantiate with a temp obj cons will not call copy cons!!!
{
	printf("###%s: trigger @%p\n", __func__, &trigger);
	vector<test> name_vector{trigger};//copy cons twice from input's temp obj
	printf("###%s: fin, name_vector's [0]@%p\n", __func__, &name_vector[0]);
}

void string_test()
{
	string s1 = __func__;
	string s2 = s1;
	if (!s2.compare(0, s1.length(), s1)) {
		printf("%s: s1.length() %ld==strlen %ld\n", __func__, s1.length(), strlen(s1.data()));
	}
}

void vector_test()
{
	printf("###%s\n", __func__);
	using vt = vector<test>;
	vector<test> *v0 = new vt;
	v0->emplace_back(9);
	printf("###start vector assign using element copy cons!\n");
	vector<test> *v1 = new vt;
	*v1 = *v0;
	printf("###%s: delete v0\n", __func__);
	delete(v0);
	printf("###%s: clear v1 will delete all its elements, formmer size %ld\n", __func__, v1->size());
	//delete(v1);
	v1->clear();
	printf("after clear: size %ld, empty %d\n", v1->size(), v1->empty());
	printf("###vector<T> var(T&), T& is via input arg: temp obj cons=>copy cons twice from temp obj\n");
	InitSingleTrigger("onrestart");//temp obj cons
	printf("###%s fin1\n", __func__);
	printf("###vector<T> var(T), T is via input arg: temp obj cons=>copy cons twice from temp obj\n");
	InitSingleTrigger2("onrestart");//temp obj cons
	printf("###%s fin\n", __func__);
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
	map_test3();
	vector_test();
	string_test();
	printf("before return:\n");
	return 0;
}