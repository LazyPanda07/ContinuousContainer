#include "gtest/gtest.h"

#include "ContinuousContainer.h"
#include "Classes.h"

TEST(ContinuousContainer, Add)
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
    ASSERT_TRUE(container.getValue<BaseClass>(0).getValue() == 123);
    ASSERT_TRUE(container.getValue<BaseClass>(0).getValue() == 1);
}

TEST(ContinuousContainer, LargeContainer)
{
    containers::ContinuousContainer container;
    std::vector<int> results;

    for (size_t i = 0; i < 10'000; i++)
    {
        container.add<BaseClass>();

        results.push_back(5);
    }

    for (size_t i = 0; i < 10'000; i++)
    {
        container.add<Derived>("1000");

        results.push_back(1000);
    }

    for (size_t i = 0; i < 10'000; i++)
    {
        container.add<AnotherDerived>(0.5, 0.5);

        results.push_back(1);
    }

    for (size_t i = 0; i < container.size(); i++)
    {
        ASSERT_TRUE(container.getValue<BaseClass>(i).getValue() == results[i]);
    }
}
