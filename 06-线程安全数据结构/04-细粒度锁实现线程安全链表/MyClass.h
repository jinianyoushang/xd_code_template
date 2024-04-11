#pragma once

#include <iostream>

class MyClass
{
public:
	MyClass(int i) :_data(i) {}

	friend std::ostream& operator << (std::ostream& os, const MyClass& mc) {
		os << mc._data;
		return os;
	}

	int GetData() const
	{
		return _data;
	}

private:
	int _data;
};