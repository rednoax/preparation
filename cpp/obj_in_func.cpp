#include <stdio.h>
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line(fmt, arg...) printf("%s %d:" fmt,__PRETTY_FUNCTION__, __LINE__, ##arg)
class sp
{
public:
	sp()
	{
		report_func();
	}
	sp(const sp& other)
	{
		report_func();
	}
	sp(sp* o)
	{
		report_line("%p\n", o);
		report_func();
	}
	~sp()
	{
		report_func();
	}
	void get(){
		report_func();
	}
};
/*
func0/1/2 all return a sp object(NOTE these 3 funcs' return type are all object "sp", not
object* or object&) from:
func0:stack object
func1:global object
func2:temp object
1. one temp object is certainlly be created, even for func1, which will return such a temp
   object from a global object;
2. if the returned temp object is not used, then its dtor is called to destructe the temp object;
3. if the returned temp object is used, eg assigned to other same type object, then no
   dtor is called. And the temp object is used as left object directly;
*/
sp func0()
{
	sp o;
	return o;
}

sp g_o;
sp func1()
{
	return g_o;
}

sp func2()
{
	return sp();
}

sp getContexObject(const sp &caller)
{
	report_line("\n");
	return NULL;
}
/*
void func3() 64:fun3 start
sp::sp(sp*) 17:(nil)
sp::sp(sp*):0x7ffc18760076<--a temp obj is constucted for argument
sp getContexObject(const sp&) 59:
sp::sp(sp*) 17:(nil)
sp::sp(sp*):0x7ffc18760077<--a temp obj is constucted for returned sp object
void sp::get():0x7ffc18760077<--sp.get show the returned sp object's this == the above line
sp::~sp():0x7ffc18760077<--the returned object's dtor is called when it is not used any more
sp::~sp():0x7ffc18760076<--the argument obj's dtor, which is mirrored
void func3() 66:fun3 end
*/
void func3()
{
	report_line("fun3 start\n");
	getContexObject(NULL).get();
	report_line("fun3 end\n");
}
/*
sp::sp():0x5634615e0019<--global sp object ctor is called, 
sp::sp():0x7ffef0c36507<--func0's returned object is not used, which conforms to 1+2
sp::~sp():0x7ffef0c36507<--func0's unused returned object is destructed
int main() 50:test1 fin
sp::sp():0x7ffef0c36503<--func0's returned object is used so it conforms to 1+3,ie no dtor
int main() 57:test2 fin
sp::sp(const sp&):0x7ffef0c36504<--func1 return a unused temp object from a global object, conforming to 1+2
sp::~sp():0x7ffef0c36504<--func1 returned, unused temp object is destructed
int main() 60:test3 fin
sp::sp(const sp&):0x7ffef0c36505<--func1 returne a temp object from a global object, and the temp object is used, conforming to 1+3
int main() 62:test4 fin
sp::sp():0x7ffef0c36506<--func2's returned unused object, conforming to 1+2, so explicite temp object here is equivalent to stack object in func0
sp::~sp():0x7ffef0c36506
int main() 64:test5 fin
sp::sp():0x7ffef0c36507<--func2's returned used object, conforming to 1+3, so explicite temp object here is equivalent to stack object in func0
int main() 67:test6 fin
sp::~sp():0x7ffef0c36507
sp::~sp():0x7ffef0c36505
sp::~sp():0x7ffef0c36503<--func0's returned object is used and it is release as mirror
sp::~sp():0x5634615e0019<---global sp object is the last to release
*/
int main()
{
	func0();
	report_line("test1 fin\n");
	sp o __attribute__((unused)) = func0();
	report_line("test2 fin\n");
	func1();
	report_line("test3 fin\n");
	sp o2 __attribute__((unused)) = func1();
	report_line("test4 fin\n");
	func2();
	report_line("test5 fin\n");
	sp o3 = func2();
	(void)o3;
	report_line("test6 fin\n");
	func3();
	return 0;
}
