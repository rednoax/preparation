#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <vector>
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
		printf("%s copy %p=>%p\n", __func__, &src, this);
	}
	MyClass()
	{
		printf("%s(): %p\n", __func__, this);
	}
	MyClass(const std::string& name, int value)
		:m_Name(name)
		,m_value(value)
	{
		printf("%s(\"%s\", %d): %p\n", __func__, name.data(), value, this);
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

//TODO:Why no move in the func body?
unique_ptr<MyClass> PassUnquePtr(unique_ptr<MyClass> obj)
{
	printf("%s: stack temp obj %p(%p)\n", __func__, &obj, &*obj);
	return obj;
}

MyClass __make_unique()
{
	MyClass stack("stack0", 0);
	printf("%s(): %p\n", __func__, &stack);
	return stack;
}

//when return arg is used as assignment, i.e. "class obj = __make_unique;" obj will instantiate with copy cons
MyClass __make_unique(MyClass arg)
{
	printf("%s(@%p)\n", __func__, &arg);
	return arg;
}

MyClass __make_unique2()
{
	return MyClass("stack1", 1);
}

/*
"The C++ Standard Library":
3.1.5 Move Semantics and Rvalue References
Finally, note the following two remarks about this feature: (1) overloading rules for rvalue and
lvalue references and (2) returning rvalue references.
*/
class NewClass {
private:
	std::string m_Name;
	int m_value;
public:
	NewClass(const NewClass& src)//lvalue reference
	{
		m_Name = src.m_Name;
		m_value = src.m_value;
		printf("%s copy %p=>%p\n", __func__, &src, this);
	}
	NewClass()
	{
		printf("%s(): %p\n", __func__, this);
	}
	NewClass(const std::string& name, int value)
		:m_Name(name)
		,m_value(value)
	{
		printf("%s(\"%s\", %d): %p\n", __func__, name.data(), value, this);
	}
	NewClass(NewClass&& rvalue)//rvalue reference
	{
		m_value = rvalue.m_value + 1;
		m_Name = rvalue.m_Name;
		printf("%s(&&): %p=>%p\n", __func__, &rvalue, this);
	}
	~NewClass()
	{
		printf("%s:\"%s\" %p\n", __func__, m_Name.c_str(), this);
	}
	NewClass& operator=(const NewClass &src)
	{
		m_Name = src.m_Name;
		m_value = src.m_value;
		printf("%s: %p<=%p\n", __func__, this, &src);
		return *this;
	}
};

//NRVO:no move or copy const, the stack var address is the same as the rvalue address in assignment
NewClass __make_unique(int unused)
{
	NewClass stack("stack1", 1);
	printf("%s(): %p\n", __func__, &stack);
	return stack;
}

//when return arg is used as assignment, i.e. "class obj = __make_unique;" obj will instantiate with move or copy cons
NewClass __make_unique(NewClass arg)
{
	printf("%s(@%p)\n", __func__, &arg);
	return arg;
}

/*
NRVO:no move or copy const, the stack var address is the same as the rvalue address in assignment
so __make_unique2 is the equivalent to __make_unique(int)
*/
NewClass __make_unique2(int unused)
{
	return NewClass("stack2", 2);
}

void copy_elision_func_return_test2()
{
	printf("###%s: when return an obj from called func's stack, either copy cons elision or NewClass(NewClass&&) is used!!\n", __func__);
	NewClass src("src", 10);
	printf("src @%p, 'src=>func(arg)' via copy cons=>'dst = retuened stack arg' via NewClass(&&):\n", &src);
	NewClass dst = __make_unique(src);
	printf("src/dst @%p/%p\n", &src, &dst);
	NewClass o0 = __make_unique(-1);
	printf("o0 @%p\n", &o0);
	NewClass o1 = __make_unique2(-1);
	printf("o1 @%p\n", &o1);
	printf("###%s, begin des\n", __func__);
}

/*
https://en.wikipedia.org/wiki/Copy_elision
http://en.cppreference.com/w/cpp/language/copy_elision
*/
void copy_elision_func_return_test()
{
	printf("###%s.....\n", __func__);
	MyClass o0 = __make_unique();
	printf("o0 @%p\n", o0);
	MyClass o1(__make_unique());
	printf("o1 @%p\n", o1);
	MyClass src("src", 9);
	printf("start =, src @%p\n", &src);
	MyClass o2 = __make_unique((src));
	printf("src/o2 @%p/%p\n", &src, &o2);
	MyClass o3 = __make_unique2();
	printf("o3 @%p\n", o3);
	MyClass o4 = __make_unique(MyClass("o4", 4));
	printf("o4 @%p\n", o4);
	MyClass o5 = __make_unique(__make_unique2());
	printf("o5 @%p\n", o5);
}

void emplace_back_test()
{
	/*
	new and vector emplace_back behave differently.
	Actually we need not care if the temp obj is generated, for it will des if it really has been consed.
	We just need to care the final element on heap.
	The emplace_back will forward args to T's cons. The cons can be copy cons or rvalue reference cons, or other valid T cons:
	1. you can use move to enforce rvalue reference cons is called
	2. when emplace_back is called with a temp obj, the temp obj is used as rvalue reference so the rvalue reference cons is called
	*/
	printf("new, cons only once, no copy/move from stack to heap\n");
	MyClass *o0 = new MyClass();
	printf("o0 @%p\n", o0);
	MyClass *o1 = new MyClass(*o0);
	printf("o1 @%p\n", o1);
	std::vector<NewClass> v;
	printf("emplace temp obj(on stack): temp obj cons=>T&& cons, the final element is on heap\n");
	v.emplace_back(NewClass());
	v.erase(v.begin());
	printf("emplace temp obj via copy: temp obj(on stack) cons via copy=>T&& cons, final element is on heap\n");
	v.emplace_back(NewClass(v[0]));
	v.erase(v.begin());
	printf("emplace non temp obj, copy cons is used dafaultly but move can be used to enforce move\n");
	NewClass c;
	v.emplace_back(c);
	v.erase(v.begin());
	printf("move enforcement\n");
	v.emplace_back(std::move(c));
	v.erase(v.begin());
	printf("emplace temp obj while it has no move, so only copy cons for final element:\n");
	std::vector<MyClass> v2;
	v2.emplace_back(MyClass());
	v2.pop_back();
	MyClass o;
	v2.emplace_back(o);//v2.emplace_back(std::move(o)); will be right
	printf("%s auto des\n", __func__);
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
	/*
	http://en.cppreference.com/w/cpp/language/copy_elision
	When a nameless temporary, not bound to any references, would be copied or moved (since C++11) into an
	object of the same type (ignoring top-level cv-qualification), the copy/move (since C++11) is omitted.
	When that temporary is constructed, it is constructed directly in the storage where it would otherwise
	be copied or moved (since C++11) to. When the nameless temporary is the argument of a return statement,
	this variant of copy elision is known as RVO, "return value optimization".
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
	emplace_back_test();
	printf("###stack obj from \"temp obj\" fin\n");
	copy_elision_func_return_test();
}

void move_test()
{
	MyClass *p = new MyClass("st", 1);
	unique_ptr<MyClass> o1(p);
#if 0//core dump
	unique_ptr<MyClass> o2(p);
	printf("%s: %p==%p?\n", __func__, &*o1, &*o2);
	sleep(5);
#endif
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
/*
unique.cpp:88:50: error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = MyClass; _Dp = std::default_delete<MyClass>]’
  unique_ptr<MyClass> uniquePointer2(uniquePointer);
                                                  ^
unique.cpp:110:39: error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = MyClass; _Dp = std::default_delete<MyClass>]’
  unique_ptr<MyClass> uniquePointer3 = uniquePointer;
                                       ^
unique.cpp:120:39: error: invalid conversion from ‘int’ to ‘MyClass*’ [-fpermissive]
  unique_ptr<MyClass> uniquePointer5 = 3;//make_unique<MyClass>("MyClass2", 20);
                                       ^                                        
*/
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
/*
unique.cpp:188:70: error: no match for ‘operator[]’ (operand types are ‘std::unique_ptr<MyClass>’ and ‘int’)
   printf("*uniquePointer6: %p, *uniquePointer7: %p\n", uniquePointer6[0], uniquePointer7[0]);
                                                                      ^
unique.cpp:188:89: error: no match for ‘operator[]’ (operand types are ‘std::unique_ptr<MyClass>’ and ‘int’)
   printf("*uniquePointer6: %p, *uniquePointer7: %p\n", uniquePointer6[0], uniquePointer7[0]);
                                                                                         ^
*/
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
	copy_elision_test();
	move_test();
	copy_elision_func_return_test2();
	printf("auto des of stack variable:\n");
	return 0;
}