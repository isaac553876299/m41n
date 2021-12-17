
#include <iostream>

const struct contains_ {} contains;

template <typename T>
struct ContainsProxy
{
    ContainsProxy(const T& t): t_(t) {}
    const T& t_;
};

template <typename T>
ContainsProxy<T> operator<(const T& lhs, const contains_& rhs)
{
    return ContainsProxy<T>(lhs);
}

bool operator>(const ContainsProxy<const char*>& lhs, const char& rhs)
{
	for (int i = 0, size = sizeof(lhs); i < size; ++i)
	{
		if (lhs.t_[i] == rhs) return true;
	}
	return false;
}

class myclass
{
public:

	float x;
	const char* name;

	myclass(float _x = 0.0f, const char* _name = "none") : x(_x), name(_name) {}
	~myclass() {}

	/*
	http://courses.cms.caltech.edu/cs11/material/cpp/donnie/cpp-ops.html
	argument as const because we don't want to change that value
	reference is returned non-const to allow operator chaining //a=b=c //a+b+c //a+=b+=c
	return *this as we need reference and not pointer
	C++ converts instances to references, and vice-versa
	check for self-assignment
	binary arithmetic operators return const instances to avoid compiling madness like (a+b)=c
	don't want to change values? use const
	friend declaration appears in a class body and grants a function or another class access to private and protected members of the class where the friend declaration appears
	*/

	myclass& operator=(const myclass& m) { if (this != &m) x = m.x; return *this; }
	myclass& operator+=(const myclass& m) { x += m.x; return *this; }
	const myclass operator+(const myclass& m) const { return myclass(*this) += m; }
	myclass& operator++() { ++x; return *this; }
	myclass& operator++(int) { myclass m2 = *this; ++(*this); return m2; }
	myclass& operator>>=(myclass& m) { myclass m2 = m; m = *this; return *this = m2; }
	myclass& operator<<=(myclass& m) { myclass m2 = *this; *this = m; return m = m2; }
	bool operator==(const myclass& m) const { return x == m.x; }
	bool operator!=(const myclass& m) const { return !(*this == m); }
	friend std::ostream& operator<<(std::ostream& out, const myclass& m) { return out << " " << m.name << " (x: " << m.x << ") " << std::endl; }

};

#include "custom_operators.h"

int main()
{
	myclass a(1, "a");
	myclass b(2, "b");
	myclass c(3, "c");

	std::cout << a << b << c << std::endl;

	//std::cout << (a.name < contains>'b') << std::endl;

	int x = 0, y = 9000;
	std::cout << x << " " << y << std::endl;
	x swap y;
	std::cout << x << " " << y << std::endl;

	return 0;
}
