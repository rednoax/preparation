#include <stdio.h>
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line(fmt, arg...) printf("%s %d:" fmt,__PRETTY_FUNCTION__, __LINE__, ##arg)
template <typename T>
class sp
{
public:
	sp(): m_ptr(0) {report_func();}
	sp(const sp<T>& other): m_ptr(other.m_ptr) {report_func();}
	sp(T* other): m_ptr(other) {report_func();}
	~sp() {report_func();}
	T* get() const {report_func();return m_ptr;}
	sp& operator=(T*other)
	{
		report_func();
		m_ptr = other;
		return *this;
	}
	bool operator!=(const T* o) const
	{
		report_func();
		return m_ptr != o;
	}
private:
	T *m_ptr;
};
class String
{
public:
	String() {report_func();}
	String(const char*s) {report_func();}
	~String() {report_func();}
};
class IInterface
{
public:
	IInterface() {report_func();}
protected:
	virtual ~IInterface() {report_func();}
};
class IBinder
{
public:
	IBinder() {report_func();}
	virtual sp<IInterface> queryLocalInterface(const String& descriptor) {
		report_func();
		return NULL;
	}
protected:
	~IBinder() {report_func();}
};

class BpBinder:public IBinder
{
public:
	BpBinder(int handle) {
		report_func();
	}
protected:
	virtual ~BpBinder() {
		report_func();
	}
};

class IServiceManager: public IInterface
{
public:
	static const String descriptor;
	static const String descriptor2;
	static sp<IInterface> asInterface(const sp<IBinder>&obj)
	{
		report_line();
		sp<IServiceManager> intr;
		if (obj != NULL) {
			intr = static_cast<IServiceManager*>(
				obj->queryLocalInterface(
					IServiceManager::descriptor).get());
			printf("intr %p\n", intr);
		}
	}
	IServiceManager() {report_func();}
	~IServiceManager() {report_func();}
};
const String IServiceManager::descriptor("android.os.IServiceManager");
const String IServiceManager::descriptor2 = "android.os.IServiceManager2";

template <typename T>
inline sp<T> interface_cast(const sp<IBinder>& obj)
{
	return T::asInterface(obj);
}

sp<IBinder> getStrongProxyForHandler(int handle)
{
	sp<IBinder> result;
	IBinder *b = new BpBinder(handle);
	result = b;
	return result;
}

sp<IBinder> getContexObject(const sp<IBinder>&)
{
	return getStrongProxyForHandler(0);
}

int main()
{
	report_line();
	interface_cast<IServiceManager>(getContexObject(NULL));
	return 0;
}
