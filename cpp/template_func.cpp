#include <stdio.h>
#include <stdlib.h>

template<typename T>
int writeAligned(T val)
{
	static char *mem = static_cast<char*>(malloc(320));
	*reinterpret_cast<T*>(mem) = val;
	return 0;
}

int main()
{
	int i = 32;
	writeAligned(i);//ok
	i = 16;
	writeAligned<int>(i);//standard
	i = 8;
	writeAligned<>(i);//ok
	return 0;
}
