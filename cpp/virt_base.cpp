//based on https://www.learncpp.com/cpp-tutorial/128-virtual-base-classes/
#include <stdio.h>
#define report_func() printf("%s:%p\n", __PRETTY_FUNCTION__, this)
#define report_line() printf("%s: %d\n", __PRETTY_FUNCTION__, __LINE__)
class PowerDevice
{
public:
	PowerDevice(int power)
	{
		report_func();
		m_power = power;
	}
	void print()
	{
		printf("%s %p: %d\n", __PRETTY_FUNCTION__, this, m_power);
	}
private:
	int m_power;
};

class Scanner:public PowerDevice
{
public:
	Scanner(int scanner, int power):PowerDevice(power)
	{
		report_func();
	}
};

class Scanner2:virtual public PowerDevice
{
public:
	Scanner2(int scanner, int power):PowerDevice(power)
	{
		report_func();
	}
};

class Printer:public PowerDevice
{
public:
	Printer(int printer, int power): PowerDevice(power)
	{
		report_func();
	}
};

class Printer2:virtual public PowerDevice
{
public:
	//whose initializer list is required to create Printer2 objects but ignored when create Copier2 objects
	Printer2(int printer, int power): PowerDevice(power)
	{
		report_func();
	}
};

class Copier:public Scanner, public Printer
{
public:
	Copier(int scanner, int printer, int power)
		:Scanner(scanner, power), Printer(printer, power)
	{
		report_func();
		report_line();
	}
};

class Copier2:public Printer2, public Scanner2
{
public:
/*
virt_base.cpp: In constructor 'Copier2::Copier2(int, int, int)':
virt_base.cpp:72:53: error: no matching function for call to 'PowerDevice::PowerDevice()'
   :Printer2(printer, power), Scanner2(scanner, power)
													 ^
	Copier2(int scanner, int printer, int power)
		:Printer2(printer, power), Scanner2(scanner, power)//wrong
	{
		report_func();
		report_line();
	}
*/
/*
virt_base.cpp: In constructor 'Copier2::Copier2(int, int, int)':
virt_base.cpp:84:49: warning: base 'Scanner2' will be initialized after [-Wreorder]
   :PowerDevice(3), Scanner2(1, 3), Printer2(2, 3)
												 ^
virt_base.cpp:84:49: warning:	base 'Printer2' [-Wreorder]
virt_base.cpp:83:2: warning:   when initialized here [-Wreorder]
  Copier2(int scanner, int printer, int power)
  ^~~~~~~
	Copier2(int scanner, int printer, int power)//waring as the initializing sequence doesnot conform to inheritence sequence
		:PowerDevice(3), Scanner2(1, 3), Printer2(2, 3)
	{
		report_func();
		report_line();
	}
*/
/*
if a class inherits one or more classes that have virtual parents, the most derived class is
responsible for constructing the virtual base class. In this case, Copier2 inherits Printer2 and
Scanner2, both of which have a PoweredDevice virtual base class. Copier2, the most derived class,
is responsible for creation of PoweredDevice.
Note that this is true even in a single inheritance case: if Copier2 was singly inherited from
Printer2, and Printer2 was virtually inherited from PoweredDevice, Copier2 is still responsible
for creating PoweredDevice.
*/
	Copier2(int scanner, int printer, int power)
		:PowerDevice(power),//virtual base classes are always created before non-virtual base classes, which ensures all bases get created before their derived classes.
		Printer2(printer, power + 1),
		Scanner2(scanner, power + 2)
	{//Printer2 and Scanner2's assignment for PowerDevice will be ignored
		report_func();
		this->print();
		report_line();
	}
};

class Copier3: Printer2
{
public:
/*
PowerDevice::PowerDevice(int):0x7fffcf3982d8
Printer2::Printer2(int, int):0x7fffcf3982d0
Copier3::Copier3(int, int):0x7fffcf3982d0<---child copier3's this==1st parent Print2's this
void PowerDevice::print() 0x7fffcf3982d8: -1<---assign for PowerDevice in Printer2 is ignored
*/
	Copier3(int printer, int power):PowerDevice(-1),Printer2(printer, power)//Even in a signle inheritence:Copier3 is singly inherited from Printer2, and Printer2 is virtually inherited from PowerDevice, Copier3 is still responsible for creating PowerDevice
	{
		report_func();
		this->print();
		report_line();
	}
};

int main()
{
/*
PowerDevice::PowerDevice(int):0x7ffd6a4dfe76
Scanner::Scanner(int, int):0x7ffd6a4dfe76<--the above 2 has the same this for they are parent/child class
PowerDevice::PowerDevice(int):0x7ffd6a4dfe77<--whose this != 1st PowerDevice's object, there are 2 PowerDevice objects
Printer::Printer(int, int):0x7ffd6a4dfe77<--the above 2 has the same this for they are parent/child class
Copier::Copier(int, int, int):0x7ffd6a4dfe76<--multi inheritence, the child copier's this==its 1st parent's this
*/
	Copier copier(1, 2, 3);
/*
PowerDevice::PowerDevice(int):0x7ffeff759700<--there is only one instance of PowerDevice
Printer2::Printer2(int, int):0x7ffeff7596f0<--Printer2 ctor is before Scanner2 ctor as it is inherited by Copier2 before Scanner
Scanner2::Scanner2(int, int):0x7ffeff7596f8
Copier2::Copier2(int, int, int):0x7ffeff7596f0<--the child copier2's this==its 1st parent instance's this
void PowerDevice::print() 0x7ffeff759700: 3<--PowerDevice's value is 3, not 4 or 5 assigned by Printer2/Scanner2 ctor; Printer2,Scanner2's PowerDevice assignment is ignored
*/
	Copier2 copier2(2, 1, 3);
	Copier3 copier3(1, 2);
	return 0;
}
