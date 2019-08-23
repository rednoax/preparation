/*
A emulated android LightRefBase test.
LightRefBase is a class template, whose typename must be the child class of
LightRefBase, but cannot be LightRefBase.
The test is mainly to check if the typename can be any level's derived class
of LightRefBase. It's OK to do so!
*/
#include <stdio.h>
#define report_func() printf("%30s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line() printf("\n%s: %d\n\n", __func__, __LINE__)

template <typename T>
class LightRefBase
{
public:
	LightRefBase(): mCount(0)
	{
		report_func();
	}
	void incStrong() const
	{
		report_func();
		mCount++;
	}
/*
if no mutable for mCount, build error:
LightRefBase.cpp:26:13: error: decrement of member 'LightRefBase<LightClass2>::mCount' in read-only object
   if (mCount-- == 1)
       ~~~~~~^~
*/
	void decStrong() const
	{
		report_func();
		if (mCount-- == 1)
			delete (T*)this;//the cast needs that "typename must be the child class of LightRefBase"
	}
protected://so typename must not be LightRefBase, LightRefBase<LightRefBase> var; var cannot be destructed, build error then!
	~LightRefBase()
	{
		report_func();
	}
private:
	mutable unsigned int mCount;
};

class LightClass2;//cannot be ignored, build error "error: 'LightClass2' was not declared in this scope" otherwise!
/*Here the LighClass2 rather than LightClass is used as the templeate parameter, it is ok.
The reason:
1. LightRefBase's T is only used as ptr
2. When T* or T& is used, only preclaim of T is needed. No need to define it before:
class LightClass: public LightRefBase<LightClass2>
The LightClass2 can be defined later. 
Besides, it cannot be defined before LightClass due to the derivation relation.
*/
class LightClass: public LightRefBase<LightClass2>
{
public:
	LightClass()
	{
		report_func();
	}
	~LightClass()
	{
		report_func();
	}
};

class LightClass2: public LightClass
{
public:
	LightClass2()
	{
		report_func();
	}
	~LightClass2()
	{
		report_func();
	}
};

/*
1. all this are the same
2. the dtors are triggered by delete derived class ptr
3. func0 will not touch LightRefBase's count, and the dtors is called explicitely by delete
ptr of final derived class. Plz note there is no any virtual in dtor of 3 classes. The final
derived class's ptr rather than casted base/middle class's ptr should be used.
LightRefBase<T>::LightRefBase() [with T = LightClass2]:0x55e9f006f280
      LightClass::LightClass():0x55e9f006f280
    LightClass2::LightClass2():0x55e9f006f280
   LightClass2::~LightClass2():0x55e9f006f280
     LightClass::~LightClass():0x55e9f006f280
LightRefBase<T>::~LightRefBase() [with T = LightClass2]:0x55e9f006f280
*/
void func0()
{
	report_line();
	LightClass2 *pLc2 = new LightClass2();
	delete pLc2;//it must be explicitely called. In case you forget it, use func1's inc/dec way
}

/*
1. all this are the same
2. the dtors are triggered by call member function decStrong of the base class LightRefBase
LightRefBase<T>::LightRefBase() [with T = LightClass2]:0x556c3eecc280
      LightClass::LightClass():0x556c3eecc280
    LightClass2::LightClass2():0x556c3eecc280
void LightRefBase<T>::incStrong() [with T = LightClass2]:0x556c3eecc280
void LightRefBase<T>::decStrong() [with T = LightClass2]:0x556c3eecc280
   LightClass2::~LightClass2():0x556c3eecc280
     LightClass::~LightClass():0x556c3eecc280
LightRefBase<T>::~LightRefBase() [with T = LightClass2]:0x556c3eecc280
*/
void func1()
{
	report_line();
	LightClass2 *pLc2 = new LightClass2();
	pLc2->incStrong();//put in another class's ctor so that it can be called by compiler's stub
	pLc2->decStrong();//use some other class's dtor to wrap it and instantiate stack var of it, so that it can be called by compiler's stub
}

int main()
{
	func0();
	func1();
	return 0;
}
