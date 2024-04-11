#pragma once
#include <iostream>

class MyClass
{
public:
	MyClass(int data):_data(data){}
	MyClass(const MyClass& mc):_data(mc._data){}
	MyClass(MyClass&& mc) :_data(mc._data)
	{

	}

	friend std::ostream& operator << (std::ostream& os, const MyClass& mc)
	{
		os << mc._data;
		return os;
	}



private:
	int _data;
};