namespace containers
{
	template<typename ClassT, auto FunctionT, typename... Args>
	void ContinuousContainer::Block::call(Args&&... args)
	{
		(*reinterpret_cast<ClassT*>(&data).*FunctionT)(forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	void ContinuousContainer::add(Args&&... args)
	{
		size_t size = sizeof(T);

		size_t start = buffer.size();
		buffer.resize(buffer.size() + size + sizeof(size_t));

		uint8_t* data = buffer.data() + start + sizeof(size_t);

		memcpy(data - sizeof(size_t), &size, sizeof(size));

		new (data) T(std::forward<Args>(args)...);
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
}
