namespace containers
{
	template<typename ClassT, auto FunctionT, typename... Args>
	inline void ContinuousContainer::Block::call(Args&&... args)
	{
		(*reinterpret_cast<ClassT*>(&data).*FunctionT)(std::forward<Args>(args)...);
	}

	template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
	inline ReturnT ContinuousContainer::Block::call(Args&&... args)
	{
		return (*reinterpret_cast<ClassT*>(&data).*FunctionT)(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	T& ContinuousContainer::add(Args&&... args)
	{
		size_t instanceSize = sizeof(T);
		size_t start = buffer.size();

		distances.push_back(distances.empty() ? 0 : buffer.size());

		buffer.resize(buffer.size() + instanceSize + sizeof(size_t));

		uint8_t* data = buffer.data() + start + sizeof(size_t);

		memcpy(data - sizeof(size_t), &instanceSize, sizeof(instanceSize));

		size++;

		return *(new (data) T(std::forward<Args>(args)...));
	}

	template<typename ClassT, auto FunctionT, typename... Args>
	void ContinuousContainer::call(Args&&... args)
	{
		for (size_t i = 0; i < buffer.size();)
		{
			Block& block = *reinterpret_cast<Block*>(buffer.data() + i);

			block.call<ClassT, FunctionT>(std::forward<Args>(args)...);

			i += sizeof(size_t) + block.size;
		}
	}

	template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
	std::vector<ReturnT> ContinuousContainer::call(Args&&... args)
	{
		std::vector<ReturnT> result;

		result.reserve(size);

		for (size_t i = 0; i < buffer.size();)
		{
			Block& block = *reinterpret_cast<Block*>(buffer.data() + i);

			result.push_back(block.call<ClassT, FunctionT, ReturnT>(std::forward<Args>(args)...));

			i += sizeof(size_t) + block.size;
		}

		return result;
	}

	template<typename T>
	inline const T& ContinuousContainer::getValue(size_t index) const
	{
		const Block& block = *reinterpret_cast<const Block*>(buffer.data() + distances[index]);

		return *reinterpret_cast<const T*>(&block.data);
	}

	template<typename T>
	inline T& ContinuousContainer::getValue(size_t index)
	{
		Block& block = *reinterpret_cast<Block*>(buffer.data() + distances[index]);

		return *reinterpret_cast<T*>(&block.data);
	}
}
