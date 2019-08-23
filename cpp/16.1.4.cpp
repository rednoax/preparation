#include <stdio.h>
#define REPORT_FUNC() printf("%30s:%p\n", __PRETTY_FUNCTION__, this)
#define REPORT_LINE() printf("%s:%d\n\n", __PRETTY_FUNCTION__, __LINE__)
class A {
public:
};

class B {
public:
};

template <typename T> class Blob {
public:
	template <typename U> Blob(U* other)
	{
		REPORT_FUNC();
	}
#if 10
/*
1. when enabled:
Blob<T>::Blob(T*) [with T = int]:0x7ffcee33ccc2
Blob<T>::Blob(U*) [with U = char; T = int]:0x7ffcee33ccc3
---
Blob<T>::Blob(T*) [with T = A]:0x7ffcee33ccd6
Blob<T>::Blob(U*) [with U = B; T = A]:0x7ffcee33ccd7

2. when disabled:
Blob<T>::Blob(U*) [with U = int; T = int]:0x7ffd7fc45602
Blob<T>::Blob(U*) [with U = char; T = int]:0x7ffd7fc45603
---
Blob<T>::Blob(U*) [with U = A; T = A]:0x7fff76875906
Blob<T>::Blob(U*) [with U = B; T = A]:0x7fff76875907
*/
	Blob(T* other)
	{
		REPORT_FUNC();
	}
#endif
};

void func0()
{
	REPORT_LINE();
	int i = 10;
	int *j = &i;
	char str[] = "";
	Blob<int> b0(j);
	Blob<int> b1(str);
}

void func1()
{
	REPORT_LINE();
	A a;
	B b;
	Blob<A> b0(&a);
	Blob<A> b1(&b);
}

int main()
{
	func0();
	func1();
	return 0;
}

