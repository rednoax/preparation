#include <stdio.h>
#include <sys/types.h>
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line() printf("%s:%d\n", __FUNCTION__, __LINE__)

typedef unsigned int uint32_t;
template <typename T>
struct trait_pointer
{
	enum
	{
		value = false,
	};
};

template <typename T>
struct trait_pointer<T*>
{
	enum
	{
		value = true,
	};
};
template<typename TYPE>
struct traits {
	enum {
		is_pointer = trait_pointer<TYPE>::value,
		has_trivial_ctor = is_pointer,
	};
};
class VectorImpl
{
public:
	VectorImpl(size_t itemSize, uint32_t flags)
		:mCount(0), mFlags(flags), mItemSize(itemSize)
	{
	}
	size_t size() const
	{
		printf("%s: %ld\n", __PRETTY_FUNCTION__, mCount);
		return mCount;
	}
	int add(const void *item)
	{
		printf("%s: %p\n", __func__, item);
		return 0;
	}
private:
	size_t mCount;
	const uint32_t mFlags;
	const size_t mItemSize;
};

template <typename TYPE>
class Vector:private VectorImpl
{
public:
	Vector():VectorImpl(sizeof(TYPE), traits<TYPE>::has_trivial_ctor)
	{
	}
	size_t size() const
	{
		printf("%s\n", __PRETTY_FUNCTION__);
		return this->VectorImpl::size();
	}
	int add(const TYPE& item)
	{
		report_func();
		printf("from %p\n", &item);
		return VectorImpl::add(&item);
	}
};
struct BufferState {};

template<typename T>
class sp
{
public:
	sp():m_ptr(0){report_func();}
	sp(T*o): m_ptr(o)
	{
		printf("from %p:", o);
		report_func();
	}
	T* get() const
	{
		return m_ptr;
	}
	~sp(){report_func();}
private:
	T * m_ptr;
};

int main()
{
	Vector<int> o;
	o.size();
	printf("%s:traits<sp<BufferState>>::has_trival_ctor %d\n"
		"traits<sp<<BufferState>*>>::has_trivial_ctor %d\n",
		__func__, traits<sp<BufferState>>::has_trivial_ctor,
		traits<sp<BufferState>*>::has_trivial_ctor);
	Vector<sp<BufferState>> states;
	states.add(NULL);//see comment0
	report_line();
	return 0;
}
/*comment 0
from (nil):sp<T>::sp(T*) [with T = BufferState]:0x7ffda86a65d8<--a temp sp<BufferState> object is constructed, whose .m_ptr==NULL
int Vector<TYPE>::add(const TYPE&) [with TYPE = sp<BufferState>]:0x7ffda86a6600<--Vector<sp<BufferState>> @states's this
from 0x7ffda86a65d8<--add's argument is the temp object constructed in the above
add: 0x7ffda86a65d8<---Vector's base class VectorImpl's add show the source this is the same as the above, it is not NULL even the most beginning call is add(NULL)
sp<T>::~sp() [with T = BufferState]:0x7ffda86a65d8<---when add(NULL) finished, the temp object is not used any more so its dtor is called before main returns
main:104
*/
