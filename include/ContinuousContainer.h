#pragma once

#include <vector>
#include <cstdint>

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
		};

	private:
		std::vector<uint8_t> buffer;

	public:
		ContinuousContainer(size_t capacity = 0);

		template<typename T, typename... Args>
		void add(Args&&... args);

		template<typename ClassT, auto FunctionT, typename... Args>
		void call(Args&&... args);

		~ContinuousContainer();
	};
}

#include "ContinuousContainer.inl"
