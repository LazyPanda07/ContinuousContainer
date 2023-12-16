#include "ContinuousContainer.h"

#include <algorithm>

namespace containers
{
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
}
