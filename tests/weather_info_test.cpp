#include "weather_info.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "http_client.hpp"

using ::testing::_;
using ::testing::Return;

class MockHttpClient : public HttpClient {
public:
    MOCK_METHOD(cpr::Response, get, (const cpr::Url& url, bool redirect),
                (override));
};

// First get(): the points lookup, which carries the hourly forecast URL.
constexpr const char *POINTS_RESPONSE = R"({
    "properties": {
        "forecastHourly": "https://api.weather.gov/gridpoints/SGX/39,58/forecast/hourly"
    }
})";

// Second get(): the hourly forecast itself.
constexpr const char *PROPER_RESPONSE = R"({
    "properties": {
        "periods": [
            {
                "number": 1,
                "temperature": 72,
                "temperatureUnit": "F",
                "probabilityOfPrecipitation": { "unitCode": "wmoUnit:percent", "value": 10 },
                "windSpeed": "5 mph",
                "windDirection": "NW",
                "relativeHumidity": { "unitCode": "wmoUnit:percent", "value": 55 }
            }
        ]
    }
})";

TEST(WeatherInfo, BeginsUnpopulated) {
    WeatherInfo info;
    EXPECT_FALSE(info.populated());
}

TEST(WeatherInfo, Fetch) {
    MockHttpClient mockClient;

    cpr::Response pointsResponse;
    pointsResponse.status_code = 200;
    pointsResponse.text = POINTS_RESPONSE;

    cpr::Response hourlyResponse;
    hourlyResponse.status_code = 200;
    hourlyResponse.text = PROPER_RESPONSE;

    EXPECT_CALL(mockClient, get(_, _))
        .WillOnce(Return(pointsResponse))
        .WillOnce(Return(hourlyResponse));

    WeatherInfo info{WEATHER_BASE_URL, DEFAULT_LATITUDE, DEFAULT_LONGITUDE,
                     &mockClient};

    EXPECT_EQ(info.fetch(), WeatherFetchResult::OK);
    EXPECT_TRUE(info.populated());
    EXPECT_EQ(info.temperature(), 72);
    EXPECT_TRUE(info.isFahrenheit());
    EXPECT_EQ(info.precipitation(), 10);
    EXPECT_EQ(info.wind(), "5 mph NW");
    EXPECT_EQ(info.humidity(), 55);
}
