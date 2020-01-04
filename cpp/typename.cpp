#include <stdio.h>
int array[2];
//typedef array array1;//wrong:error: 'array' does not name a type
typedef int array2[2];//ok
typedef array2 array3;//ok:int[2]
class RefBase
{
public:
	class weakref_type
	{
	};
	static int weakref_type;//if removed, the typedef line in wp can ignore "typename"
};

class RefBase2
{
public:
	class weakref_type{};
protected:
	RefBase2(): mRefs(NULL){}
	virtual ~RefBase2()
	{
/*
typename.cpp:22:11: error: assignment of read-only member 'RefBase2::mRefs'
   mRefs = NULL;
           ^
		//mRefs = NULL;//wrong,see above
*/
/*
why const_cast<weakref_impl*>(mRefs) = NULL is error in cpp:
https://stackoverflow.com/questions/8766109/lvalue-required-as-left-operand-of-assignment-in-odd-place-c
C pointer casts create an lvalue; C++ pointer casts do not unless you cast to a reference type
*/
		//const_cast<weakref_impl*>(mRefs) = NULL;//error: lvalue required as left operand of assignment
		const_cast<weakref_impl*&>(mRefs) = NULL;
	}
private:
	class weakref_impl: public weakref_type
	{
	};
	weakref_impl *const mRefs;
};
//template <typename T>//to prove "typedef typename" here has no relation to template, remove template!
class wp
{
public:
/*
if no "typename" in the following typedef, there will be error emitted:
typename.cpp:23:19: error: 'weakref_type' in 'class RefBase' does not name a type
  typedef RefBase::weakref_type weakref_type;
                   ^~~~~~~~~~~~
typename.cpp:12:13: note: 'RefBase::weakref_type' declared here
  static int weakref_type;
             ^~~~~~~~~~~~
typename.cpp:25:2: error: 'weakref_type' does not name a type
  weakref_type* m_refs;
  ^~~~~~~~~~~~
*/
	//typedef RefBase::weakref_type weakref_type;//wrong: see above
	typedef typename RefBase::weakref_type weakref_type;//typename here is necessary, wrong otherwise
	typedef RefBase2::weakref_type weakref_type2;//ok:as weakref_type has no ambiguity in RefBase2
private:
	weakref_type* m_refs;
};

int main()
{
	array3 o;
	printf("sizeof(int[2]): %ld\n", sizeof(o));
	return 0;
}
