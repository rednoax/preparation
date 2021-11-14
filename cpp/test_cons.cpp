#include <stdio.h>

class A
{
public:
	A(){
		printf("%s\n", __func__);
	}
private:
	int i;
};
int main()
{
	A a;
	return 0;
}
