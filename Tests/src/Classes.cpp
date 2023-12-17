#include "Classes.h"

int BaseClass::get() const
{
	return 5;
}

Derived::Derived(const std::string& value) :
	value(value)
{

}

int Derived::get() const
{
	return std::stol(value);
}

AnotherDerived::AnotherDerived(double first, double second) :
	first(first),
	second(second)
{

}

int AnotherDerived::get() const
{
	return static_cast<int>(first + second);
}

Accumulated::Accumulated(int& out, int value) :
	out(out),
	value(value)
{

}

Accumulated::~Accumulated()
{
	out += value;
}
