#pragma once

#include <string>

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
