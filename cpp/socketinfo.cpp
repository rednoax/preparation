#include <stdio.h>
#include <string>
#include <vector>
struct SocketInfo;
using std::string;
using FN = int (*)(const std::vector<SocketInfo>& args);
struct SocketInfo {
	SocketInfo();
	SocketInfo(const string& name, const string& type, uid_t uid,
		gid_t gid, int perm, const string& socketcon);
	string name;
	string type;
	uid_t uid;
	gid_t gid;
	int perm;
	std::string socketcon;//std:: can be ignored
};

class test_class {
private:
	using PRI_int = int;
public:
	using PUB_int = int;
	using FN2 = int(*)(const std::vector<SocketInfo>& sockinfo);
};

SocketInfo::SocketInfo():uid(0), gid(0), perm(0)
{
	printf("%s: %d, %p", __func__, __LINE__, this);
}

SocketInfo::SocketInfo(const string& name, const string& type, uid_t uid, gid_t gid, int perm, const string& socketcon)
	:name(name), type(type), uid(uid), gid(gid), perm(perm), socketcon(socketcon)
{

	printf("%s: %d, %p", __func__, __LINE__, this);
}

std::vector<SocketInfo> sockets_;
int test(const std::vector<SocketInfo>& sockinfo)
{
	printf("%s: %d\n", __func__, __LINE__);
	return 0;
}

struct info {
	info()
	{
		printf("%s: %d, %p\n", __func__, __LINE__, this);
	}
	info(int i)
	{
		printf("%s: %d, %p %d\n", __func__, __LINE__, this, i);
	}
	info(const char *str)
	{
		printf("%s: %d, %p [%s]\n", __func__, __LINE__, this, str);
	}
	info(const info& src)
	{
		printf("%s: %d, %p=>%p\n", __func__, __LINE__, &src, this);
	}
	~info()
	{
		printf("%s: %d, %p\n", __func__, __LINE__, this);
	}
};

void emplace_back_test(void)
{
	info obj;
	printf("new:");
	std::vector<info> *v = new std::vector<info>;
	printf("%p\n", v);
	//
	printf("emplace_back src(stack obj %p)m const only 1 times\n", &obj);
	v->emplace_back(obj);//typeof(obj) must be info
	printf("emplace_back fin %p, then ease it\n", &(*(v->begin())));
	v->erase(v->begin());
	//
	printf("emplace_back int obj(cons 1 times)\n");
	v->emplace_back(1);//there is only one time constuction compared with push_back
	printf("emplace_back fin %p, then ease it\n", &(*(v->begin())));
	v->erase(v->begin());
	//
	printf("push_back src(stack obj %p), const only 1 times too\n", &obj);
	v->push_back(obj);//typeof(obj) must be info, push_back use its reference so there is no copy constructor happen
	printf("push_back fin %p, then ease it\n", &(*(v->begin())));
	v->erase(v->begin());
	//
	printf("push_back int obj(cons twice, 1st temp obj then vector element cons)\n");
	v->push_back(1);//
	printf("push_back fin %p, then ease it(element will auto des)\n", &(*(v->begin())));
	v->erase(v->begin());
	v->push_back("");
	//
	printf("delete:");//It seems that delete will desctruct all its member!
	delete v;
	printf("%p\n", v);
	std::vector<info> v2;
	printf("push_back %p to local verctor<info>\n", &obj);
	v2.push_back(obj);
	v2.erase(v2.begin());
	printf("emplace_back %p to local vector<info>\n", &obj);
	v2.emplace_back(obj);
	printf("begin desctructor\n");
}

int main()
{
	//sockets_.emplace_back();
	printf("###emplace test start\n");
	emplace_back_test();
	printf("###emplace test end\n");
	FN test2 = test;
	test2(sockets_);
	test_class::FN2 test3 = test;//test_class must be used!
	test3(sockets_);
	test_class::PUB_int a = 4;
	(void)a;
#if 0
	/*
	socketinfo.cpp: In function ‘int main()’:
	socketinfo.cpp:21:21: error: ‘using PRI_int = int’ is private
	  using PRI_int = int;
	                     ^
	socketinfo.cpp:55:14: error: within this context
	  test_class::PRI_int b = 3;
	*/
	test_class::PRI_int b = 3;
	(void)b;
#endif
	return 0;
}