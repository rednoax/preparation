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
	std::vector<BufferState> states;
	std::vector<BufferedTextOutput::BufferState> state2;
};
static BufferedTextOutput gBufferedTextOutput;

int main()
{
	report_line();
	TextOutput &alog(gBufferedTextOutput);//equivalent to the next line
	//alog.func();//error: 'class TextOutput' has no member named 'func'
	alog.moveIndent(1);//ok:this is gBufferedTextOutput's
	TextOutput &alog2 = alog;
	alog2.moveIndent(2);//this is gBufferedTextOutput's
	report_line();
	return 0;
}
