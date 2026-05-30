#include "iter_utils.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using namespace std;

TEST(IterUtils, ZipEven) {
    vector<int> a { 1, 2, 3 };
    vector<int> b { 4, 5, 6 };

    ZipRange zipped = ZipRange { a, b };

    for_each(zipped.begin(), zipped.end(), [] (pair<int, int> value) {
        EXPECT_EQ(value.first, value.second - 3);
    });
}

TEST(IterUtils, ZipUnEven) {
    vector<int> a { 1, 2, 3, 4 };
    vector<int> b { 5, 6, 7 };
    vector<int> c { 1, 2, 3 };
    vector<int> d { 4, 5, 6, 7 };

    ZipRange zipped_left = ZipRange { a, b };
    ZipRange zipped_right = ZipRange { c, d };

    ranges::for_each(zipped_left, [] (pair<int, int> value) {
        EXPECT_EQ(value.first, value.second - 4);
    });

    ranges::for_each(zipped_right, [] (pair<int, int> value) {
        EXPECT_EQ(value.first, value.second - 3);
    });
}
