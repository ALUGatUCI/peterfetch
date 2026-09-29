/** @file */

#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <iosfwd>
#include <memory>

#include "artwork.hpp"
#include "section.hpp"

class TextLayout
{
  public:
    TextLayout(const Artwork& art);

    void print(std::ostream& out) const;

    void addSection(std::shared_ptr<Section> section);

  private:
    SectionList produce_padded_section() const;

    Artwork m_art;
    SectionList m_sections;
    // TODO: Add section field name colors
};

std::ostream& operator<<(std::ostream& out, const TextLayout& layout);

#endif
