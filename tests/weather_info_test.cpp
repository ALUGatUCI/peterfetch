#include "weather_info.hpp"

#include <string>
#include <sstream>
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "http_client.hpp"

TEST(WeatherInfo, BeginsUnpopulated) {
    WeatherInfo info;
    EXPECT_FALSE(info.populated);
}

TEST(WeatherInfo, Fetch) {
    WeatherInfo info;

    info.fetch();
    info.print(std::cout);
    EXPECT_TRUE(info.populated);
}
