namespace data_structures
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
	T& ContinuousContainer::ContinuousContainerIterator::as()
	{
		return *reinterpret_cast<T*>(buffer + sizeof(size_t));
	}

	template<typename T>
	const T& ContinuousContainer::ContinuousContainerIterator::as() const
	{
		return *reinterpret_cast<const T*>(buffer + sizeof(size_t));
	}

	template<typename T>
	const T& ContinuousContainer::ConstContinuousContainerIterator::as() const
	{
		return *reinterpret_cast<const T*>(buffer + sizeof(size_t));
	}

	template<typename T>
	ContinuousContainer::ContinuousContainer(std::initializer_list<T> objects)
	{
		for (const T& object : objects)
		{
			this->add<T>(object);
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

		buffer.resize(buffer.size() + sizeof(size_t) + objectSize);

		uint8_t* data = buffer.data() + start + sizeof(size_t);

		memcpy(data - sizeof(size_t), &objectSize, sizeof(objectSize));

		return *(new (data) T(std::forward<Args>(args)...));
	}

	template<typename T, typename... Args>
	T& ContinuousContainer::insert(size_t index, Args&&... args)
	{
		std::vector<uint8_t> temp;
		size_t objectSize = sizeof(T);
		size_t distance = meta[index].distance;
		std::future<void> updateMeta;
		T* result = nullptr;

		temp.reserve(buffer.size() + sizeof(size_t) + objectSize);

		for (size_t i = 0; i < buffer.size(); i++)
		{
			if (i == distance)
			{
				MetaInformation information
				(
					distance,
					objectSize,
					[](void* ptr) { std::destroy_at<T>(reinterpret_cast<T*>(ptr)); }
				);

				meta.insert(meta.begin() + index, std::move(information));

				for (size_t j = 0; j < objectSize + sizeof(size_t); j++)
				{
					temp.emplace_back();
				}

				uint8_t* data = temp.data() + distance + sizeof(size_t);

				memcpy(data - sizeof(size_t), &objectSize, sizeof(objectSize));

				result = (new (data) T(std::forward<Args>(args)...));

				updateMeta = std::async(std::launch::async, [this, index, objectSize]()
					{
						for (size_t i = index + 1; i < meta.size(); i++)
						{
							meta[i].distance += objectSize + sizeof(size_t);
						}
					});
			}

			temp.push_back(buffer[i]);
		}

		buffer = std::move(temp);

		updateMeta.wait();

		return *result;
	}

	template<typename T, typename... Args>
	T& ContinuousContainer::insert(const ContinuousContainerIterator& iterator, Args&&... args)
	{
		return this->insert<T, Args...>(iterator.index, args...);
	}

	template<typename T, typename... Args>
	T& ContinuousContainer::insert(const ConstContinuousContainerIterator& iterator, Args&&... args)
	{
		return this->insert<T, Args...>(iterator.index, args...);
	}

	template<typename T>
	T& ContinuousContainer::front()
	{
		return *reinterpret_cast<T*>(buffer.data() + sizeof(size_t));
	}

	template<typename T>
	const T& ContinuousContainer::front() const
	{
		return *reinterpret_cast<const T*>(buffer.data() + sizeof(size_t));
	}

	template<typename T>
	T& ContinuousContainer::back()
	{
		return *reinterpret_cast<T*>(buffer.data() + meta.back().distance + sizeof(size_t));
	}

	template<typename T>
	const T& ContinuousContainer::back() const
	{
		return *reinterpret_cast<const T*>(buffer.data() + meta.back().distance + sizeof(size_t));
	}

	// TODO: refactor

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

	template<typename ClassT, auto FunctionT, typename... Args>
	void ContinuousContainer::callIf(const std::function<bool(const ClassT&)>& predicate, Args&&... args)
	{
		for (const auto& [distance, objectSize, destructor] : meta)
		{
			Block& block = *reinterpret_cast<Block*>(buffer.data() + distance);

			if (predicate(*reinterpret_cast<const ClassT*>(&block.data)))
			{
				block.call<ClassT, FunctionT>(std::forward<Args>(args)...);
			}
		}
	}

	template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
	std::vector<ReturnT> ContinuousContainer::callIf(const std::function<bool(const ClassT&)>& predicate, Args&&... args)
	{
		std::vector<ReturnT> result;

		result.reserve(buffer.size());

		for (const auto& [distance, objectSize, destructor] : meta)
		{
			Block& block = *reinterpret_cast<Block*>(buffer.data() + distance);

			if (predicate(*reinterpret_cast<const ClassT*>(&block.data)))
			{
				result.push_back(block.call<ClassT, FunctionT, ReturnT>(std::forward<Args>(args)...));
			}
		}

		return result;
	}

	template<typename ClassT, auto FunctionT, typename... Args>
	void ContinuousContainer::callIf(const std::function<bool(const ClassT&)>& predicate, Args&&... args) const
	{
		for (const auto& [distance, objectSize, destructor] : meta)
		{
			const Block& block = *reinterpret_cast<const Block*>(buffer.data() + distance);

			if (predicate(*reinterpret_cast<const ClassT*>(&block.data)))
			{
				block.call<ClassT, FunctionT>(std::forward<Args>(args)...);
			}
		}
	}

	template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
	std::vector<ReturnT> ContinuousContainer::callIf(const std::function<bool(const ClassT&)>& predicate, Args&&... args) const
	{
		std::vector<ReturnT> result;

		result.reserve(buffer.size());

		for (const auto& [distance, objectSize, destructor] : meta)
		{
			const Block& block = *reinterpret_cast<const Block*>(buffer.data() + distance);

			if (predicate(*reinterpret_cast<const ClassT*>(&block.data)))
			{
				result.push_back(block.call<ClassT, FunctionT, ReturnT>(std::forward<Args>(args)...));
			}
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
