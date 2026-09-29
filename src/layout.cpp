#include "layout.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <ranges>

#include "artwork.hpp"
#include "iter_utils.hpp"
#include "section.hpp"

TextLayout::TextLayout(const Artwork& art)
    : m_art(art)
    , m_sections({})
{
}

void TextLayout::addSection(std::shared_ptr<Section> section)
{
    m_sections.add(section);
}

SectionList TextLayout::produce_padded_section() const
{
    size_t section_lines = m_sections.end() - m_sections.begin();
    size_t num_padding = m_art.artwork_lines() >= section_lines
                             ? m_art.artwork_lines() - section_lines
                             : 0UL;
    SectionList sections {{}};

    auto padding_section = std::make_shared<BlankSection>(num_padding);
    sections.add(padding_section);

    return sections;
}

void TextLayout::print(std::ostream& out) const
{
    std::string padding("  ");
    SectionList padding_section = produce_padded_section();
    auto sections = std::array {m_sections, padding_section} | std::views::join
                    | std::views::common;
    auto zipped = ZipRange {m_art, sections};

    std::for_each(
        zipped.begin(), zipped.end(),
        [&](auto line)
        {
            out << "\x1b[38;2;254;204;7m" << padding << line.first << "\x1b[0m"
                << padding;

            if (line.second.type != SectionLineType::BLANK)
                out << line.second.label << ": " << line.second.value;
            out << "\n";
        }
    );
}

std::ostream& operator<<(std::ostream& out, const TextLayout& layout)
{
    layout.print(out);
    return out;
}
