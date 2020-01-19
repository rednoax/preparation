#include <stdio.h>
#include <vector>
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line() printf("%s:%d\n", __FUNCTION__, __LINE__)
class TextOutput
{
public:
	TextOutput()
	{
		report_func();
	}
	TextOutput(const TextOutput &o)
	{
		report_func();
	}
	virtual ~TextOutput()
	{
		report_func();
	}
	virtual void moveIndent(int delta)
	{
		report_func();
	}
};

class BufferedTextOutput: public TextOutput
{
public:
	virtual void func()
	{
		report_func();
	}
private:
	struct BufferState;
	struct ThreadState;
};

struct BufferedTextOutput::BufferState
{
};

struct BufferedTextOutput::ThreadState
{
	std::vector<BufferState> states;//"BufferedTextOutput::" can be ignored
	std::vector<BufferedTextOutput::BufferState> state2;
};
static BufferedTextOutput gBufferedTextOutput;

int main()
{
	report_line();
	TextOutput &alog(gBufferedTextOutput);//equivalent:TextOutput &alog = gBufferedTextOutput;
//then alog can only reference the TextOutput's member;alog can NOT reference TextOutput's derived class's member
	//alog.func();//error: 'class TextOutput' has no member named 'func'
	alog.moveIndent(1);//ok:printed this is gBufferedTextOutput's
	TextOutput &alog2 = alog;//think reference assignment to be a const ptr's assignment
	alog2.moveIndent(2);//printed this is gBufferedTextOutput's
	report_line();
	printf("%p==%p?\n", &alog, &alog2);
	return 0;
}
