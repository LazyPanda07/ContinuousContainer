#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <string_view>

namespace containers
{
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
		};

	private:
		std::vector<uint8_t> buffer;
		std::vector<size_t> distances;

	public:
		ContinuousContainer(size_t capacity = 0);

		template<typename T, typename... Args>
		T& add(Args&&... args);

		template<typename ClassT, auto FunctionT, typename... Args>
		void call(Args&&... args);

		template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
		std::vector<ReturnT> call(Args&&... args);

		size_t size() const;

		template<typename T>
		const T& getValue(size_t index) const;

		template<typename T>
		T& getValue(size_t index);

		~ContinuousContainer();
	};
}

#include "ContinuousContainer.inl"
