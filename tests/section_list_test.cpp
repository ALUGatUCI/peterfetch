#include "section.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

using namespace std;

TEST(SectionListTest, EmptyListIteration) {
    SectionList list { {} };
    EXPECT_EQ(list.begin(), list.end());
}

TEST(SectionListTest, SingeItemIteration) {
    SectionList list { { make_shared<BlankSection>(5) } };

    EXPECT_EQ(distance(list.begin(), list.end()), 5);
}

TEST(SectionListTest, HomogeneousIteration) {
    auto section = make_shared<BlankSection>(5);
    SectionList list { { section, section } };

    EXPECT_EQ(distance(list.begin(), list.end()), 10);

    for_each(list.begin(), list.end(), [] (SectionLine line) {
            EXPECT_EQ(line.value, "");
            });
}

TEST(SectionListTest, HeterogeneousIteration) {
    SectionList list { { make_shared<BlankSection>(5),
        make_shared<BlankSection>(10),
        make_shared<BlankSection>(13) } };

    EXPECT_EQ(distance(list.begin(), list.end()), 28);

    for_each(list.begin(), list.end(), [] (SectionLine line) {
            EXPECT_EQ(line.value, "");
            });
}

TEST(SectionListTest, IteratorSkipsEmpty) {
    SectionList list { { make_shared<BlankSection>(1),
        make_shared<BlankSection>(0),
        make_shared<BlankSection>(1) } };
    SectionList list2  { { make_shared<BlankSection>(0),
        make_shared<BlankSection>(1),
        make_shared<BlankSection>(2) } };

    EXPECT_EQ(distance(list.begin(), list.end()), 2);
    EXPECT_EQ(distance(list2.begin(), list2.end()), 3);
}
