#include <stdio.h>

class Fish {
public:
	virtual void Swim();
};

class Tuna: public Fish {
public:
	void Swim();
};

class Carp: public Fish {
public:
	void Swim();	
};

void Fish::Swim()
{
	printf("%p base swim\n", this);
}

void Tuna::Swim()
{
	printf("%p Tuna swim\n", this);
}

void Carp::Swim()
{
	printf("%p Carp swim\n", this);
}

void MakeFishSwim(Fish aFish)
{
	aFish.Swim();
}

void MakeFishSwim2(Fish *aFish)
{
	aFish->Swim();
}

template<typename T>
class deepcopy_smart_ptr {
	T *object;
public:
	deepcopy_smart_ptr(T *p):object(p)
	{
	}
	deepcopy_smart_ptr(const deepcopy_smart_ptr &source)
	{
		object = new T(source);
	}
	~deepcopy_smart_ptr()
	{
		delete object;
	}
};
int main()
{
	Carp freshWaterFish;
	Tuna marineFish;
	//base swim twice
	MakeFishSwim(freshWaterFish);
	MakeFishSwim(marineFish);
	//derived swim
	MakeFishSwim2(&freshWaterFish);
	MakeFishSwim2(&marineFish);
	/*
	deepcopy_smart_ptr<Carp> freshWaterFish2(new Carp());
	MakeFishSwim(freshWaterFish2);
	deepcopy_smart_ptr<Tuna> marineFish2(new Tuna());
	MakeFishSwim(marineFish2);
	*/
	char *p = new char[4096];
	delete []p;
}