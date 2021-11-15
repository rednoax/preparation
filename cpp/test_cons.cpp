#include <iostream>

class A
{
public:
	int a;
	int *p;
#if 0/*if enabled, no default ctor but there is implicit copy/mov ctor still
$ clang++ -Wall test_cons.cpp
test_cons.cpp:19:3: error: no matching constructor for initialization of 'A'
A ma;
  ^
test_cons.cpp:9:2: note: candidate constructor not viable: requires single argument 'i', but no arguments were provided
        A(int i)
        ^
test_cons.cpp:3:7: note: candidate constructor (the implicit copy constructor) not viable: requires 1 argument, but 0 were provided
class A
      ^
test_cons.cpp:3:7: note: candidate constructor (the implicit move constructor) not viable: requires 1 argument, but 0 were provided
1 error generated.
*/
	A(int i)
	{
		a = i;
	}
#endif
	void print()
	{
		printf("%x %p\n", a, p);
	}
};
A ma;

class base
{
public:
		base(int i)
		{
			printf("%s(%d)\n", __func__, i);
		}
};

class derived:public base
{
public:
#if 0
	derived()
	{
		printf("%s()\n", __func__);
	}
/*
test_cons.cpp:46:2: error: constructor for 'derived' must explicitly initialize the base class 'base' which does not have a default constructor
        derived()
        ^
test_cons.cpp:34:7: note: 'base' declared here
class base
      ^
1 error generated.
*/
#endif
	derived():base(1)
	{
		printf("%s()\n", __func__);
	}
};
derived d;
int main()
{
	int i;
	char c;
	ma.print();//0 (nil)
	std::cin >> i >> c;//'\t\t 304\t\t 20'=>304 50
	printf("%d %d\n", i, c);
	return 0;
}
