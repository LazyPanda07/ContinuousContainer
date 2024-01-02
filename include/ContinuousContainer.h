#pragma once

#include <vector>
#include <functional>
#include <string_view>
#include <future>

namespace data_structures
{
	/**
	 * @brief Stores any object in continuous block of memory
	*/
	class ContinuousContainer
	{
	public:
		/**
		 * @brief Current version of ContinuousContainer
		*/
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

			template<typename ClassT, auto FunctionT, typename... Args>
			void call(Args&&... args) const;

			template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
			ReturnT call(Args&&... args) const;
		};

		struct MetaInformation
		{
			size_t distance;
			size_t objectSize;
			std::function<void(void*)> destructor;
		};

	private:
		// TODO: custom allocators support
		std::vector<uint8_t> buffer;
		std::vector<MetaInformation> meta;

	public:
		/**
		 * @brief Iterator
		*/
		class ContinuousContainerIterator
		{
		private:
			uint8_t* buffer;
			size_t index;

		public:
			ContinuousContainerIterator(uint8_t* buffer = nullptr, size_t index = 0);

			/**
			 * @brief Get value
			 * @tparam T
			 * @return
			*/
			template<typename T>
			T& as();

			/**
			 * @brief Get value
			 * @tparam T
			 * @return
			*/
			template<typename T>
			const T& as() const;

			ContinuousContainerIterator& operator*();

			const ContinuousContainerIterator& operator*() const;

			bool operator==(const ContinuousContainerIterator& other) const;

			bool operator!=(const ContinuousContainerIterator& other) const;

			ContinuousContainerIterator& operator++();

			ContinuousContainerIterator operator++(int);

			~ContinuousContainerIterator() = default;

			friend class ContinuousContainer;
		};

		/**
		 * @brief Const iterator
		*/
		class ConstContinuousContainerIterator
		{
		private:
			const uint8_t* buffer;
			size_t index;

		public:
			ConstContinuousContainerIterator(const uint8_t* buffer = nullptr, size_t index = 0);

			/**
			 * @brief Get value
			 * @tparam T
			 * @return
			*/
			template<typename T>
			const T& as() const;

			ConstContinuousContainerIterator& operator*();

			const ConstContinuousContainerIterator& operator*() const;

			bool operator==(const ConstContinuousContainerIterator& other) const;

			bool operator!=(const ConstContinuousContainerIterator& other) const;

			ConstContinuousContainerIterator& operator++();

			ConstContinuousContainerIterator operator++(int);

			~ConstContinuousContainerIterator() = default;

			friend class ContinuousContainer;
		};

	public:
		/**
		 * @brief Create container with preallocated memory storage
		 * @param capacity
		*/
		ContinuousContainer(size_t capacity = 0);

		/**
		 * @brief Create container with objects
		 * @tparam T
		 * @param objects
		*/
		template<typename T>
		ContinuousContainer(std::initializer_list<T> objects);

		/**
		 * @brief Create object
		 * @tparam T
		 * @tparam ...Args
		 * @param ...args
		 * @return
		*/
		template<typename T, typename... Args>
		T& add(Args&&... args);

		/**
		 * @brief Create object
		 * @tparam T
		 * @tparam ...Args
		 * @param ...args
		 * @return
		*/
		template<typename T, typename... Args>
		T& push_back(Args&&... args);

		/**
		 * @brief Delete object at index
		 * @param index
		*/
		void remove(size_t index);

		/**
		 * @brief Delete object with iterator
		 * @param iterator
		*/
		void remove(const ContinuousContainerIterator& iterator);

		/**
		 * @brief Delete object with iterator
		 * @param iterator
		*/
		void remove(const ConstContinuousContainerIterator& iterator);

		/**
		 * @brief Create and insert object at index
		 * @tparam T
		 * @tparam ...Args
		 * @param index
		 * @param ...args
		 * @return
		*/
		template<typename T, typename... Args>
		T& insert(size_t index, Args&&... args);

		/**
		 * @brief Create and insert object with iterator
		 * @tparam T
		 * @tparam ...Args
		 * @param iterator
		 * @param ...args
		 * @return
		*/
		template<typename T, typename... Args>
		T& insert(const ContinuousContainerIterator& iterator, Args&&... args);

		/**
		 * @brief Create and insert object with iterator
		 * @tparam T
		 * @tparam ...Args
		 * @param iterator
		 * @param ...args
		 * @return
		*/
		template<typename T, typename... Args>
		T& insert(const ConstContinuousContainerIterator& iterator, Args&&... args);

		/**
		 * @brief Get first element
		 * @tparam T
		 * @return
		*/
		template<typename T>
		T& front();

		/**
		 * @brief Get first element
		 * @tparam T
		 * @return
		*/
		template<typename T>
		const T& front() const;

		/**
		 * @brief Get last element
		 * @tparam T
		 * @return
		*/
		template<typename T>
		T& back();

		/**
		 * @brief Get last element
		 * @tparam T
		 * @return
		*/
		template<typename T>
		const T& back() const;

		/**
		 * @brief Call class member function for each object
		 * @tparam ClassT
		 * @tparam ...Args
		 * @tparam FunctionT
		 * @param ...args
		*/
		template<typename ClassT, auto FunctionT, typename... Args>
		void call(Args&&... args) const;

		/**
		 * @brief Call class member function for each object with return value
		 * @tparam ClassT
		 * @tparam ReturnT
		 * @tparam ...Args
		 * @tparam FunctionT
		 * @param callback
		 * @param ...args
		 * @return
		*/
		template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
		void call(const std::function<void(ReturnT&&)>& callback, Args&&... args) const;

		/**
		 * @brief Call class member function for each object if predicate returns true
		 * @tparam ClassT
		 * @tparam ...Args
		 * @tparam FunctionT
		 * @param ...args
		*/
		template<typename ClassT, auto FunctionT, typename... Args>
		void callIf(const std::function<bool(const ClassT&)>& predicate, Args&&... args) const;

		/**
		 * @brief Call class member function for each object with return value if predicate returns true
		 * @tparam ClassT
		 * @tparam ReturnT
		 * @tparam ...Args
		 * @tparam FunctionT
		 * @param callback
		 * @param ...args
		 * @return
		*/
		template<typename ClassT, auto FunctionT, typename ReturnT, typename... Args>
		void callIf(const std::function<void(ReturnT&&)>& callback, const std::function<bool(const ClassT&)>& predicate, Args&&... args) const;

		/**
		 * @brief Find element by predicate
		 * @tparam T
		 * @param predicate
		 * @return
		*/
		template<typename T>
		T* find(const std::function<bool(const T&)>& predicate) const;

		/**
		 * @brief Returns amount of objects in container
		 * @return
		*/
		size_t size() const;

		/**
		 * @brief Returns size in bytes of container
		 * @return
		*/
		size_t sizeInBytes() const;

		/**
		 * @brief Get object from container with index check
		 * @tparam T
		 * @param index
		 * @return
		 * @exception std::out_of_range
		*/
		template<typename T>
		T& at(size_t index);

		/**
		 * @brief Get object from container with index check
		 * @tparam T
		 * @param index
		 * @return
		 * @exception std::out_of_range
		*/
		template<typename T>
		const T& at(size_t index) const;

		/**
		 * @brief Get object from container
		 * @tparam T
		 * @param index
		 * @return
		*/
		template<typename T>
		T& get(size_t index);

		/**
		 * @brief Get object from container
		 * @tparam T
		 * @param index
		 * @return
		*/
		template<typename T>
		const T& get(size_t index) const;

		ContinuousContainerIterator begin();

		ContinuousContainerIterator end();

		ConstContinuousContainerIterator begin() const;

		ConstContinuousContainerIterator end() const;

		~ContinuousContainer();
	};
}

#include "ContinuousContainer.inl"
