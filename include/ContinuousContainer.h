#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <string_view>

#include "ContinuousContainerIterator.h"

namespace containers
{
	/**
	 * @brief Stores any object in continuous block of memory
	*/
	class ContinuousContainer
	{
	public:
		static inline constexpr std::string_view continuousContainerVersion = "0.1.0";

	private:
		struct Block
		{
			size_t size;
			uint8_t* data;

			template<typename ClassT, auto FunctionT, typename... Args>
			void call(Args&&... args);

			template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
			ReturnT call(Args&&... args);

			template<typename ClassT, auto FunctionT, typename... Args>
			void call(Args&&... args) const;

			template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
			ReturnT call(Args&&... args) const;
		};

		struct MetaInformation
		{
			size_t distance;
			size_t objectSize;
			std::function<void(void*)> destructor;
		};

	private:
		std::vector<uint8_t> buffer;
		std::vector<MetaInformation> meta;

	public:
		ContinuousContainer(size_t capacity = 0);

		template<typename T>
		ContinuousContainer(std::initializer_list<T> objects);

		template<typename T, typename... Args>
		T& add(Args&&... args);

		void remove(size_t index);

		template<typename ClassT, auto FunctionT, typename... Args>
		void call(Args&&... args);

		template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
		std::vector<ReturnT> call(Args&&... args);

		template<typename ClassT, auto FunctionT, typename... Args>
		void call(Args&&... args) const;

		template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
		std::vector<ReturnT> call(Args&&... args) const;

		size_t size() const;

		size_t sizeInBytes() const;

		template<typename T>
		const T& getValue(size_t index) const;

		template<typename T>
		T& getValue(size_t index);

		template<typename T>
		ContinuousContainerIterator<T> begin();

		template<typename T>
		ContinuousContainerIterator<T> end();

		template<typename T>
		ContinuousContainerIterator<T> begin() const;

		template<typename T>
		ContinuousContainerIterator<T> end() const;
		~ContinuousContainer();
	};
}

#include "ContinuousContainer.inl"
