#include <iostream>

class A
{
public:
	int a;
	int *p;
	/*
	A(int i)
	{
		a = i;
	}
	*/
	void print()
	{
		printf("%x %p\n", a, p);
	}
};
A ma;
int main()
{
	int i;
	char c;
	ma.print();//0 0
	std::cin >> i >> c;//'\t\t 304\t\t 20'=>304 50
	printf("%d %d\n", i, c);
	return 0;
}
