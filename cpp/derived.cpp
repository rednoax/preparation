#include <stdio.h>
#include <vector>
#include <unistd.h>
class b0 {
public:
	b0()
	{
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}
	~b0()
	{
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}
};

class b1 {
public:
	b1()
	{
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}
	virtual ~b1()
	{
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}
};

class d0: public b0 {
public:
	d0() {
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}
	~d0() {
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}

};

class d1: public b1 {
public:
	d1() {
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}
	~d1() {
		printf("%s: %d %p\n", __func__, __LINE__, this);
	}
};

class spid {
public:
	static const char name[32];// = "abc";
};
const char spid::name[32] = "spid";
int main()
{
	d0 *pd0 = new d0;
	delete pd0;
	printf("start new b1:\n");
	b1 *pb1 = new b1();
	printf("start new d1:\n");
	d1 *pd1 = new d1();
	std::vector<b1*> v;
	v.emplace_back(pb1);
	v.emplace_back(pd1);
	for (std::vector<b1*>::iterator it = v.begin(); it != v.end(); it++) {//const auto si:v) {
		printf("start delete b1 %p\n", *it);
		delete *it;
	}
	printf("%ld\n", v.size());
	v.pop_back();
	v.pop_back();
#if 10
	pb1 = new b1();
	pd1 = new d1();
	v.push_back(pb1);
	v.push_back(pd1);
	printf("%ld\n", v.size());
	for (const auto& si:v) {
		printf("delete %p\n", si);
		delete si;
	}
	pause();
#endif
	return 0;
}
