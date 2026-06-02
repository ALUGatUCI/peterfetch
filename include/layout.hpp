/** @file */

#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <iosfwd>

#include "artwork.hpp"
#include "section.hpp"

class TextLayout {
public:
    TextLayout(const Artwork &art);

    void print(std::ostream &out) const;

    void addSection(Section *section);

private:
    SectionList produce_padded_section() const;

    Artwork m_art;
    SectionList m_sections;
};

std::ostream &operator<<(std::ostream &out, const TextLayout &layout);

#endif
