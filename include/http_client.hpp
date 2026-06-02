/** @file */

#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <cpr/cpr.h>

class HttpClient {
public:
    HttpClient(cpr::Header headers = {});

    virtual cpr::Response get(const cpr::Url& url, bool redirect = true);

    virtual ~HttpClient() = default;

private:
    cpr::Header m_headers;
};

#endif
