#include "artwork.hpp"

Artwork::Artwork(const std::string &raw, int offset)
    : offset(offset)
{
    int j = 0;
    for (int i = 0; (i = raw.find("\n", i)) != std::string::npos; ++i) {
        lines.push_back(raw.substr(j, i - j));
        j = i + 1;
    }
}

Artwork::iterator Artwork::begin() const {
    return Artwork::iterator { this, 0, false };
}

Artwork::iterator Artwork::end() const {
    return Artwork::iterator { this, 0, true };
}

bool Artwork::iterator::isComplete() const {
    return ptr ? index > ptr->lines.size() : true;
}

Artwork::iterator &Artwork::iterator::operator++() {
    if (ptr)
        ++index;
    return *this;
}

bool Artwork::iterator::operator==(const Artwork::iterator &other) const {
    if (is_end || other.is_end)
        return is_end == other.is_end;
    else
        return ptr == other.ptr && index == other.index;
}

std::string Artwork::iterator::operator*() const {
    if (ptr && index < ptr->lines.size())
        return ptr->lines[index];
    else
        return std::string(ptr->offset, ' ');
}
