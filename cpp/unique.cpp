#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#if 10
#define USE_STD_UNIQUE
#else
# if 0
#define USE_UNIQUE_UNSTANDARD
# endif
#endif

#ifdef USE_STD_UNIQUE
using std::unique_ptr;
#else
template<typename T>
class unique_ptr {
private:
	T *m;
	unique_ptr& operator=(const unique_ptr& src)
	{
		m = src.m;
		printf("%s: %p<=%p(%p)\n", __func__, this, &src, m);
		return *this;
	}
	unique_ptr& operator=(const T *src)
	{
		m = src;
		printf("%s:%p <==%p\n", __func__, this, src);
		return *this;
	}
# if !defined(USE_UNIQUE_UNSTANDARD)
public://build error if copy cons is private
# endif
	unique_ptr(const unique_ptr& src)
	{
		m = src.m;
		printf("%s(const T&): %p(%p)==>%p\n", __func__, &src, m, this);
	}
public:
	unique_ptr& operator=(int i)
	{
		//m = NULL;
		printf("%s:%p <==%d\n", __func__, this, i);
		return *this;
	}	
	unique_ptr(T *p)
	{
		m = p;
		printf("%s(T* %p): %p(%p)\n", __func__, p, this, m);
	}
	T* operator->() const
	{
		return this->m;
	}
	T& operator*() const
	{
		return this->m[0];//->.[]are at the same level, from left to right
	}
	~unique_ptr()
	{
		if (m)
			delete m;
		printf("%s: %p(%p), fin delete %p\n", __func__, this, m, m);
	}
//friend unique_ptr<T> make_unique(Args... args);
};
#endif

#if __cplusplus > 200400L && __cplusplus < 201200L
# ifdef USE_UNIQUE_UNSTANDARD
template<typename T, typename... Args>
T* make_unique(Args... args)
{
	return new T(args...);
}
# else
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args... args)
{
	printf("start new T: ");
	return unique_ptr<T>(new T(args...));
}
# endif
#endif

class MyClass {
private:
	std::string m_Name;
	int m_value;
public:
	MyClass(const MyClass& src)
	{
		m_Name = src.m_Name;
		m_value = src.m_value;
		printf("%s(const MyClass& %p): %p<=%p\n", __func__, &src, this, &src);
	}
	MyClass()
	{
		printf("%s(): %p\n", __func__, this);
	}
	MyClass(const std::string& name, int value)
		:m_Name(name)
		,m_value(value)
	{
		printf("%s(const string& \"%s\", %d): %p\n", __func__, name.data(), value, this);
	}
	~MyClass()
	{
		printf("%s:\"%s\" %p\n", __func__, m_Name.c_str(), this);
	}
	const std::string& GetName() const
	{
		return m_Name;
	}
	int GetValue() const
	{
		return m_value;
	}
	MyClass& operator=(const MyClass &src)
	{
		m_Name = src.m_Name;
		m_value = src.m_value;
		printf("%s: %p<=%p\n", __func__, this, &src);
		return *this;
	}
	/*
	bool operator()(const MyClass& right)
	{
		const char* p1 = this->GetName().c_str();
		const char* p2 = right.GetName().c_str();
		printf("%s < %s\n", p1, p2);
		return strcmp(p1, p2);
	}
	*/
};

unique_ptr<MyClass> PassUnquePtr(unique_ptr<MyClass> obj)
{
	printf("%s: stack temp obj %p(%p)\n", __func__, &obj, &*obj);
	return obj;
}

void copy_elision_test()
{
	printf("###stack temp obj start:\n");
	MyClass("test0", 0);
	printf("###stack temp obj des fin\n");
	printf("###these 2 equivalents are NOT temp obj copy cons!!!!!!!!\n");
	/*
	T obj(T(args...)) will not call copy cons:
	FLASE: temp obj with T cons=>copy cons from temp obj to obj	<---------actually not so!!!!!!!!!!
	true:obj is instantiated with T cons directly.				<---------actually so!!!!!!!!!!!!!!
	*/
	MyClass o0(MyClass("test1", 1));//temp obj call related cons, then no copy cons
	MyClass o1 = MyClass("test2", 2);//exactly equal to the above
	printf("o0/1 @ %p/%p\n", &o0, &o1);
	printf("###these 2 equivalents really call copy cons\n");
	printf("push_back(temp obj) will call cons twice: first temp obj cons then copy cons, which "\
			"is not like the following \"obj instantiate using copy cons\"\n");
	/*
	only a non temp obj in cons will call copy-cons!!!
	*/
	MyClass o2 = o0;
	MyClass o3(o0);//absolutely equal to the above
	printf("###stack obj from \"temp obj\" fin\n");
}

int main(int argc, char **argv)
{
	//template unique_ptr<MyClass>;
	//printf("%d\n", (o0, o1));
	//
	printf("###these 4 quivalent instantiation, no copy cons during obj instantiation\n");
	unique_ptr<MyClass> uniquePointer0;
	printf("default cons point to %p\n", *uniquePointer0);
	unique_ptr<MyClass> uniquePointer1(nullptr);
	printf("uniquePointer1 %p, point to %p\n", &uniquePointer1, *uniquePointer1);
	unique_ptr<MyClass> uniquePointer2(new MyClass("MyClass0", 10));
	printf("MyClass is @%p via .get()\n", uniquePointer2.get());
	printf("__cplusplus: %ld\n", __cplusplus);//201103
	unique_ptr<MyClass> uniquePointer3(make_unique<MyClass>("MyClass1", 10));
	printf("%s: %d\n", uniquePointer3->GetName().c_str(), uniquePointer3->GetValue());
	unique_ptr<MyClass> uniquePointer4 = make_unique<MyClass>("MyClass2", 20);//exactly the same as before
#if !defined(USE_STD_UNIQUE)
	uniquePointer4 = 3;//make_unique<MyClass>("MyClass2", 20);
#endif
	unique_ptr<MyClass> uniquePointer5(unique_ptr<MyClass>(new MyClass("MyClass3", 30)));//the 3rd quivalent
	printf("uniquePointer2/3/4/5 @%p/%p/%p/%p\n", &uniquePointer2, &uniquePointer3, &uniquePointer4, &uniquePointer5);
#if 0
unique.cpp:88:50: error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = MyClass; _Dp = std::default_delete<MyClass>]’
  unique_ptr<MyClass> uniquePointer2(uniquePointer);
                                                  ^
unique.cpp:110:39: error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = MyClass; _Dp = std::default_delete<MyClass>]’
  unique_ptr<MyClass> uniquePointer3 = uniquePointer;
                                       ^
unique.cpp:120:39: error: invalid conversion from ‘int’ to ‘MyClass*’ [-fpermissive]
  unique_ptr<MyClass> uniquePointer5 = 3;//make_unique<MyClass>("MyClass2", 20);
                                       ^                                        
#endif
	printf("###move test, unique_ptr<T> 's copy cons(NOT deep copy) is called??\n");
	/*
	unique_ptr<T> move(unique_ptr<T>);
	*/
	unique_ptr<MyClass> uniquePointer6 = move(uniquePointer5);//equal to uniquePointer6(move(...))
	if (uniquePointer5 == nullptr) {
		printf("*uniquePointer5: %p\n"
			"*uniquePointer6: %p, uniquePointer6 @%p=>\"%s:%d\"\n",
			*uniquePointer5, *uniquePointer6, &uniquePointer6, uniquePointer6->GetName().data(), uniquePointer6->GetValue());
	}
#if 0
unique.cpp:188:70: error: no match for ‘operator[]’ (operand types are ‘std::unique_ptr<MyClass>’ and ‘int’)
   printf("*uniquePointer6: %p, *uniquePointer7: %p\n", uniquePointer6[0], uniquePointer7[0]);
                                                                      ^
unique.cpp:188:89: error: no match for ‘operator[]’ (operand types are ‘std::unique_ptr<MyClass>’ and ‘int’)
   printf("*uniquePointer6: %p, *uniquePointer7: %p\n", uniquePointer6[0], uniquePointer7[0]);
                                                                                         ^
#endif	
	unique_ptr<MyClass> uniquePointer7(move(uniquePointer6));//equal to uniquePointer7 = move(...)
	if (uniquePointer6 == nullptr) {
		printf("*uniquePointer6: %p, *uniquePointer7: %p\n", *uniquePointer6, *uniquePointer7);
	}
	/*
	special case that there is no copy cons(please note:= is equivalent to ()):
	1. left must be a unique_ptr<T> instantiation
	2. right must be a temp obj of unique_ptr<T>, it can be
	   a. stack var returned by func like make_unique<T> or PassUnquePtr
	   b. T(args...)
	*/
	unique_ptr<MyClass> uniquePointer8 = PassUnquePtr(move(uniquePointer7));
	printf("uniquePointer8 %p points to %p(%p)\n", &uniquePointer8, uniquePointer8.get(), &*uniquePointer8);
	printf("auto des of stack variable:\n");
	return 0;
}