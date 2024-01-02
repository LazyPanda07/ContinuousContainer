#include "gtest/gtest.h"

#include <algorithm>
#include <random>

#include "ContinuousContainer.h"
#include "Classes.h"

TEST(ContinuousContainer, ConstructorInitializerList)
{
	data_structures::ContinuousContainer container({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

	for (size_t i = 0; i < container.size(); i++)
	{
		ASSERT_TRUE(container.get<int>(i) == i);
	}
}

TEST(ContinuousContainer, Add)
{
	data_structures::ContinuousContainer container;
	size_t count = 10;

	for (size_t i = 0; i < count; i++)
	{
		container.add<size_t>(i);

		container.push_back<size_t>(i);
	}

	ASSERT_TRUE(container.size() == count * 2);
}

TEST(ContinuousContainer, Size)
{
	data_structures::ContinuousContainer container;

	container.add<BaseClass>();
	container.add<Derived>("123");
	container.add<AnotherDerived>(0.15, 0.85);

	ASSERT_TRUE(container.size() == 3);
}

TEST(ContinuousContainer, SizeInBytes)
{
	data_structures::ContinuousContainer container;

	container.add<BaseClass>();
	container.add<Derived>("123");
	container.add<AnotherDerived>(0.15, 0.85);

	ASSERT_TRUE(container.sizeInBytes() == sizeof(BaseClass) + sizeof(Derived) + sizeof(AnotherDerived) + sizeof(size_t) * 3);
}

TEST(ContinuousContainer, Getters)
{
	data_structures::ContinuousContainer container;

	container.add<BaseClass>();
	container.add<Derived>("123");
	container.add<AnotherDerived>(0.15, 0.85);

	ASSERT_TRUE(container.get<BaseClass>(0).get() == 5);
	ASSERT_TRUE(container.front<BaseClass>().get() == 5);
	ASSERT_TRUE(container.get<Derived>(1).get() == 123);
	ASSERT_TRUE(container.get<AnotherDerived>(2).get() == 1);
	ASSERT_TRUE(container.back<AnotherDerived>().get() == 1);
}

TEST(ContinuousContainer, LargeContainer)
{
	data_structures::ContinuousContainer container;
	std::vector<int> results;
	std::vector<int> containerResults;

	for (size_t i = 0; i < 10'000; i++)
	{
		results.push_back(container.add<BaseClass>().get());
	}

	for (size_t i = 0; i < 10'000; i++)
	{
		results.push_back(container.add<Derived>("1000").get());
	}

	for (size_t i = 0; i < 10'000; i++)
	{
		results.push_back(container.add<AnotherDerived>(0.5, 0.5).get());
	}

	container.call<BaseClass, &BaseClass::get, int>(containerResults);

	ASSERT_TRUE(std::ranges::equal(containerResults, results));
}

TEST(ContinuousContainer, Remove)
{
	data_structures::ContinuousContainer container;
	std::vector<int> results;
	std::vector<int> containerResults;

	for (size_t i = 0; i < 100; i++)
	{
		results.push_back(container.add<BaseClass>().get());
	}

	for (size_t i = 0; i < 100; i++)
	{
		results.push_back(container.add<Derived>("5").get());
	}

	for (size_t i = 0; i < 10; i++)
	{
		container.remove(i);
		container.remove(i + 10);

		results.erase(results.begin() + i);
		results.erase(results.begin() + i + 10);
	}

	container.call<BaseClass, &BaseClass::get, int>(containerResults);

	ASSERT_TRUE(std::ranges::equal(containerResults, results));
}

TEST(ContinuousContainer, Iterators)
{
	data_structures::ContinuousContainer container;
	std::mt19937 random;

	ASSERT_TRUE(container.begin() == container.end());

	for (size_t i = 0; i < 100; i++)
	{
		switch (random() % 3)
		{
		case 0:
			container.add<BaseClass>();

			break;

		case 1:
			container.add<Derived>("100");

			break;

		case 2:
			container.add<AnotherDerived>(1.0, 1.0);

			break;

		default:
			break;
		}
	}

	for (data_structures::ContinuousContainer::ContinuousContainerIterator it = container.begin(); it != container.end(); ++it)
	{
		int value = it.as<BaseClass>().get();

		ASSERT_TRUE(value == 5 || value == 100 || value == 2);
	}

	for (data_structures::ContinuousContainer::ConstContinuousContainerIterator it = const_cast<const data_structures::ContinuousContainer&>(container).begin();
		it != const_cast<const data_structures::ContinuousContainer&>(container).end();
		++it)
	{
		int value = it.as<BaseClass>().get();

		ASSERT_TRUE(value == 5 || value == 100 || value == 2);
	}

	for (const auto& it : container)
	{
		int value = it.as<BaseClass>().get();

		ASSERT_TRUE(value == 5 || value == 100 || value == 2);
	}

	for (auto& it : container)
	{
		int value = it.as<BaseClass>().get();

		ASSERT_TRUE(value == 5 || value == 100 || value == 2);
	}
}

TEST(ContinuousContainer, Insert)
{
	data_structures::ContinuousContainer container;

	container.add<BaseClass>();
	container.add<Derived>("123");
	container.add<AnotherDerived>(0.15, 0.85);

	container.insert<Derived>(1, "456");
	container.insert<BaseClass>(2);
	container.insert<AnotherDerived>(3, 1.5, 3.25);

	ASSERT_TRUE(container.get<BaseClass>(1).get() == 456);
	ASSERT_TRUE(container.get<BaseClass>(2).get() == 5);
	ASSERT_TRUE(container.get<BaseClass>(3).get() == 4);

	ASSERT_TRUE(container.get<BaseClass>(0).get() == 5);
	ASSERT_TRUE(container.get<BaseClass>(4).get() == 123);
	ASSERT_TRUE(container.get<BaseClass>(5).get() == 1);

	container.insert<AnotherDerived>(0, 5.5, 6.5);
	container.insert<BaseClass>(container.size() - 1);

	ASSERT_TRUE(container.front<BaseClass>().get() == 12);
	ASSERT_TRUE(container.get<BaseClass>(container.size() - 2).get() == 5);
}

TEST(ContinuousContainer, Call)
{
	data_structures::ContinuousContainer container;

	container.add<BaseClass>();
	container.add<Derived>("123");
	container.add<AnotherDerived>(0.15, 0.85);

	std::vector<int> result;
	
	container.call<BaseClass, &BaseClass::get, int>(result);

	ASSERT_TRUE(result.size() == 3);
}

TEST(ContinuousContainer, CallIf)
{
	data_structures::ContinuousContainer container;

	container.add<BaseClass>();
	container.add<Derived>("123");
	container.add<AnotherDerived>(0.15, 0.85);

	std::vector<int> result;
	
	container.callIf<BaseClass, &BaseClass::get, int>(result, [](const BaseClass& object) { return object.get() > 5; });

	ASSERT_TRUE(result.size() == 1);
}

TEST(ContinuousContainer, Speed)
{
#ifndef NDEBUG
	return;
#endif

	double first = 0.0;
	double second = 0.0;
	size_t firstResult = 0;
	size_t secondResult = 0;
	size_t runs = 10'000;

	{
		std::vector<std::unique_ptr<BaseClass>> container;

		for (size_t i = 0; i < 500'000; i++)
		{
			container.push_back(std::make_unique<BaseClass>());
			container.push_back(std::make_unique<AnotherDerived>(static_cast<double>(i), static_cast<double>(i * 2)));
		}

		{
			Timer timer(first);

			for (size_t i = 0; i < runs; i++)
			{
				for (const std::unique_ptr<BaseClass>& value : container)
				{
					value->increase(firstResult);
				}
			}
		}
	}

	{
		data_structures::ContinuousContainer container;

		for (size_t i = 0; i < 500'000; i++)
		{
			container.push_back<BaseClass>();
			container.push_back<AnotherDerived>(static_cast<double>(i), static_cast<double>(i * 2));
		}

		{
			Timer timer(second);

			for (size_t i = 0; i < runs; i++)
			{
				container.call<BaseClass, &BaseClass::increase>(secondResult);
			}
		}
	}

	std::cout << "Pointers: " << first << " seconds" << std::endl << "ContinuousContainer: " << second << " seconds" << std::endl;

	ASSERT_TRUE(firstResult == secondResult);
	ASSERT_TRUE(first > second);
}

TEST(ContinuousContainer, SpeedWithReturnValue)
{
#ifndef NDEBUG
	return;
#endif

	double first = 0.0;
	double second = 0.0;
	size_t firstResult = 0;
	size_t secondResult = 0;
	size_t runs = 10'000;

	{
		std::vector<std::unique_ptr<BaseClass>> container;

		for (size_t i = 0; i < 500'000; i++)
		{
			container.push_back(std::make_unique<BaseClass>());
			container.push_back(std::make_unique<AnotherDerived>(static_cast<double>(i), static_cast<double>(i * 2)));
		}

		{
			Timer timer(first);

			for (size_t i = 0; i < runs; i++)
			{
				for (size_t j = 0; j < container.size(); j++)
				{
					firstResult += container[j]->get();
				}
			}
		}
	}

	{
		data_structures::ContinuousContainer container;

		for (size_t i = 0; i < 500'000; i++)
		{
			container.push_back<BaseClass>();
			container.push_back<AnotherDerived>(static_cast<double>(i), static_cast<double>(i * 2));
		}

		{
			std::vector<int> temp;

			temp.reserve(container.size());

			Timer timer(second);

			for (size_t i = 0; i < runs; i++)
			{
				container.call<BaseClass, &BaseClass::get, int>(temp);

				for (int value : temp)
				{
					secondResult += value;
				}
			}
		}
	}

	std::cout << "Pointers: " << first << " seconds" << std::endl << "ContinuousContainer: " << second << " seconds" << std::endl;

	ASSERT_TRUE(firstResult == secondResult);
	ASSERT_TRUE(first > second);
}

TEST(ContinuousContainer, Destructor)
{
	int result = 0;

	{
		data_structures::ContinuousContainer container;

		for (size_t i = 0; i < 10; i++)
		{
			container.add<Accumulated>(result, 1);
		}
	}

	ASSERT_TRUE(result == 10);
}
