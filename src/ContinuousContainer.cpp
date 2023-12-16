#include "ContinuousContainer.h"

namespace containers
{
	ContinuousContainer::ContinuousContainer(size_t capacity)
	{
		buffer.reserve(capacity);
	}

	size_t ContinuousContainer::size() const
	{
		return distances.size();
	}

	ContinuousContainer::~ContinuousContainer()
	{

	}
}
