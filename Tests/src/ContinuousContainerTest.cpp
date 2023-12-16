#include "gtest/gtest.h"

#include <algorithm>

#include "ContinuousContainer.h"
#include "Classes.h"

TEST(ContinuousContainer, Size)
{
    containers::ContinuousContainer container;

    container.add<BaseClass>();
    container.add<Derived>("123");
    container.add<AnotherDerived>(0.15, 0.85);

    ASSERT_TRUE(container.size() == 3);
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
