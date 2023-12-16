#pragma once

#include <vector>

namespace containers
{
	template<typename T, bool isConst = false>
	class ContinuousContainerIterator
	{
	private:
		uint8_t* buffer;
		size_t objectSize;

	public:
		ContinuousContainerIterator(uint8_t* buffer = nullptr, size_t objectSize = 0);

		template<typename std::enable_if_t<!isConst>>
		T* operator->();

		const T* operator->() const;

		template<typename std::enable_if_t<!isConst>>
		T& operator*();

		const T& operator*() const;

		bool operator==(const ContinuousContainerIterator& other) const;

		bool operator!=(const ContinuousContainerIterator& other) const;

		ContinuousContainerIterator& operator++();

		ContinuousContainerIterator operator++(int);

		ContinuousContainerIterator() = default;
	};
}

#include "ContinuousContainerIterator.inl"
