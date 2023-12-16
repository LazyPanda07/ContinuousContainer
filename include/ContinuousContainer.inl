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

	template<typename ClassT, auto FunctionT, typename... Args>
	void ContinuousContainer::Block::call(Args&&... args) const
	{
		(*reinterpret_cast<const ClassT*>(&data).*FunctionT)(std::forward<Args>(args)...);
	}

	template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
	ReturnT ContinuousContainer::Block::call(Args&&... args) const
	{
		return (*reinterpret_cast<const ClassT*>(&data).*FunctionT)(std::forward<Args>(args)...);
	}

	template<typename T>
	ContinuousContainer::ContinuousContainer(std::initializer_list<T> objects)
	{
		for (const T& object : objects)
		{
			this->add(object);
		}
	}

	template<typename T, typename... Args>
	T& ContinuousContainer::add(Args&&... args)
	{
		size_t objectSize = sizeof(T);
		size_t start = buffer.size();

		meta.emplace_back
		(
			meta.empty() ? 0 : buffer.size(),
			objectSize,
			[](void* ptr) { std::destroy_at<T>(reinterpret_cast<T*>(ptr)); }
		);

		buffer.resize(buffer.size() + objectSize + sizeof(size_t));

		uint8_t* data = buffer.data() + start + sizeof(size_t);

		memcpy(data - sizeof(size_t), &objectSize, sizeof(objectSize));

		return *(new (data) T(std::forward<Args>(args)...));
	}

	template<typename ClassT, auto FunctionT, typename... Args>
	void ContinuousContainer::call(Args&&... args)
	{
		for (const auto& [distance, objectSize, destructor] : meta)
		{
			Block& block = *reinterpret_cast<Block*>(buffer.data() + distance);

			block.call<ClassT, FunctionT>(std::forward<Args>(args)...);
		}
	}

	template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
	std::vector<ReturnT> ContinuousContainer::call(Args&&... args)
	{
		std::vector<ReturnT> result;

		result.reserve(buffer.size());

		for (const auto& [distance, objectSize, destructor] : meta)
		{
			Block& block = *reinterpret_cast<Block*>(buffer.data() + distance);

			result.push_back(block.call<ClassT, FunctionT, ReturnT>(std::forward<Args>(args)...));
		}

		return result;
	}

	template<typename ClassT, auto FunctionT, typename... Args>
	void ContinuousContainer::call(Args&&... args) const
	{
		for (const auto& [distance, objectSize, destructor] : meta)
		{
			const Block& block = *reinterpret_cast<const Block*>(buffer.data() + distance);

			block.call<ClassT, FunctionT>(std::forward<Args>(args)...);
		}
	}

	template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
	std::vector<ReturnT> ContinuousContainer::call(Args&&... args) const
	{
		std::vector<ReturnT> result;

		result.reserve(buffer.size());

		for (const auto& [distance, objectSize, destructor] : meta)
		{
			const Block& block = *reinterpret_cast<const Block*>(buffer.data() + distance);

			result.push_back(block.call<ClassT, FunctionT, ReturnT>(std::forward<Args>(args)...));
		}

		return result;
	}

	template<typename T>
	inline const T& ContinuousContainer::getValue(size_t index) const
	{
		const Block& block = *reinterpret_cast<const Block*>(buffer.data() + meta[index].distance);

		return *reinterpret_cast<const T*>(&block.data);
	}

	template<typename T>
	inline T& ContinuousContainer::getValue(size_t index)
	{
		Block& block = *reinterpret_cast<Block*>(buffer.data() + meta[index].distance);

		return *reinterpret_cast<T*>(&block.data);
	}
}
