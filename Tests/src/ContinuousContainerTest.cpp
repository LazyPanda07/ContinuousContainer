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

    ASSERT_TRUE(std::ranges::equal(container.call<BaseClass, &BaseClass::get, int>(), results));
}

TEST(ContinuousContainer, Remove)
{
    data_structures::ContinuousContainer container;
    std::vector<int> results;

    for (size_t i = 0; i < 100; i++)
    {
        results.push_back(container.add<BaseClass>().get());
    }

    for (size_t i = 0; i < 100; i++)
    {
        results.push_back(container.add<Derived>("1000").get());
    }

    for (size_t i = 0; i < 10; i++)
    {
        container.remove(i);
        container.remove(i + 100);

        results.erase(results.begin() + i);
        results.erase(results.begin() + i + 100);
    }

    ASSERT_TRUE(std::ranges::equal(container.call<BaseClass, &BaseClass::get, int>(), results));
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

    std::vector<int> result = container.call<BaseClass, &BaseClass::get, int>();

    ASSERT_TRUE(result.size() == 3);
}

TEST(ContinuousContainer, CallIf)
{
    data_structures::ContinuousContainer container;

    container.add<BaseClass>();
    container.add<Derived>("123");
    container.add<AnotherDerived>(0.15, 0.85);

    std::vector<int> result = container.callIf<BaseClass, &BaseClass::get, int>([](const BaseClass& object) { return object.get() > 5; });

    ASSERT_TRUE(result.size() == 1);
}

TEST(ContinuousContainer, Speed)
{
    double first = 0.0;
    double second = 0.0;
    size_t result = 0;

    {
        std::vector<std::unique_ptr<BaseClass>> container;

        for (size_t i = 0; i < 100'000; i++)
        {
            container.push_back(std::make_unique<BaseClass>());
            container.push_back(std::make_unique<Derived>(std::to_string(i)));
            container.push_back(std::make_unique<AnotherDerived>(static_cast<double>(i), static_cast<double>(i * 2)));
        }

        Timer timer(first);

        for (size_t i = 0; i < 10'000; i++)
        {
            for (const auto& value : container)
            {
                result += value->get();
            }
        }
    }

    result = 0;

    {
        data_structures::ContinuousContainer container;

        for (size_t i = 0; i < 100'000; i++)
        {
            container.push_back<BaseClass>();
            container.push_back<Derived>(std::to_string(i));
            container.push_back<AnotherDerived>(static_cast<double>(i), static_cast<double>(i * 2));
        }

        Timer timer(second);

        for (size_t i = 0; i < 10'000; i++)
        {
            for (const auto& value : container)
            {
                result += value.as<BaseClass>().get();
            }
        }
    }
    
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
