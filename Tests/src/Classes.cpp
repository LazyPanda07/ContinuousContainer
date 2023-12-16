#include "Classes.h"

int BaseClass::getValue() const
{
	return 5;
}

Derived::Derived(const std::string& value) :
	value(value)
{

}

int Derived::getValue() const
{
	return std::stol(value);
}

AnotherDerived::AnotherDerived(double first, double second) :
	first(first),
	second(second)
{

}

int AnotherDerived::getValue() const
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
