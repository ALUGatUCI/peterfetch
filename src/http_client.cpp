#include "http_client.hpp"

#include <cpr/cpr.h>

HttpClient::HttpClient(cpr::Header headers)
    : m_headers(headers)
{
    // Ensure that some base headers are always present
    m_headers["User-Agent"] = "peterfetch";
}

cpr::Response HttpClient::get(const cpr::Url &url, bool redirect) {
    return cpr::Get(url, m_headers, cpr::Redirect { redirect });
}
