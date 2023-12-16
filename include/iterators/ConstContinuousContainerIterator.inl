namespace containers
{
	template<typename T>
	const T& ConstContinuousContainerIterator::as() const
	{
		return *reinterpret_cast<const T*>(buffer);
	}
}
