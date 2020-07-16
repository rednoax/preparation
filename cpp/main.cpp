#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class base {
	int a;
	void f()
	{
		printf("%d\n", a);
	}
public:
	int b;
	void g()
	{
		printf("%d\n", a);
	}
	base()
	{
		a = b = c = 0;
		printf("%5s:%p, %d\n", __func__, this, __LINE__);
	}
	base(int v):a(v), b(0), c(0)
	{
		printf("%5s:%p, %d\n", __func__, this, __LINE__);		
	}
	base(const base &b)
	{
		printf("%5s:%p=>%p\n", __func__, &b, this);		
	}
	~base()
	{
		printf("%s:%p\n", __func__, this);
	}
protected:
	int c;
	void k()
	{
		printf("%d\n", a);
	}
};
class derived:public base {
public:
	void df()
	{
		//printf("%d", a);		//wrong!
		printf("%d", b);
		printf("%d", c);
		//f();					//wrong!
		g();
		k();
	}
	derived():base(1)
	{
		printf("%5s:%p\n", __func__, this);
	}
	~derived()
	{
		printf("%s:%p\n", __func__, this);
	}
};

void func()
{
	base b;
	//printf("%d", b.a);			//wrong!
	printf("%d", b.b);
	//printf("%d", b.c);			//wrong!
	//b.f();							//wrong!
	b.g();
	//b.k();							//wrong!
	derived d;
	//printf("%d", d.a);		//wrong!
	printf("%d", d.b);		//wrong!
	//printf("%d", d.c);		//wrong!
	//d.f();//wrong
	d.g();
	//d.k();//wrong
	char property[32] = {0};
	char *endptr;
	unsigned long value;
	/*
	kernel:	long simple_strtol(const char *cp, char **endp, unsigned int base)
	c:		long int strtol(const char *nptr, char **endptr, int base);
	*/
	value = strtoul(property, &endptr, 10);
	printf("%ld, %ld\n", value, endptr - property);
	strcpy(property, "192m");
	value = strtoul(property, &endptr, 10);
	printf("%ld, %ld\n", value, endptr - property);
	strcpy(property, "0777");
	value = strtoul(property, &endptr, 8);
	printf("%lo, %ld, endptr[0] is %d\n", value, endptr - property, endptr[0]);
	strcpy(property, "+0x10");
	value = strtoul(property, &endptr, 0);
	printf("%lx, %ld, endptr[0] is %d\n", value, endptr - property, endptr[0]);
	strcpy(property, "a");
	value = strtoul(property, &endptr, 0);
	printf("%lx, %ld, endptr[0] is '%c'\n", value, endptr - property, endptr[0]);
	value = strtoul(property, &endptr, 16);
	printf("%lx, %ld, endptr[0] is %d\n", value, endptr - property, endptr[0]);
	strcpy(property, "0");
	value = strtoul(property, &endptr, 0);
	printf("%lx, %ld, endptr[0] is %d\n", value, endptr - property, endptr[0]);
	//
	strcpy(property, "57600n8");
	value = strtoul(property, &endptr, 10);
	printf("%ld, [%ld], endptr[0] is '%c'\n", value, endptr - property, endptr[0]);
	//
	printf("test end\n");
}

template<typename T>
class node {
	T m;
	derived d;
	node *next;
public:
	node():next(0)
	{
		printf("%5s:%p\n", __func__, this);
	}
	~node()
	{
		printf("%s:%p\n", __func__, this);
	}
};
template<class T>
class node2 {
	T pT;
public:
	node2()
	{
		printf("%5s:%p\n", __func__, this);
	}
	~node2()
	{
		printf("%5s:%p\n", __func__, this);
	}
};

template<class T>
class node3 {
	T *pT;
public:
	node3()
	{
		pT = NULL;
		printf("%5s:%p\n", __func__, this);
	}
	void add(const T &t)//ref is const ptr actually.the following &t will expose the ptr value 
	{
		pT = new T(t);//call T's copy constructor
		printf("%s:%p=>%p\n", __func__, &t, pT);
	}
	~node3()
	{
		if (pT)
			delete pT;
		printf("%5s:%p, %p\n", __func__, this, pT);		
	}
};
#if 0
node3_test:130
node3:0x7fffa3aa9410
 base:0x22e8070, 21
add:0x7fffa3aa9430=>0x22e8050
~base:0x22e8070
node3_test:0x7fffa3aa9430
node3:0x7fffa3aa9420
 base:0x22e8070, 21
 base:0x22e8070=>0x22e7c20
add:0x22e8070=>0x22e7c20
~base:0x22e8070
node3_test:0x22e8070, 140
~base:0x22e7c20
~node3:0x7fffa3aa9420, 0x22e7c20
~node3:0x7fffa3aa9410, 0x22e8050
#endif
void node3_test(void)
{
	printf("%s:%d\n", __func__, __LINE__);
	node3<base*> n1;
	base *p = new base(4);
	n1.add(p);
	delete p;
	printf("%s:%p\n", __func__, &p);
	node3<base> n2;
	p = new base(3);
	n2.add(*p);
	delete p;	
	printf("%s:%p, %d\n", __func__, p, __LINE__);
}

template<typename T>
class list {
	node<T> *first;
public:
	list():first(0)
	{
		printf("%5s:%p\n", __func__, this);
	}
	~list()
	{
		printf("%s:%p\n", __func__, this);
	}
};

//from system/core/logd/main.cpp
bool property_get_bool(const char *key, int flag)
{
	printf("%s:%d\n", __func__, __LINE__);
	return true;
}

#if 0
/*
main.cpp:180:54: error: ambiguating new declaration of ‘long long int property_get_bool(const char*, int)’
 long long property_get_bool(const char *key, int flag)
                                                      ^
main.cpp:174:6: note: old declaration ‘bool property_get_bool(const char*, int)’
 bool property_get_bool(const char *key, int flag)
      ^
Makefile:5: recipe for target 'all' failed
*/
long long property_get_bool(const char *key, int flag)
{
	printf("%s:%d\n", __func__, __LINE__);
	return -1;
}
#else
#endif
//from system/core/libcutils/properties.c
char property_get_bool(const char *key, char flag)
{
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}


void func_reload_test()
{
	bool ret = property_get_bool("dummy", 0);
	char c = property_get_bool("dummy", (char)0);
	c = property_get_bool("dummy", 0);
	int i = property_get_bool("stange", 0);
	printf("%s:%d %d %d\n", __func__, ret, c, i);
}
int main()
{
	base *pb1 = new base;
	base *pb2 = new base();
	base *pb3 = new base(9);
	delete pb1;
	delete pb2;
	delete pb3;
	base **pb4 = new base*;
	delete pb4;
	base pb5;
	printf("now there is function declaration\n");
	base pb6();
	printf("template test begins:\n");
	node<base*> n1;
	printf("template test 2 begins:\n");
	node<base> n2;
	printf("template test 3 begins:\n");
	node2<base> n3;
	printf("template test 4 begins:\n");
	node2<base*> n4;
	node3_test();
	func_reload_test();
	func();
	return 0;
}