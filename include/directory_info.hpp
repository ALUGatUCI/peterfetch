#ifndef DIRECTORY_INFO_HPP
#define DIRECTORY_INFO_HPP

#include <string>

#include <libxml/tree.h>

#include "http_client.hpp"

constexpr const char *DIRECTORY_BASE_URL = "https://directory.uci.edu/people/";

enum class DirectoryFetchResult {
    OK,
    INVALID_RESPONSE,
    DOESNT_EXIST,
    PARSING_FAILED,
};

enum class StudentLevel {
    FRESHMAN,
    SOPHOMORE,
    JUNIOR,
    SENIOR
};

class DirectoryInfo {
public:
    DirectoryInfo(
        const std::string &netid,
        const std::string &url = DIRECTORY_BASE_URL,
        HttpClient *client = new HttpClient
    );

    /**
     * Fetch the required data and populate this object with it.
     */
    DirectoryFetchResult fetch();

    void print(std::ostream &out) const;

#define GEN_GETATTR(type, name)                                               \
    [[nodiscard]]                                                             \
    const type &name() const {                                                \
        ensure_populated();                                                   \
        return m_##name;                                                      \
    }

    GEN_GETATTR(std::string, netid);
    GEN_GETATTR(std::string, name);
    GEN_GETATTR(std::string, major);
    GEN_GETATTR(StudentLevel, level);

#undef GEN_GETATTR

    const bool &populated() const { return m_populated; }

private:
    bool m_populated;
    std::string m_url;
    std::string m_netid;
    std::string m_name;
    std::string m_major;
    StudentLevel m_level;
    HttpClient *m_client;

    /**
     * Parse the provided raw text and populate this object.
     */
    int parseRaw(const std::string &raw);

    /**
     * Traverse an XPath tree, parsing along the way to populate this object.
     */
    void parseXPath(xmlNodePtr node);

    StudentLevel levelFromString(const std::string &level) const;

    void ensure_populated() const;

    friend std::ostream &operator<<(std::ostream &out, const DirectoryInfo &info);
};

template<>
struct std::formatter<StudentLevel, char> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    std::format_context::iterator format(
        const StudentLevel &level,
        std::format_context &ctx
    ) const;
};

std::ostream &operator<<(std::ostream &out, const DirectoryInfo &info);
std::ostream &operator<<(std::ostream &out, const StudentLevel &level);

#endif
