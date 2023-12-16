#include "iterators/ConstContinuousContainerIterator.h"

namespace containers
{
	ConstContinuousContainerIterator::ConstContinuousContainerIterator(const uint8_t* buffer, size_t objectSize) :
		buffer(buffer),
		objectSize(objectSize)
	{

	}

	ConstContinuousContainerIterator& ConstContinuousContainerIterator::operator*()
	{
		return *this;
	}

	const ConstContinuousContainerIterator& ConstContinuousContainerIterator::operator*() const
	{
		return *this;
	}

	bool ConstContinuousContainerIterator::operator==(const ConstContinuousContainerIterator& other) const
	{
		return buffer == other.buffer;
	}

	bool ConstContinuousContainerIterator::operator!=(const ConstContinuousContainerIterator& other) const
	{
		return !((*this) == other);
	}

	ConstContinuousContainerIterator& ConstContinuousContainerIterator::operator++()
	{
		buffer += objectSize + sizeof(size_t);

		return *this;
	}

	ConstContinuousContainerIterator ConstContinuousContainerIterator::operator++(int)
	{
		ConstContinuousContainerIterator it(*this);

		++(*this);

		return it;
	}
}
