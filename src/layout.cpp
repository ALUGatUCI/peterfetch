#include "layout.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>

#include "artwork.hpp"
#include "iter_utils.hpp"
#include "section.hpp"

using namespace std;

TextLayout::TextLayout(const Artwork &art)
    : m_art(art)
    , m_sections({})
{}

void TextLayout::addSection(Section *section) {
    m_sections.add(section);
}

SectionList TextLayout::produce_padded_section() const {
    size_t section_lines = m_sections.end() - m_sections.begin();
    size_t num_padding = max(m_art.artwork_lines() - section_lines, 0UL);
    SectionList sections { {} };

    // TODO: migrate everything to smart pointers
    BlankSection *padding_section = new BlankSection { num_padding };
    sections.add(padding_section);

    return sections;
}

void TextLayout::print(ostream &out) const {
    string padding("  ");
    SectionList padding_section = produce_padded_section();
    auto sections = array { m_sections, padding_section } | views::join | views::common;
    auto zipped = ZipRange { m_art, sections };

    for_each(zipped.begin(), zipped.end(), [&] (auto line) {
        out << "\x1b[38;2;254;204;7m"
            << padding << line.first << "\x1b[0m" << padding;

        if (line.second.type != SectionLineType::BLANK)
            out << line.second.label << ": " << line.second.value;
        out << "\n";
    });
}

ostream &operator<<(ostream &out, const TextLayout &layout) {
    layout.print(out);
    return out;
}
