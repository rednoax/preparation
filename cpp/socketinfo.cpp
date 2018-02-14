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
		printf("%s(), %p\n", __func__, this);
	}
	info(int i)
	{
		printf("%s(int %d), %p\n", __func__, i, this);
	}
	info(const char *str)
	{
		printf("%s(\"%s\"), %p\n", __func__, str, this);
	}
	info(const info& src)
	{
		printf("%s(const info&), %p=>%p\n", __func__, &src, this);
	}
	~info()
	{
		printf("%s: %p\n", __func__, this);
	}
};

class mystring {
	//char *m_buf;
public:
	mystring(const char *s)
	{
		//m_buf = malloc(strlen(s) + 1);
		printf("%s(\"%s\"): %p\n", __func__, s, this);//, m_buf);
	}
	mystring(const mystring &s)
	{
		printf("%s(const mystring&): %p=>%p\n", __func__, &s, this);
	}
	~mystring()
	{
		//free(m_buf);
		printf("%s: %p\n", __func__, this);//, m_buf);
	}
};

class combo {
public:
	combo(const info &info, const mystring &n1, const mystring &n2)
	{
		printf("%s(%p, %p, %p): %p\n", __func__, &info, &n1, &n2, this);
	}
	combo(const combo& s)
	{
		printf("%s(const combo&): %p=>%p\n", __func__, s, this);
	}
	~combo()
	{
		printf("%s: %p\n", __func__, this);
	}
};
void emplace_back_typecast_test()
{
	/*
	vector<T>::emplace_back(args...) is the same as "new T(args...)"
	*/
	std::vector<mystring> *vp = new std::vector<mystring>;
	vp->emplace_back("test");//new mystring("test")
	printf("erase %p: ", &*vp->begin());
	vp->erase(vp->begin());
	//push_back's only non-temp obj case is transfer T arg, all others will cons twice!!!
	vp->push_back("test");//a temp mystring("test") is cons and is copied cons for vector element
	printf("erase %p: ", &*vp->begin());
	vp->erase(vp->begin());
	delete vp;
	printf("---multiple self-defined obj args test---\n"
		"emplace_back's 3 args of (const char*)=>associated temp obj since their cons accept (const char*)\n");
	std::vector<combo> *vp2 = new std::vector<combo>;
	vp2->emplace_back("info", "n1", "n2");//equivalent: new combo("info", "n1", "n2")
	printf("---there is 3 temp obj destructors above---\n");
	printf("erase %p: ", &vp2->begin()[0]);
	vp2->erase(vp2->begin());
	printf("now try push_back\n");
	info i;
	mystring s1("s1"), s2("s2");
	/*
	there is only one form of push_back:	push_back(elem) that Appends a copy of elem at the end
	1. its arg can be "T obj" then it will "new T(obj)", i.e. call T's copy cons. If the arg is not of type T, a temp T is
	    tried to be instantiated from arg if there is associated T cons. Please noted in the whole process of temp T's construction
	    there should only one argument for T's constructor, even T cons have been overloaded with more args. The following can fail:
socketinfo.cpp:124:26: error: no matching function for call to ¡®std::vector<combo>::push_back(info&, mystring&, mystring&)¡¯
  vp2->push_back(i, s1, s2);
socketinfo.cpp:139:36: error: no matching function for call to ¡®std::vector<combo>::push_back(const char [6], const char [3], const char [3])¡¯
  vp2->push_back("combo", "n1", "n2");
	2. if 1 fails, it cannot accept arg of other type
	*/
	vp2->push_back(combo(i, s1, s2));
	printf("erase %p: ", &vp2->begin()[0]);
	vp2->erase(vp2->begin());
	printf("push_back a temp obj, obj cons will typecast args, that's why"
		" emplace_back transfering args to obj cons can work, let obj cons to typecast these args...\n");
	printf("4 temp objs:3 via args, 1 via combo\n");
	vp2->push_back(combo("combo", "n1", "n2"));
	printf("erase %p: ", &vp2->begin()[0]);
	vp2->erase(vp2->begin());
	delete vp2;
	printf("auto des:\n");
}

void emplace_back_test(void)
{
	info obj;
	printf("new std::vector<info>:");
	std::vector<info> *v = new std::vector<info>;//no T construction called during vector<T> instantiation
	printf("%p, .size() %ld\n", v, v->size());
	/*
	.emplace_back(args...)Appends a copy of an element initialized with args at the end (returns nothing; since C++11)
	its args is direclty given to T's overloaded constructor, that seems like make_unique<T>
	template <typename T, typename... Args>
	unique_ptr<T> make_unique(Args... args)
	{
		return unique_ptr<T>{ new T(args...) };
	}
	*/
	printf("emplace_back stack obj %p, const only 1 time\n", &obj);
	v->emplace_back(obj);//typeof(obj) must be info
	printf("emplace_back fin %p, then erase it: ", &*v->begin());
	v->erase(v->begin());
	//
	printf("push_back stack obj %p, const only 1 time too\n", &obj);
	v->push_back(obj);//typeof(obj) must be info, push_back use its reference so there is no copy constructor happen
	printf("push_back fin %p, then erase it: ", &*v->begin());
	v->erase(v->begin());
	//
	printf("emplace_back(int), cons 1 time\n");
	v->emplace_back(1);//there is only one time constuction compared with push_back, new T(args...)
	printf("emplace_back fin %p, then erase it: ", &*v->begin());
	v->erase(v->begin());//vector will erase its obj constructed via new T(1), we should not worry about it.
	//
	printf("push_back(int), const 2 times, 1st temp obj then vector element cons\n");
	v->push_back(1);
	printf("push_back fin %p, then erase it(element will auto des): ", &v->begin()[0]);
	v->erase(v->begin());
	//
	printf("emplace_back empty arg, cons 1 time\n");
	v->emplace_back();//there is only one time constuction compared with push_back
	printf("emplace_back fin %p, then erase it: ", &*v->begin());
	v->erase(v->begin());
	//
	printf("push_back empty arg, push_back() can not be used, just using temp obj, so const 2 times\n");
#if 0
	socketinfo.cpp:114:15: error: no matching function for call to â€˜std::vector<info>::push_back()â€™
	  v->push_back();
	               ^
#endif
	v->push_back(info());//arg is a temp obj
	printf("push_back fin %p, then erase it: ", &v->begin()[0]);
	v->erase(v->begin());
	//
	printf("emplace_back(\"\"), cons 1 time\n");
	v->emplace_back("");
	printf("emplace_back fin %p, then erase it: ", &v->begin()[0]);
	v->erase(v->begin());
	//
	printf("push_back(\"\"), cons 2 times, 1st temp obj then copy cons\n");
	v->push_back("");
	printf("push_back fin %p, then erase it: ", &v->begin()[0]);
	v->erase(v->begin());
	//
	v->emplace_back("only");
	printf("start delete:");
	//delete->class desctruct->release all vector elements->T desctruct(what's T? vector<T>)
	delete v;
	printf("fin to delete %p\n", v);
	std::vector<info> v2;
	printf("push_back %p to local verctor<info>\n", &obj);
	v2.push_back(obj);
	//v2.erase(v2.begin());
	printf("emplace_back %p to local vector<info>, the vector<info>[0] is copied to be continued with vector<info>[1]"
			" then deleted\n", &obj);
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
	dst.clear();
	dst.append("suffix", 3);
	printf("dst:%s\n", dst.data());
	dst.push_back('!');
	printf("after push_back, dst '%s', .size() %ld, .begin() '%c', %p\n", dst.data(), dst.size(), dst.begin()[0], &*dst.begin());//, data.end());
	printf(".end() %p[%x]\n", &*dst.end(), *dst.end());
	printf("dst via iterator '%s'\n", &*dst.begin());
	//vector<T> name(T iterator, T iterator);
	std::vector<char> vec(dst.begin(), dst.end());
	printf("vec[0]:'%c'. vec[%ld]:'%c'\n", vec[0], vec.size() - 1, vec[vec.size() - 1]);
	std::vector<char> vec2;
	printf("emptry std::vector<char> .size is %ld\n", vec2.size());
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
	socketinfo.cpp: In function â€˜int main()â€™:
	socketinfo.cpp:21:21: error: â€˜using PRI_int = intâ€™ is private
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
	emplace_back_typecast_test();
	return 0;
}
