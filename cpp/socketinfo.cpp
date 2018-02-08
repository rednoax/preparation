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

int main()
{
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