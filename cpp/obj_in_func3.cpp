#include <stdio.h>
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line(fmt, arg...) printf("%s %d:" fmt,__PRETTY_FUNCTION__, __LINE__, ##arg)
template <typename T>
class sp
{
public:
	sp(): m_ptr(0) {report_func();}
	sp(const sp<T>& other): m_ptr(other.m_ptr) {report_func();}
	sp(T* other): m_ptr(other) {report_func();printf("%p\n", other);}
	~sp() {report_func();}
	T* get() const {report_func();return m_ptr;}
	sp<T>& operator=(T*other)//if return type:sp<T>&,no error and no difference in result
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
	T* operator->() const
	{
		report_func();
		return m_ptr;
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
/*
initialization and declaration for static member in class must be separated. the former should has no
"static" while the latter has "static";
*/
	static const String descriptor;
	static const String descriptor2;
	//static const String descriptor3("xx");//error
#if 0
	//error: a storage class can only be specified for objects and functions
	static enum {
		IServiceManager_SENUM = 0,
	};
#endif
/*
1.you can declare an enumeration within a class.An enumeration given in a class
declaration has class scope, so you can use enumerations to provide class scope symbolic
names for integer constants.
2.Note that declaring an enumeration in this fashion does not create a class data mem-
ber.That is, each individual object does not carry an enumeration in it. So the following
IServiceManager_ENUM is just a symbolic name that the compiler replaces with 1 when it
encounters it in code in class scope.
3.the equivalent:static int IServiceManager_ENUM = 1;
*/
	enum {
		IServiceManager_ENUM = 1,
	};
	static sp<IServiceManager> asInterface(const sp<IBinder>&obj)
	{
		report_line("\n");
		sp<IServiceManager> intr;
		if (obj != NULL) {
			intr = static_cast<IServiceManager*>(
				obj->queryLocalInterface(
					IServiceManager::descriptor).get());//at last .get() return "IInterface*"
			printf("intr %p\n", intr.get());
		}
		return NULL;
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

void enum_test()
{
	IServiceManager o0;
	IServiceManager o1(o0);
	const int &i = o0.IServiceManager_ENUM;
	const int &j = o1.IServiceManager_ENUM;
	printf("%s: %d %d;%p %p, %d\n", __func__, o0.IServiceManager_ENUM, o1.IServiceManager_ENUM,
		&i, &j, IServiceManager::IServiceManager_ENUM);
}

int main()
{
	report_line("\n");
	interface_cast<IServiceManager>(getContexObject(NULL));
	enum_test();
	return 0;
}
/*
test result:
String::String(const char*):0x55e8e81fd019<--static var is like global var, their ctors run 1st
String::String(const char*):0x55e8e81fd01a<--descriptor("...") is equivalent to descriptor2 = "..."
int main() 114:
sp<T>::sp(T*) [with T = IBinder]:0x7fffac0f4050<--NULL to a temp sp<IBinder> object, whose dtor is not called before main's return 0, even the argument object is not used  at all
(nil)
sp<T>::sp() [with T = IBinder]:0x7fffac0f4058<--stack temp obj @result's ctor
IBinder::IBinder():0x55e8e8f38e80<---new BpBinder will construct base class IBinder 1st
BpBinder::BpBinder(int):0x55e8e8f38e80<---then derived class ctor, whose this == the above line
sp<T>& sp<T>::operator=(T*) [with T = IBinder]:0x7fffac0f4058<--result = b; this==@result's this, the stack var @result is used so it will not be destructed until it is not used any more
static sp<IServiceManager> IServiceManager::asInterface(const sp<IBinder>&) 77:
sp<T>::sp() [with T = IServiceManager]:0x7fffac0f3fe8<---sp<IServiceManager> intr;'s ctor
bool sp<T>::operator!=(const T*) const [with T = IBinder]:0x7fffac0f4058<---!='s this is the stack object of the above @result
T* sp<T>::operator->() const [with T = IBinder]:0x7fffac0f4058<---stack var's ->
virtual sp<IInterface> IBinder::queryLocalInterface(const String&):0x55e8e8f38e80<--IBinder's virtual func, whose this==BpBinder's
sp<T>::sp(T*) [with T = IInterface]:0x7fffac0f3ff0<--a temp object sp<IInterface> is constructed with NULL when queryLocalInterface returned
(nil)
T* sp<T>::get() const [with T = IInterface]:0x7fffac0f3ff0<--the temp object of the above line is used to call .get()
sp<T>& sp<T>::operator=(T*) [with T = IServiceManager]:0x7fffac0f3fe8<--intr=static_cast<IServiceManager*>(...), whose this==@intr's
sp<T>::~sp() [with T = IInterface]:0x7fffac0f3ff0<---the returned temp object's dtor, since it is not used after supplying .get() with this
T* sp<T>::get() const [with T = IServiceManager]:0x7fffac0f3fe8<--intr.get()
intr (nil)
sp<T>::sp(T*) [with T = IServiceManager]:0x7fffac0f4060<--a temp object is constructed for asInterface returns NULL
(nil)
sp<T>::~sp() [with T = IServiceManager]:0x7fffac0f3fe8<--intr's dtor
sp<T>::~sp() [with T = IServiceManager]:0x7fffac0f4060<--the returned object by asInterface is not used so it is destructed at once
sp<T>::~sp() [with T = IBinder]:0x7fffac0f4058<--stack var returned by getContexObject is not used and released in mirror with the below
sp<T>::~sp() [with T = IBinder]:0x7fffac0f4050<---NULL to a temp sp<IBinder> object's dtor
String::~String():0x55e8e81fd01a<---static var's dtor runs in mirror
String::~String():0x55e8e81fd019<---ditto
*/
