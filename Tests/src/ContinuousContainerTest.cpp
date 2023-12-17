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
        ASSERT_TRUE(container.getValue<int>(i) == i);
    }
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

    ASSERT_TRUE(container.getValue<BaseClass>(0).getValue() == 5);
    ASSERT_TRUE(container.front<BaseClass>().getValue() == 5);
    ASSERT_TRUE(container.getValue<Derived>(1).getValue() == 123);
    ASSERT_TRUE(container.getValue<AnotherDerived>(2).getValue() == 1);
    ASSERT_TRUE(container.back<AnotherDerived>().getValue() == 1);
}

TEST(ContinuousContainer, LargeContainer)
{
    data_structures::ContinuousContainer container;
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
    data_structures::ContinuousContainer container;
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
        int value = it.as<BaseClass>().getValue();

        ASSERT_TRUE(value == 5 || value == 100 || value == 2);
    }

    for (data_structures::ContinuousContainer::ConstContinuousContainerIterator it = const_cast<const data_structures::ContinuousContainer&>(container).begin();
        it != const_cast<const data_structures::ContinuousContainer&>(container).end();
        ++it)
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

TEST(ContinuousContainer, Insert)
{
    data_structures::ContinuousContainer container;

    container.add<BaseClass>();
    container.add<Derived>("123");
    container.add<AnotherDerived>(0.15, 0.85);

    container.insert<Derived>(1, "456");
    container.insert<BaseClass>(2);
    container.insert<AnotherDerived>(3, 1.5, 3.25);

    ASSERT_TRUE(container.getValue<BaseClass>(1).getValue() == 456);
    ASSERT_TRUE(container.getValue<BaseClass>(2).getValue() == 5);
    ASSERT_TRUE(container.getValue<BaseClass>(3).getValue() == 4);

    ASSERT_TRUE(container.getValue<BaseClass>(0).getValue() == 5);
    ASSERT_TRUE(container.getValue<BaseClass>(4).getValue() == 123);
    ASSERT_TRUE(container.getValue<BaseClass>(5).getValue() == 1);

    container.insert<AnotherDerived>(0, 5.5, 6.5);
    container.insert<BaseClass>(container.size() - 1);

    ASSERT_TRUE(container.front<BaseClass>().getValue() == 5);
    ASSERT_TRUE(container.back<BaseClass>().getValue() == 12);
}

TEST(ContinuousContainer, Speed)
{
    // TODO: speed test


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
