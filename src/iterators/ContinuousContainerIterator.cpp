#include "iterators/ContinuousContainerIterator.h"

namespace containers
{
	ContinuousContainerIterator::ContinuousContainerIterator(uint8_t* buffer, size_t objectSize) :
		buffer(buffer),
		objectSize(objectSize)
	{

	}

	ContinuousContainerIterator& ContinuousContainerIterator::operator*()
	{
		return *this;
	}

	const ContinuousContainerIterator& ContinuousContainerIterator::operator*() const
	{
		return *this;
	}

	bool ContinuousContainerIterator::operator==(const ContinuousContainerIterator& other) const
	{
		return buffer == other.buffer;
	}

	bool ContinuousContainerIterator::operator!=(const ContinuousContainerIterator& other) const
	{
		return !((*this) == other);
	}

	ContinuousContainerIterator& ContinuousContainerIterator::operator++()
	{
		buffer += objectSize + sizeof(size_t);

		return *this;
	}

	ContinuousContainerIterator ContinuousContainerIterator::operator++(int)
	{
		ContinuousContainerIterator it(*this);

		++(*this);

		return it;
	}
}
