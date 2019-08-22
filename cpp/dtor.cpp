#include <stdio.h>
#define report_func() printf("%30s: %p\n", __PRETTY_FUNCTION__, this)

inline void report_line()
{
	printf("%s: %d\n\n", __func__, __LINE__);
}

class A
{
public:
	A()
	{
		report_func();
	}
	virtual ~A()
	{
		report_func();
	}
};

class B: public A
{
public:
	A a;
	B()
	{
		report_func();
	}
	~B()
	{
		report_func();
	}
};

void func0()
{
/*
when no virtual for ~A, no matter if instance B is created with new or on the stack, B's dtors sequence is right(mirror with B's ctors)
 A::A(): 0x7ffcccbf85d6
 A::A(): 0x7ffcccbf85d7
 B::B(): 0x7ffcccbf85d6
B::~B(): 0x7ffcccbf85d6
A::~A(): 0x7ffcccbf85d7
A::~A(): 0x7ffcccbf85d6

when there is virtual for ~A, NO change compared with the above:
		 A::A(): 0x7ffd93002600
		 A::A(): 0x7ffd93002608
		 B::B(): 0x7ffd93002600
virtual B::~B(): 0x7ffd93002600
virtual A::~A(): 0x7ffd93002608
virtual A::~A(): 0x7ffd93002600

*/
	report_line();
	B b;
}

void func1()
{
/*
when no virtual for ~A, the dtors of the derived can work for derived class ptr has not been cast to base ptr.
 A::A(): 0x5633aba14280
 A::A(): 0x5633aba14281
 B::B(): 0x5633aba14280
B::~B(): 0x5633aba14280
A::~A(): 0x5633aba14281
A::~A(): 0x5633aba14280

when there is virtual for ~A, NO change compared with the above:
		 A::A(): 0x555c4d205280
		 A::A(): 0x555c4d205288
		 B::B(): 0x555c4d205280
virtual B::~B(): 0x555c4d205280
virtual A::~A(): 0x555c4d205288
virtual A::~A(): 0x555c4d205280
*/
	report_line();
	B *pB = new B();
	delete pB;
}

void func2()
{
/*
when no virtual for ~A, the dtor sequence is wrong for the casted base ptr from derived ptr is used!!!
 A::A(): 0x5633aba14280
 A::A(): 0x5633aba14281
 B::B(): 0x5633aba14280
A::~A(): 0x5633aba14280

when there is virtual for ~A, the destruction performs right, not like the above wrong sequence:
		 A::A(): 0x555c4d205280
		 A::A(): 0x555c4d205288
		 B::B(): 0x555c4d205280
virtual B::~B(): 0x555c4d205280//the following 3 dtors is mirrored with ctors, the sequence is right!
virtual A::~A(): 0x555c4d205288
virtual A::~A(): 0x555c4d205280
*/
	report_line();
	A *pA = new B();
	delete pA;
}

class LightRefBase
{
public:
	LightRefBase()
	{
		report_func();
	}
/*
https://stackoverflow.com/questions/8970466/is-there-a-use-for-making-a-protected-destructor-virtual
If the destructor is protected, then calling code cannot destroy through a base class pointer and
the destructor does not need to be virtual; it does need to be protected, not private, so that
derived destructors can invoke it.
*/
protected:
	~LightRefBase()//protected/private dtor make LightRefBase cannot be instantiated directly. LightRefBase can only be derived!
	{
		report_func();
	}
};

class LightClass: LightRefBase
{
public:
	LightClass()
	{
		report_func();
	}
	~LightClass()//derived class dtor will call base's dtor, which can be protected or private!
	{
		report_func();
	}
};

void func3()
{
	report_line();
/*
	LightRefBase lrf;//g++ will emit error as following:
	dtor.cpp: In function 'void func3()':
	dtor.cpp:122:15: error: 'LightRefBase::~LightRefBase()' is protected within this context
	  LightRefBase lrf;
				   ^~~
	dtor.cpp:114:2: note: declared protected here
	  ~LightRefBase()
	  ^
*/
	/*
	 LightRefBase::LightRefBase(): 0x562d4bb51280
		 LightClass::LightClass(): 0x562d4bb51280
		LightClass::~LightClass(): 0x562d4bb51280
	LightRefBase::~LightRefBase(): 0x562d4bb51280<--derived class's dtor will call its base's dtor implicitely
	*/
	LightClass *lc = new LightClass();
	delete lc;
}

int main()
{
	func0();
	func1();
	func2();
	func3();
	return 0;
}
