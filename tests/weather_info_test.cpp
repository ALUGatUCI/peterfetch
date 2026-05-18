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
    WeatherInfo info{WEATHER_BASE_URL,
                    33.6405,
                    -117.8389,
                    new HttpClient()};

    info.fetch();
    info.print(std::cout);
    EXPECT_TRUE(info.populated);
}
