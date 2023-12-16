#pragma once

#include <cstdint>

namespace containers
{
	class ContinuousContainerIterator
	{
	private:
		uint8_t* buffer;
		size_t objectSize;

	public:
		ContinuousContainerIterator(uint8_t* buffer = nullptr, size_t objectSize = 0);

		template<typename T>
		T& as();

		template<typename T>
		const T& as() const;

		ContinuousContainerIterator& operator*();

		const ContinuousContainerIterator& operator*() const;

		bool operator==(const ContinuousContainerIterator& other) const;

		bool operator!=(const ContinuousContainerIterator& other) const;

		ContinuousContainerIterator& operator++();

		ContinuousContainerIterator operator++(int);

		~ContinuousContainerIterator() = default;
	};
}

#include "ContinuousContainerIterator.inl"
