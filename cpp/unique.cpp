#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#if 0
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


int main(int argc, char **argv)
{
	//template unique_ptr<MyClass>;
	printf("###stack temp obj start:\n");
	MyClass("test0", 0);
	printf("###stack temp obj des fin\n");
	printf("###these 2 equivalents are NOT temp obj copy cons!!!!!!!!\n");
	printf("push_back(temp obj) will call cons twice: first temp obj cons then copy cons, which "\
			"is not like the following \"obj instantiate using copy cons\"\n");
	MyClass o0(MyClass("test1", 1));//temp obj call related cons, then no copy cons
	MyClass o1 = MyClass("test2", 2);//exactly equal to the above
	printf("###these 2 equivalents really call copy cons\n");
	MyClass o2 = o0;
	MyClass o3(o0);//absolutely equal to the above
	printf("###stack obj from \"temp obj\" fin\n");
	//printf("%d\n", (o0, o1));
	//
	printf("###these 3 quivalent instantiation");
	unique_ptr<MyClass> uniquePointer(make_unique<MyClass>("MyClass", 10));
	printf("__cplusplus: %ld; unique_ptr instance %p\n", __cplusplus, &uniquePointer);//201103
	printf("%s: %d\n", uniquePointer->GetName().c_str(), uniquePointer->GetValue());
	unique_ptr<MyClass> uniquePointer4 = make_unique<MyClass>("MyClass2", 20);//exactly the same as before
	uniquePointer4 = 3;//make_unique<MyClass>("MyClass2", 20);
	printf("uniquePointer4 @%p\n", &uniquePointer4);
	unique_ptr<MyClass> uniquePointer5(unique_ptr<MyClass>(new MyClass("MyClass3", 30)));//the 3rd quivalent
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
	printf("auto des of stack variable:\n");
	return 0;
}