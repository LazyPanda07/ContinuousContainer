#include "ContinuousContainer.h"

namespace containers
{
	ContinuousContainer::ContinuousContainer(size_t capacity) :
		size(0)
	{
		buffer.reserve(capacity);
	}

	ContinuousContainer::~ContinuousContainer()
	{

	}
}
