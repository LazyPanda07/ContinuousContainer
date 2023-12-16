namespace containers
{
	template<typename T, bool isConst>
	ContinuousContainerIterator<T, isConst>::ContinuousContainerIterator(uint8_t* buffer, size_t objectSize) :
		buffer(buffer),
		objectSize(objectSize)
	{

	}

	template<typename T, bool isConst>
	template<typename std::enable_if_t<!isConst>>
	T* ContinuousContainerIterator<T, isConst>::operator->()
	{
		return reinterpret_cast<T*>(buffer);
	}

	template<typename T, bool isConst>
	const T* ContinuousContainerIterator<T, isConst>::operator->() const
	{
		return reinterpret_cast<const T*>(buffer);
	}

	template<typename T, bool isConst>
	template<typename std::enable_if_t<!isConst>>
	T& ContinuousContainerIterator<T, isConst>::operator*()
	{
		return *reinterpret_cast<T*>(buffer);
	}

	template<typename T, bool isConst>
	const T& ContinuousContainerIterator<T, isConst>::operator*() const
	{
		return *reinterpret_cast<const T*>(buffer);
	}

	template<typename T, bool isConst>
	bool ContinuousContainerIterator<T, isConst>::operator==(const ContinuousContainerIterator& other) const
	{
		return buffer == other.buffer;
	}

	template<typename T, bool isConst>
	bool ContinuousContainerIterator<T, isConst>::operator!=(const ContinuousContainerIterator& other) const
	{
		return !((*this) == other);
	}

	template<typename T, bool isConst>
	ContinuousContainerIterator<T, isConst>& ContinuousContainerIterator<T, isConst>::operator++()
	{
		buffer += objectSize + sizeof(size_t);
		
		return *this;
	}

	template<typename T, bool isConst>
	ContinuousContainerIterator<T, isConst> ContinuousContainerIterator<T, isConst>::operator++(int)
	{
		ContinuousContainerIterator it(*this);

		++(*this);

		return it;
	}
}