#ifndef DIRECTORY_INFO_HPP_
#define DIRECTORY_INFO_HPP_

#include <string>

#include <libxml/tree.h>

using namespace std::string_view_literals;

constexpr auto DIRECTORY_BASE_URL = "https://directory.uci.edu/people/"sv;

enum class DirectoryFetchResult {
    OK,
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
    DirectoryInfo(const std::string &netid);

    /**
     * Fetch the required data and populate this object with it.
     */
    DirectoryFetchResult fetch();

    void print(std::ostream &out) const;

private:
    bool m_populated;
    std::string m_url;
    std::string m_netid;
    std::string m_name;
    std::string m_major;
    StudentLevel m_level;

    /**
     * Parse the provided raw text and populate this object.
     */
    int parseRaw(const std::string &raw);

    /**
     * Traverse an XPath tree, parsing along the way to populate this object.
     */
    void parseXPath(xmlNodePtr node);

    StudentLevel levelFromString(const std::string &level) const;

    friend std::ostream &operator<<(std::ostream &out, const DirectoryInfo &info);
};

std::ostream &operator<<(std::ostream &out, const DirectoryInfo &info);
std::ostream &operator<<(std::ostream &out, const StudentLevel &level);

#endif
