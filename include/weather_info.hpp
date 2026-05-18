#ifndef DIRECTORY_INFO_HPP
#define DIRECTORY_INFO_HPP

#include <string>

#include <nlohmann/json.hpp>

#include "http_client.hpp"

constexpr const char *WEATHER_BASE_URL = "https://api.weather.gov/";

enum class WeatherFetchResult {
    OK,
    INVALID_RESPONSE,
    DOESNT_EXIST,
    PARSING_FAILED,
};

class WeatherInfo {
public:
    WeatherInfo(const std::string& url = WEATHER_BASE_URL,
                const double latitude = 33.6405,
                const double longitude = -117.8389,
                HttpClient *client = new HttpClient());

    /* Fetch weather data and populate the weather info */
    WeatherFetchResult fetch();

    void print(std::ostream &out) const;

    const std::string &temperature() { return m_temperature; }
    const std::string &precipitation() { return m_precipitation; }
    const std::string &wind() { return m_wind; }
    const std::string &humidity() { return m_humidity; }

    bool populated;

private:
    std::string m_url;
    std::string m_temperature;
    std::string m_precipitation;
    std::string m_wind;
    std::string m_humidity;
    HttpClient *m_client;

    std::string getHourlyForecastLink(const std::string& response);

    void populateFromHourlyForecast(const std::string& hourlyForecastResponse);
};

#endif // WEATHER_INFO_HPP
