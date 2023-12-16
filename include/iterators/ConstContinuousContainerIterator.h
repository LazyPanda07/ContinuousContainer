#pragma once

#include <cstdint>

namespace containers
{
	class ConstContinuousContainerIterator
	{
	private:
		const uint8_t* buffer;
		size_t objectSize;

	public:
		ConstContinuousContainerIterator(const uint8_t* buffer = nullptr, size_t objectSize = 0);

		template<typename T>
		const T& as() const;

		ConstContinuousContainerIterator& operator*();

		const ConstContinuousContainerIterator& operator*() const;

		bool operator==(const ConstContinuousContainerIterator& other) const;

		bool operator!=(const ConstContinuousContainerIterator& other) const;

		ConstContinuousContainerIterator& operator++();

		ConstContinuousContainerIterator operator++(int);

		~ConstContinuousContainerIterator() = default;
	};
}

#include "ConstContinuousContainerIterator.inl"
