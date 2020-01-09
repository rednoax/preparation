#include <stdio.h>
namespace android {
template <typename T>
struct trait_pointer
{
	enum
	{
		value = false,
	};
};

template <typename T>
struct trait_pointer<T*>//partial specialization
{
	enum
	{
		value = true,
	};
};
template <typename T>
class A
{
};
};

using namespace android;
void trait_debug()
{
	printf("%d, %d\n", trait_pointer<int>::value, trait_pointer<int*>::value);
}

int main()
{
	trait_debug();
	return 0;
}
