#include <stdio.h>
#include <string>
//both is ok
//using namespace std;
using std::string;
typedef int status_t;
class IServiceManager
{
public:
	virtual status_t addService(const string& name, bool allowIsolated = false) = 0;
};

class BpServiceManager: public IServiceManager
{
public:
	virtual status_t addService(const string& name, bool allowIsolated)
	{
		printf("%s: %d\n", name.c_str(), allowIsolated);
		return 0;
	}
};

int main()
{
	BpServiceManager obj;
	obj.addService(string("test"), false);//cannot ignore false
	IServiceManager *p = &obj;
	p->addService(string("test"));//false can be ignored
	return 0;
}