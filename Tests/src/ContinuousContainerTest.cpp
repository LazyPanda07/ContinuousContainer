#include "gtest/gtest.h"

#include <algorithm>
#include <random>

#include "ContinuousContainer.h"
#include "Classes.h"

TEST(ContinuousContainer, ConstructorInitializerList)
{
    containers::ContinuousContainer container({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    for (size_t i = 0; i < container.size(); i++)
    {
        ASSERT_TRUE(container.getValue<int>(i) == i);
    }
}

TEST(ContinuousContainer, Size)
{
    containers::ContinuousContainer container;

    container.add<BaseClass>();
    container.add<Derived>("123");
    container.add<AnotherDerived>(0.15, 0.85);

    ASSERT_TRUE(container.size() == 3);
}

TEST(ContinuousContainer, SizeInBytes)
{
    containers::ContinuousContainer container;

    container.add<BaseClass>();
    container.add<Derived>("123");
    container.add<AnotherDerived>(0.15, 0.85);

    ASSERT_TRUE(container.sizeInBytes() == sizeof(BaseClass) + sizeof(Derived) + sizeof(AnotherDerived));
}

TEST(ContinuousContainer, Getters)
{
    containers::ContinuousContainer container;

    container.add<BaseClass>();
    container.add<Derived>("123");
    container.add<AnotherDerived>(0.15, 0.85);

    ASSERT_TRUE(container.getValue<BaseClass>(0).getValue() == 5);
    ASSERT_TRUE(container.getValue<Derived>(1).getValue() == 123);
    ASSERT_TRUE(container.getValue<AnotherDerived>(2).getValue() == 1);
}

TEST(ContinuousContainer, LargeContainer)
{
    containers::ContinuousContainer container;
    std::vector<int> results;

    for (size_t i = 0; i < 10'000; i++)
    {
        results.push_back(container.add<BaseClass>().getValue());
    }

    for (size_t i = 0; i < 10'000; i++)
    {
        results.push_back(container.add<Derived>("1000").getValue());
    }

    for (size_t i = 0; i < 10'000; i++)
    {
        results.push_back(container.add<AnotherDerived>(0.5, 0.5).getValue());
    }

    ASSERT_TRUE(std::ranges::equal(container.call<BaseClass, &BaseClass::getValue, int>(), results));
}

TEST(ContinuousContainer, Remove)
{
    containers::ContinuousContainer container;
    std::vector<int> results;

    for (size_t i = 0; i < 100; i++)
    {
        results.push_back(container.add<BaseClass>().getValue());
    }

    for (size_t i = 0; i < 100; i++)
    {
        results.push_back(container.add<Derived>("1000").getValue());
    }

    for (size_t i = 0; i < 10; i++)
    {
        container.remove(i);
        container.remove(i + 100);

        results.erase(results.begin() + i);
        results.erase(results.begin() + i + 100);
    }

    ASSERT_TRUE(std::ranges::equal(container.call<BaseClass, &BaseClass::getValue, int>(), results));
}

TEST(ContinuousContainer, Iterators)
{
    containers::ContinuousContainer container;
    std::mt19937 random;

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

    for (containers::ContinuousContainerIterator it = container.begin(); it != container.end(); ++it)
    {
        int value = it.as<BaseClass>().getValue();

        ASSERT_TRUE(value == 5 || value == 100 || value == 2);
    }

    for (const auto& it : container)
    {
        int value = it.as<BaseClass>().getValue();

        ASSERT_TRUE(value == 5 || value == 100 || value == 2);
    }

    for (auto& it : container)
    {
        int value = it.as<BaseClass>().getValue();

        ASSERT_TRUE(value == 5 || value == 100 || value == 2);
    }
}

TEST(ContinuousContainer, Destructor)
{
    int result = 0;

    {
        containers::ContinuousContainer container;

        for (size_t i = 0; i < 10; i++)
        {
            container.add<Accumulated>(result, 1);
        }
    }

    ASSERT_TRUE(result == 10);
}
