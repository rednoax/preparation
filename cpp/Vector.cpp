#include <stdio.h>
#include <sys/types.h>
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
};


int main()
{
	Vector<int> o;
	o.size();
	return 0;
}
