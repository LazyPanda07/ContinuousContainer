#include "Classes.h"

int BaseClass::get() const
{
	return 5;
}

void BaseClass::increase(size_t& value) const
{
	value += 5;
}

Derived::Derived(const std::string& value) :
	value(value)
{

}

int Derived::get() const
{
	return std::stol(value);
}

void Derived::increase(size_t& value) const
{
	value += std::stol(this->value);
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

void AnotherDerived::increase(size_t& value) const
{
	value += first + second;
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

Timer::Timer(double& out) :
	out(out),
	start(std::chrono::high_resolution_clock::now())
{

}

Timer::~Timer()
{
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

	double result = static_cast<double>((end - start).count());

	result /= std::chrono::high_resolution_clock::period::den;

	out += result;
}
