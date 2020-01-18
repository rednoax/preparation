#include <stdio.h>
//forward declaration, then it can be used only as
//1.ptr or reference 
//2.arg or return type when declaring(rather than define) some function
class TextOutput;//if commented, error: 'TextOutput' does not name a type
class BufferedTextOutput//: public TextOutput//if open:error: invalid use of incomplete type 'class TextOutput'
{
	TextOutput *p;
};

class TextOutput
{
public:
	class Bundle;//no error even no defination
};

int main()
{
	return 0;
}
