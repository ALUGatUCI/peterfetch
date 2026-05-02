#include "directory_info.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>

#include <cpr/cpr.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "http_client.hpp"

DirectoryInfo::DirectoryInfo(const std::string &netid, const std::string &url,
                             HttpClient *client)
    : m_netid(netid)
    , m_url(url + netid + ".txt")
    , m_client(client)
    , populated(false)
{
}

DirectoryFetchResult DirectoryInfo::fetch() {
    cpr::Response response = m_client->get(cpr::Url { m_url }, false);

    if (response.status_code / 100 == 3)
        return DirectoryFetchResult::DOESNT_EXIST;
    else if (response.status_code != 200)
        return DirectoryFetchResult::INVALID_RESPONSE;

    if (parseRaw(response.text) < 0)
        return DirectoryFetchResult::PARSING_FAILED;

    return DirectoryFetchResult::OK;
}

void DirectoryInfo::print(std::ostream &out) const {
    if (populated)
        out << "DirectoryInfo {"
            << " netid=" << m_netid
            << ", name=" << m_name
            << ", level=" << m_level
            << ", major=" << m_major
            << " }";
    else
        out << "DirectoryInfo { unpopulated }";
}

int DirectoryInfo::parseRaw(const std::string &raw) {
    std::unique_ptr<xmlDoc, void(*)(xmlDocPtr)> doc(
        htmlReadMemory(raw.c_str(), raw.size(), m_url.c_str(), "UTF-8", 0),
        xmlFreeDoc
    );
    if (!doc) {
        std::cerr << "Unable to parse raw directory info!\n";
        return -1;
    }

    std::unique_ptr<xmlXPathContext, void(*)(xmlXPathContextPtr)> xpathCtx(
        xmlXPathNewContext(doc.get()),
        xmlXPathFreeContext
    );
    if (!xpathCtx) {
        std::cerr << "Failed to create an XPath context!\n";
        return -1;
    }

    std::unique_ptr<xmlXPathObject, void(*)(xmlXPathObjectPtr)> xpathObj(
        xmlXPathEvalExpression(
            reinterpret_cast<const xmlChar*>("//body"),
            xpathCtx.get()
        ),
        xmlXPathFreeObject
    );
    if(!xpathObj) {
        std::cerr << "Failed to evaluate XPath expression!\n";
        return -1;
    }

    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    if (nodeset->nodeNr < 1) {
        std::cerr << "XPath is missing nodes!\n";
        return -1;
    }

    // Note: the extraction takes place on the text nodes under the body and
    // not the body itself
    parseXPath(nodeset->nodeTab[0]->children);

    populated = true;

    return 0;
}

void DirectoryInfo::parseXPath(xmlNodePtr node) {
    for (xmlNodePtr p = node; p->next; p = p->next) {
        if (p->type == XML_TEXT_NODE) {
            std::string str { reinterpret_cast<const char *>(p->content) };
            if (str[0] == '\n')
                str.erase(0, 1);

            // Rework this to not be a bit less dumb
            std::size_t split = str.find(": ");
            std::string_view key { str };
            key = key.substr(0, split);
            std::string value { str.substr(split + 2) };

            if (key == "UCInetID") {
                m_netid = value;
            } else if (key == "Name") {
                m_name = value;
            } else if (key == "Major") {
                m_major = value;
            } else if (key == "Student's Level") {
                m_level = levelFromString(value);
            }
        }
    }
}

StudentLevel DirectoryInfo::levelFromString(const std::string &level) const {
    if (level == "SR") {
        return StudentLevel::SENIOR;
    } else if (level == "JR") {
        return StudentLevel::JUNIOR;
    } else if (level == "SO") {
        return StudentLevel::SOPHOMORE;
    } else {
        return StudentLevel::FRESHMAN;
    }
}

std::ostream &operator<<(std::ostream &out, const DirectoryInfo &info) {
    info.print(out);
    return out;
}

std::ostream &operator<<(std::ostream &out, const StudentLevel &level) {
    switch (level) {
        case StudentLevel::FRESHMAN:
            out << "FRESHMAN";
            break;
        case StudentLevel::SOPHOMORE:
            out << "SOPHOMORE";
            break;
        case StudentLevel::JUNIOR:
            out << "JUNIOR";
            break;
        case StudentLevel::SENIOR:
            out << "SENIOR";
            break;
    }
    return out;
}
