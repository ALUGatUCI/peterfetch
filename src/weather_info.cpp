#include "weather_info.hpp"
#include "http_client.hpp"

#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

WeatherInfo::WeatherInfo(const std::string& url,
                         const double latitude,
                         const double longitude,
                         HttpClient *client):
    m_url(url + "points/" + std::to_string(latitude) + "," + std::to_string(longitude)),
    m_client(client),
    populated(false) {
}

WeatherFetchResult WeatherInfo::fetch() {
    cpr::Response response = m_client->get(m_url);
    if (response.status_code / 100 == 3)
        return WeatherFetchResult::DOESNT_EXIST;
    else if (response.status_code != 200)
        return WeatherFetchResult::INVALID_RESPONSE;

    std::string hourlyForecastLink;
    try {
        hourlyForecastLink = getHourlyForecastLink(response.text);
    } catch (const nlohmann::json::exception& e) {
        return WeatherFetchResult::INVALID_RESPONSE;
    }

    cpr::Response hourlyForecastResponse = m_client->get(hourlyForecastLink);
    if (hourlyForecastResponse.status_code != 200)
        return WeatherFetchResult::INVALID_RESPONSE;

    try {
        populateFromHourlyForecast(hourlyForecastResponse.text);
    } catch (const nlohmann::json::exception& e) {
        return WeatherFetchResult::INVALID_RESPONSE;
    }

    return WeatherFetchResult::OK;
}

void WeatherInfo::print(std::ostream &out) const {
    if (!populated) return;
    out << "Temperature: " << m_temperature << std::endl;
    out << "Precipitation: " << m_precipitation << std::endl;
    out << "Wind: " << m_wind << std::endl;
    out << "Humidity: " << m_humidity << std::endl;
}

std::string WeatherInfo::getHourlyForecastLink(const std::string& response) {
    nlohmann::json jsonResponse = nlohmann::json::parse(response);
    std::cout << jsonResponse["properties"]["forecastHourly"] << std::endl;
    return jsonResponse["properties"]["forecastHourly"];
}

void WeatherInfo::populateFromHourlyForecast(const std::string& hourlyForecastResponse) {
    nlohmann::json hourlyForecastJson = nlohmann::json::parse(hourlyForecastResponse);

    // Populate with the first item in the hourly forecast
    if (hourlyForecastJson.empty()) {
        throw std::runtime_error("Hourly forecast is empty");
    }

    try {
        nlohmann::json firstItem = hourlyForecastJson["properties"]["periods"][0];
        m_temperature = std::to_string(firstItem["temperature"].get<int>())
                      + " " + firstItem["temperatureUnit"].get<std::string>();
        m_precipitation = std::to_string(firstItem["probabilityOfPrecipitation"].get<int>()) + "%";
        m_wind = firstItem["windSpeed"].get<std::string>()
               + " " + firstItem["windDirection"].get<std::string>();
        m_humidity = std::to_string(firstItem["relativeHumidity"]["value"].get<int>()) + "%";

    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Failed to parse hourly forecast: " + std::string(e.what()));
    }

    populated = true;
}
