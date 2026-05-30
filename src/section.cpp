#include "section.hpp"

#include <format>
#include <utility>

#include "directory_info.hpp"

DirectoryInfoSection::DirectoryInfoSection(const DirectoryInfo &info)
    : m_info(info)
{}

SectionLine DirectoryInfoSection::at(std::size_t offset) const {
    // NOTE: when plumbing in configuration, this will need to become dynamic
    switch (offset) {
        case 0:
            return { "UCInetID", m_info.netid(), SectionLineType::KV };
        case 1:
            return { "Name", m_info.name(), SectionLineType::KV };
        case 2:
            return { "Major", m_info.major(), SectionLineType::KV };
        case 3:
            return { "Level", std::format("{}", m_info.level()),
                     SectionLineType::KV };
        default:
            throw std::out_of_range("Invalid DirectoryInfoSection offset");
    }
}
