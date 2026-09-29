#include "section.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

TEST(SectionListTest, EmptyListIteration)
{
    SectionList list {{}};
    EXPECT_EQ(list.begin(), list.end());
}

TEST(SectionListTest, SingeItemIteration)
{
    SectionList list {{std::make_shared<BlankSection>(5)}};

    EXPECT_EQ(std::distance(list.begin(), list.end()), 5);
}

TEST(SectionListTest, HomogeneousIteration)
{
    auto section = std::make_shared<BlankSection>(5);
    SectionList list {
        {section, section}
    };

    EXPECT_EQ(std::distance(list.begin(), list.end()), 10);

    std::for_each(
        list.begin(), list.end(),
        [](SectionLine line) { EXPECT_EQ(line.value, ""); }
    );
}

TEST(SectionListTest, HeterogeneousIteration)
{
    SectionList list {
        {std::make_shared<BlankSection>(5), std::make_shared<BlankSection>(10),
         std::make_shared<BlankSection>(13)}
    };

    EXPECT_EQ(std::distance(list.begin(), list.end()), 28);

    std::for_each(
        list.begin(), list.end(),
        [](SectionLine line) { EXPECT_EQ(line.value, ""); }
    );
}

TEST(SectionListTest, IteratorSkipsEmpty)
{
    SectionList list {
        {std::make_shared<BlankSection>(1), std::make_shared<BlankSection>(0),
         std::make_shared<BlankSection>(1)}
    };
    SectionList list2 {
        {std::make_shared<BlankSection>(0), std::make_shared<BlankSection>(1),
         std::make_shared<BlankSection>(2)}
    };

    EXPECT_EQ(std::distance(list.begin(), list.end()), 2);
    EXPECT_EQ(std::distance(list2.begin(), list2.end()), 3);
}
