#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

typedef int32_t status_t;
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line(fmt,arg...) printf("%s %d:" fmt, __PRETTY_FUNCTION__, __LINE__, ##arg)
const char gEmptyString[] = {0};
#ifndef B_PACK_CHARS
#define B_PACK_CHARS(c1, c2, c3, c4) (((c1)<<24) | ((c2)<<16) | ((c3)<<8) | (c4))
#endif
class String8
{
public:
	String8();
	String8(const    char* o);
	inline const char *string() const;
private:
	const char *mString;
};

inline const char *String8::string() const
{
	return mString;
}

String8::String8()//:mString(gEmptyString)
{
	report_func();
}

String8::String8(const char* o)
{
	report_func();
}


class String16
{
public:
	operator const char16_t*() const
	{
		report_func();
		report_line("%p\n", mString);
		return mString;
	}
	
private:
	const char16_t* mString;
};

class TextOutput
{
public:
	TextOutput();
	virtual ~TextOutput();
	virtual status_t print(const char* txt, size_t len) = 0;
	//
	TextOutput& operator<<(const char *str)//it's a member func rather than global func in org TextOutput.h
	{
		report_func();
		this->print(str, strlen(str));
		return *this;
	}
	//
};
#if 0
TextOutput& operator<<(TextOutput& to, const char *str)
{
	report_line("\n");
	to.print(str, strlen(str));
	return to;
}
#endif
TextOutput::TextOutput(){
	report_func();
}
//
TextOutput::~TextOutput(){//error if I don't define it
}
//
TextOutput& operator<<(TextOutput& to, const void* val)
{
	char buf[32];
	snprintf(buf, sizeof(buf), "%p", val);
	to.print(buf, strlen(buf));
	report_line("%p\n", &to);
	return to;
}

TextOutput& operator<<(TextOutput& to, int val)
{
	char buf[16];
	sprintf(buf, "%d", val);
	to.print(buf, strlen(buf));
	report_line("%p\n", &to);
	return to;
}


TextOutput& operator<<(TextOutput& to, const String8& val)
{
	to<<val.string();	
	report_line("%p\n", &to);
	return to;
}

class BufferedTextOutput:public TextOutput
{
public:
	virtual status_t print(const char *txt, size_t len)
	{
		printf("BufferedTextOutput: printing [%s]\n", txt);
		return 0;
	}

};
class LogTextOutput:public BufferedTextOutput
{
public:
	virtual ~LogTextOutput(){}
};

class TypeCode
{
public:
	inline TypeCode(uint32_t code);
	inline ~TypeCode();
	inline uint32_t typeCode() const;
private:
	uint32_t mCode;
};

inline TypeCode::TypeCode(uint32_t code):mCode(code)
{
	report_func();
}

inline TypeCode::~TypeCode()
{
	report_func();
}

inline uint32_t TypeCode::typeCode() const
{
	return mCode;
}

//void printTypeCode(uint32_t typeCode, )
TextOutput& operator<<(TextOutput& to, const TypeCode &val)
{
	//printTypeCode(val.typeCode(), textOutputPrinter, (void*)&to);
	report_line("%p, %d\n", &to, val.typeCode());
	return to;
}

inline TextOutput& indent(TextOutput& to)
{
	report_line("from %p\n", &to);
	return to;
}

inline TextOutput& dedent(TextOutput& to)
{
	report_line("from %p\n", &to);
	return to;
}

typedef TextOutput&(*TextOutputMainFunc)(TextOutput&to);
TextOutput& operator<<(TextOutput& to, TextOutputMainFunc func)
{
	report_line("%p\n", &to);
	return func(to);
}

inline TextOutput& endl(TextOutput &to)
{
	to.print("\n", 1);
	report_line("%p\n", &to);
	return to;
}

static LogTextOutput gLogTextOutput;
TextOutput &alog(gLogTextOutput);

class Parcel
{
public:
	Parcel()
	{
		report_func();
	}
	~Parcel()
	{
		report_func();
	}
	void print(TextOutput &to, uint32_t flags = 0) const;
private:
	Parcel(const Parcel &o);
#if 0
	{
		report_func();
	}
#endif
};

void Parcel::print(TextOutput &to, uint32_t /*flags = 0*/) const
{
	to << "Parcel(";
	to << "NULL";
	to << ")";
}

inline TextOutput& operator<<(TextOutput& to, const Parcel& parcel)
{
	report_line("%p, %p\n", &to, &parcel);
	parcel.print(to);
	return to;
}

int main()
{
	uint32_t code = B_PACK_CHARS('_', 'P', 'N', 'G');
	Parcel stack_data;
	const Parcel &data = stack_data;
	alog << "BC_TRANSACTION thr " << (void*)pthread_self() << " / hand "
		<< 0 << " / code " << TypeCode(code) << ": "
		<< indent << data << dedent << endl;//n1
}

int char16_t_test()
{
	printf("sizeof(char16_t):%ld\n", sizeof(char16_t));//2
	char *buf = (char*)malloc(32);
	char16_t *u16str = (char16_t*)buf;
	report_line("\n");
	const char16_t *u16str2 = static_cast<const char16_t*>(String16());
//	char16_t *u16str3 = (char16_t*)String16();//error: invalid cast from type 'String16' to type 'char16_t*'
	const char16_t* u16str4 = (const char16_t*)String16();//ok
	String16 obj;
	String16 & ref = obj;
	const char16_t *u16str5 = (const char16_t*)ref;//ref here is like *(const pointer)
	return 0;
}

/*n1
TextOutput::TextOutput():0x55a5875dc020<---gLogTextOutput's Base ctor, reference alog has no ctor to call, alog's this is the same as gLogTextOutput's this
Parcel::Parcel():0x7ffe4ab1e557<---stack_data's ctor, stack reference data has no ctor to call
TextOutput& TextOutput::operator<<(const char*):0x55a5875dc020<---here member func is called, whose this==alog's this
BufferedTextOutput: printing [BC_TRANSACTION thr ]<---BufferedTextOutput's virtual print() is called
BufferedTextOutput: printing [0x7feaeb9a5d80]
TextOutput& operator<<(TextOutput&, const void*) 89:0x55a5875dc020<---alog's this
TextOutput& TextOutput::operator<<(const char*):0x55a5875dc020<---alog's this
BufferedTextOutput: printing [ / hand ]
BufferedTextOutput: printing [0]
TextOutput& operator<<(TextOutput&, int) 98:0x55a5875dc020<---alog's this
TextOutput& TextOutput::operator<<(const char*):0x55a5875dc020<---alog's this
BufferedTextOutput: printing [ / code ]
TypeCode::TypeCode(uint32_t):0x7ffe4ab1e558<--a temp TypeCode obj is ctor-ed, which is dtor-ed after "<<endl"
TextOutput& operator<<(TextOutput&, const TypeCode&) 155:0x55a5875dc020, 1599098439<---alog's this
TextOutput& TextOutput::operator<<(const char*):0x55a5875dc020<---alog's this
BufferedTextOutput: printing [: ]
TextOutput& operator<<(TextOutput&, TextOutputMainFunc) 174:0x55a5875dc020<---alog's this
TextOutput& indent(TextOutput&) 161:from 0x55a5875dc020<---alog's this
TextOutput& operator<<(TextOutput&, const Parcel&) 218:0x55a5875dc020, 0x7ffe4ab1e557<---alog's this
TextOutput& TextOutput::operator<<(const char*):0x55a5875dc020
BufferedTextOutput: printing [Parcel(]
TextOutput& TextOutput::operator<<(const char*):0x55a5875dc020
BufferedTextOutput: printing [NULL]
TextOutput& TextOutput::operator<<(const char*):0x55a5875dc020
BufferedTextOutput: printing [)]
TextOutput& operator<<(TextOutput&, TextOutputMainFunc) 174:0x55a5875dc020<---alog's this
TextOutput& dedent(TextOutput&) 167:from 0x55a5875dc020
TextOutput& operator<<(TextOutput&, TextOutputMainFunc) 174:0x55a5875dc020<---alog's this
BufferedTextOutput: printing [<--"\n" 1B is printed
]
TextOutput& endl(TextOutput&) 181:0x55a5875dc020<---alog's this
TypeCode::~TypeCode():0x7ffe4ab1e558<--the temp TypeCode obj ctor-ed before is dtor-ed, which is mirror comparing @stack_data
Parcel::~Parcel():0x7ffe4ab1e557<--@stack_data is dtor-ed

*/
