#include <stdio.h>

class TextOutput
{
public:
	TextOutput();
	virtual ~TextOutput();
};

class TextOutput2
{
public:
	TextOutput2();
	~TextOutput2();
};

class TextOutput3
{
private:
	TextOutput3();
	~TextOutput3();
};

class TextOutput4
{
public:
	TextOutput4() {}
	~TextOutput4() {}
	TextOutput4(const TextOutput4 &o);
};
//TextOutput obj;//if commented, the whole file building is ok;else error;
TextOutput4 obj4;
//TextOutput4 obj4_1(obj4);//if uncommented, build error.
//so if U use some ctor/dtor, it must be defined. Only declaring it will be error
//if some ctor is not used at all. It can be declared only but not defined.


int main()
{
	return 0;
}
