#include "iter_utils.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

TEST(IterUtils, ZipEven)
{
    std::vector<int> a {1, 2, 3};
    std::vector<int> b {4, 5, 6};

    ZipRange zipped = ZipRange {a, b};

    std::for_each(
        zipped.begin(), zipped.end(), [](std::pair<int, int> value)
        { EXPECT_EQ(value.first, value.second - 3); }
    );
}

TEST(IterUtils, ZipUnEven)
{
    std::vector<int> a {1, 2, 3, 4};
    std::vector<int> b {5, 6, 7};
    std::vector<int> c {1, 2, 3};
    std::vector<int> d {4, 5, 6, 7};

    ZipRange zipped_left = ZipRange {a, b};
    ZipRange zipped_right = ZipRange {c, d};

    std::ranges::for_each(
        zipped_left, [](std::pair<int, int> value)
        { EXPECT_EQ(value.first, value.second - 4); }
    );

    std::ranges::for_each(
        zipped_right, [](std::pair<int, int> value)
        { EXPECT_EQ(value.first, value.second - 3); }
    );
}

TEST(IterUtils, ZipLargeDifference)
{
    std::string left_small {"hi"};
    std::string left_large {"hello big world!"};
    std::vector<int> right_small {1, 2, 3, 4};
    std::vector<int> right_large {1, 2, 3, 4, 5, 6, 7};

    ZipRange zipped_left = ZipRange {left_large, right_small};
    ZipRange zipped_right = ZipRange {left_small, right_large};

    EXPECT_EQ(std::ranges::distance(zipped_left), 4);
    EXPECT_EQ(std::ranges::distance(zipped_right), 2);
}
