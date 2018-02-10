#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <unistd.h>
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
	PRI_int m_i;//ok
	PUB_int m_i2;//ok
};

SocketInfo::SocketInfo():uid(0), gid(0), perm(0)
{
	printf("%s: %d, %p", __func__, __LINE__, this);
}

SocketInfo::SocketInfo(const string& name, const string& type, uid_t uid, gid_t gid, int perm, const string& socketcon)
	:name(name), type(type), uid(uid), gid(gid), perm(perm), socketcon(socketcon)
{

	printf("%s: %d, %p, %s\n", __func__, __LINE__, this, name.c_str());
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
	printf("delete:");
	//delete->class desctruct->release all vector elements->T(vector<T>) desctruct
	delete v;
	printf("%p\n", v);
	std::vector<info> v2;
	printf("push_back %p to local verctor<info>\n", &obj);
	v2.push_back(obj);
	//v2.erase(v2.begin());
	printf("emplace_back %p to local vector<info>\n", &obj);
	v2.emplace_back(obj);
	std::vector<info>::iterator it;
	int i;
	for (i = 0, it = v2.begin(); it != v2.end(); i++, it++) {
		printf("%d %p\n", i, &*it);
	}
	printf("begin desctructor\n");//vector will release its own element when it destucts
}

void string_test()
{
	string str = "012";
	string dst = str;//directly =
	printf("string str:%s, bool(!=\"012\")%d\n", str.c_str(), str != "012");//directly != or ==
	//the src/dst 's space holding char[] is different
	printf("dst %s: %p, src: %p, %p==%p?\n", dst.c_str(), &dst, &str, dst.c_str(), str.c_str());
	printf("%s:empty %d\n", dst.c_str(), dst.empty());
	dst = "";
	printf("[%s]empty %d\n", dst.c_str(), dst.empty());
	dst = "prefix:";
	dst.append(str.c_str(), strlen(str.c_str()));
	printf("appended dst[%s]\n", dst.c_str());
	dst.append("suffix");
	printf("appended dst[%s][%s]\n", dst.data(), dst.c_str());
	string result;
	printf("default string obj[%s][%s]:%ld\n", result.c_str(), result.data(), result.size());//.size is strlen
}
//http://zh.cppreference.com/w/cpp/language/range-for
void range_based_for_test(void)
{
	int i;
	using std::vector;
	vector<int> v;
	for (i = 0; i < 20; i ++) {
		v.push_back(i++);
		v.emplace_back(i);
	}
	for (const auto& iter: v) {
		printf("%p: %d\n", &iter, iter);
	}
	//multiple char[] of the following args can all be changed to string, amazing!
	sockets_.emplace_back("logd", "stream", 0, 0, 0666, "");
	sockets_.emplace_back("logdr", "seqpacket", 0, 0, 0666, "");
	printf("%ld elements in vector<SocketInfo> sockets_\n", sockets_.size());
	printf("[1]:%s; element@%p %p\n", sockets_[1].name.c_str(), &sockets_[0], &sockets_[1]);
	for (const SocketInfo& si: sockets_) {
		printf("%p: %s:%s:%d:%d:%o:%s\n", &si, si.name.c_str(), si.type.c_str(), si.uid, si.gid, si.perm, si.socketcon.c_str());
	}
	string_test();
}

int main()
{
	//sockets_.emplace_back();
	printf("###emplace test start\n");
	emplace_back_test();
	printf("###emplace test end\n");
	range_based_for_test();
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
	printf("getpgid %d\n", getpgid(getpid()));//or getpgid(0) to get self pgid
	system("ps -o pgid,pid,ppid,comm");
	int ret = setpgid(0, getppid());
	printf("setpgid %d=>%d, %d\n", getpid(), getppid(), ret);
	system("ps -o pgid,pid,ppid,comm");
	ret = setpgid(0, getpid());
	printf("setpgid %d=>%d: %d\n", getpid(), getpid(), ret);
	system("ps -o pgid,pid,ppid,comm");
	return 0;
}