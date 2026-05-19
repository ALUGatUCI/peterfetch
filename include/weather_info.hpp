#ifndef WEATHER_INFO_HPP
#define WEATHER_INFO_HPP

#include <string>

#include <nlohmann/json.hpp>

#include "http_client.hpp"

constexpr const char *WEATHER_BASE_URL = "https://api.weather.gov/";
constexpr const double DEFAULT_LATITUDE = 33.6405; // Latitude of UC Irvine
constexpr const double DEFAULT_LONGITUDE = -117.8389; // Longitude of UC Irvine

enum class WeatherFetchResult {
    OK,
    INVALID_RESPONSE,
    DOESNT_EXIST,
    PARSING_FAILED,
};

class WeatherInfo {
public:
    WeatherInfo(const std::string& url = WEATHER_BASE_URL,
                const double latitude = DEFAULT_LATITUDE,
                const double longitude = DEFAULT_LONGITUDE,
                HttpClient *client = new HttpClient());

    /* Fetch weather data and populate the weather info */
    WeatherFetchResult fetch();

    void print(std::ostream &out) const;

    const int &temperature() const { return m_temperature; }
    const bool &isFahrenheit() const { return m_isFahrenheit; }
    const int &precipitation() const { return m_precipitation; }
    const std::string &wind() const { return m_wind; }
    const int &humidity() const { return m_humidity; }
    const bool &populated() const { return m_populated; }

private:
    std::string m_url;
    int m_temperature;
    bool m_isFahrenheit;
    int m_precipitation;
    std::string m_wind;
    int m_humidity;
    HttpClient *m_client;

    bool m_populated;

    std::string getHourlyForecastLink(const std::string& response);

    void populateFromHourlyForecast(const std::string& hourlyForecastResponse);
};

#endif // WEATHER_INFO_HPP
