#include <stdio.h>
#include <memory>
#include <string.h>
#include <map>

class SectionParser {
public:
	virtual ~SectionParser()
	{
		printf("%s(): %p\n", __func__, this);
	}
	SectionParser()
	{
		printf("%s(): %p\n", __func__, this);		
	}
};

class Service {
public:
	Service()
	{
		printf("%s(): %p\n", __func__, this);
	}
	~Service()
	{
		printf("%s: %p\n", __func__, this);		
	}
};

class ServiceParser: public SectionParser {
public:
	ServiceParser():service_(nullptr)//Service * m_ptr = NULL; no Service cons called
	{
		printf("%s: %p\n", __func__, this);
	}
	~ServiceParser()
	{
		printf("%s: %p\n", __func__, this);
	}
private:
	std::unique_ptr<Service> service_;
};

class mystring {
	const char* m_str;
public:
	mystring(const char *s)
	{
		m_str = s;
		printf("%s(\"%s\"): %p\n", __func__, s, this);		
	}
	mystring()
	{
		printf("%s(): %p\n", __func__, this);		
	}
	mystring(const mystring& src)
	{
		m_str = src.m_str;
		printf("%s(const mystring& %p): %p\n", __func__, &src, this);	
	}
	~mystring()
	{
		printf("%s(\"%s\"): %p\n", __func__, m_str, this);		
	}
friend bool operator<(const mystring&, const mystring&);
};

bool operator<(const mystring& s1, const mystring& s2)
{
	return s1.m_str < s2.m_str;
}

class Parser {
public:
	static Parser& GetInstance();
	void AddSectionParser(const mystring& name, std::unique_ptr<SectionParser> parser)
	{
		printf("Add(%p, %p) to map\n", &name, *parser);
		section_parsers_[name] = move(parser);
		printf("fin adding, temp mystring auto des\n");
	}
	~Parser()
	{
		printf("%s: %p\n", __func__, this);
	}
private:
	Parser()
	{
		printf("%s(): %p\n", __func__, this);		
	}
	/*
	class Parser des=>its member des=>map delete all its element pair=>string des, smart pointer unique_ptr<T> des;
	the unique_ptr<T> des will delete the obj T it pointers=>T des,i.e. SectionParser des
	*/
	std::map<mystring, std::unique_ptr<SectionParser>> section_parsers_;
};

Parser& Parser::GetInstance()
{
	static Parser instance;
	return instance;
}

int main()
{
	Parser& parser = Parser::GetInstance();
	//"Service"=>temp mystring obj
	parser.AddSectionParser("Service", std::make_unique<ServiceParser>());//unique_ptr<ServiceParser>(new ServiceParser())
	printf("auto des start(map pair is auto deleted)\n");
	return 0;
}