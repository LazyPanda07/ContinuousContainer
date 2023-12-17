#pragma once

#include <string>
#include <chrono>

class BaseClass
{
public:
	BaseClass() = default;

	virtual int get() const;

	virtual ~BaseClass() = default;
};

class Derived : public BaseClass
{
private:
	std::string value;

public:
	Derived(const std::string& value);

	int get() const override;

	~Derived() = default;
};

class AnotherDerived : public BaseClass
{
private:
	double first;
	double second;

public:
	AnotherDerived(double first, double second);

	int get() const override;

	~AnotherDerived() = default;
};

class Accumulated
{
private:
	int& out;
	int value;

public:
	Accumulated(int& out, int value);

	~Accumulated();
};

class Timer
{
private:
	double& out;
	std::chrono::high_resolution_clock::time_point start;

public:
	Timer(double& out);

	~Timer();
};
