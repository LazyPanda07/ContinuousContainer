#include "ContinuousContainer.h"

namespace containers
{
	ContinuousContainer::ContinuousContainer(size_t capacity)
	{
		buffer.reserve(capacity);
	}

	ContinuousContainer::~ContinuousContainer()
	{

	}
}
