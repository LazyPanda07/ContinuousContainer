#include "ContinuousContainer.h"

#include <algorithm>

namespace data_structures
{
	using ContinuousContainerIterator = ContinuousContainer::ContinuousContainerIterator;
	using ConstContinuousContainerIterator = ContinuousContainer::ConstContinuousContainerIterator;

	ContinuousContainerIterator::ContinuousContainerIterator(uint8_t* buffer, size_t index) :
		buffer(buffer),
		index(index)
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
		Block& block = *reinterpret_cast<Block*>(buffer);

		buffer += block.size + sizeof(size_t);
		index++;

		return *this;
	}

	ContinuousContainerIterator ContinuousContainerIterator::operator++(int)
	{
		ContinuousContainerIterator it(*this);

		++(*this);

		return it;
	}

	ConstContinuousContainerIterator::ConstContinuousContainerIterator(const uint8_t* buffer, size_t index) :
		buffer(buffer),
		index(index)
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
		const Block& block = *reinterpret_cast<const Block*>(buffer);

		buffer += block.size + sizeof(size_t);
		index++;

		return *this;
	}

	ConstContinuousContainerIterator ConstContinuousContainerIterator::operator++(int)
	{
		ConstContinuousContainerIterator it(*this);

		++(*this);

		return it;
	}

	ContinuousContainer::ContinuousContainer(size_t capacity)
	{
		buffer.reserve(capacity);
	}

	void ContinuousContainer::remove(size_t index)
	{
		size_t distance = meta[index].distance;
		size_t objectSize = meta[index].objectSize;
		Block& block = *reinterpret_cast<Block*>(buffer.data() + distance);

		meta[index].destructor(reinterpret_cast<void*>(&block.data));

		for (size_t i = index + 1; i < meta.size(); i++)
		{
			meta[i].distance -= objectSize;
		}

		buffer.erase(buffer.begin() + distance, buffer.begin() + distance + objectSize);

		meta.erase(meta.begin() + index);
	}

	void ContinuousContainer::remove(const ContinuousContainerIterator& iterator)
	{
		this->remove(iterator.index);
	}

	void ContinuousContainer::remove(const ConstContinuousContainerIterator& iterator)
	{
		this->remove(iterator.index);
	}

	size_t ContinuousContainer::size() const
	{
		return meta.size();
	}

	size_t ContinuousContainer::sizeInBytes() const
	{
		return buffer.size();
	}

	ContinuousContainer::~ContinuousContainer()
	{
		for (const auto& [distance, objectSize, destructor] : meta)
		{
			Block& block = *reinterpret_cast<Block*>(buffer.data() + distance);

			destructor(reinterpret_cast<void*>(&block.data));
		}
	}

	ContinuousContainerIterator ContinuousContainer::begin()
	{
		if (meta.empty())
		{
			return ContinuousContainerIterator();
		}

		return ContinuousContainerIterator(buffer.data());
	}

	ContinuousContainerIterator ContinuousContainer::end()
	{
		if (meta.empty())
		{
			return ContinuousContainerIterator();
		}

		return ContinuousContainerIterator(buffer.data() + buffer.size(), meta.size());
	}

	ConstContinuousContainerIterator ContinuousContainer::begin() const
	{
		if (meta.empty())
		{
			return ConstContinuousContainerIterator();
		}

		return ConstContinuousContainerIterator(buffer.data());
	}

	ConstContinuousContainerIterator ContinuousContainer::end() const
	{
		if (meta.empty())
		{
			return ConstContinuousContainerIterator();
		}

		return ConstContinuousContainerIterator(buffer.data() + buffer.size(), meta.size());
	}
}
