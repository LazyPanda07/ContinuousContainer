#pragma once

#include <vector>
#include <cstdint>
#include <functional>

namespace containers
{
	class ContinuousContainer
	{
	private:
		struct Block
		{
			size_t size;
			uint8_t* data;

			template<typename ClassT, auto FunctionT, typename... Args>
			void call(Args&&... args);

			template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
			ReturnT call(Args&&... args);
		};

	private:
		std::vector<uint8_t> buffer;
		std::vector<size_t> distances;
		size_t size;

	public:
		ContinuousContainer(size_t capacity = 0);

		template<typename T, typename... Args>
		T& add(Args&&... args);

		template<typename ClassT, auto FunctionT, typename... Args>
		void call(Args&&... args);

		template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
		std::vector<ReturnT> call(Args&&... args);

		template<typename T>
		const T& getValue(size_t index) const;

		template<typename T>
		T& getValue(size_t index);

		~ContinuousContainer();
	};
}

#include "ContinuousContainer.inl"
