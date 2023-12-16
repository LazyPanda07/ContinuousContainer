namespace containers
{
	template<typename T>
	T& ContinuousContainerIterator::as()
	{
		return *reinterpret_cast<T*>(buffer);
	}

	template<typename T>
	const T& ContinuousContainerIterator::as() const
	{
		return *reinterpret_cast<const T*>(buffer);
	}
}
