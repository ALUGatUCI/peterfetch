#include "section.hpp"

#include <algorithm>
#include <iostream>
#include <utility>
#include <ranges>
#include <vector>

using namespace std;
using SLIterator = SectionList::iterator;

SectionList::SectionList(vector<Section*> sections)
    : m_sections { sections }
{}

void SectionList::add(Section *section) {
    m_sections.push_back(section);
}

SLIterator SectionList::begin() const {
    return SLIterator { this, 0, 0 };
}

SLIterator SectionList::end() const {
    return SLIterator { this, m_sections.size(), 0 };
}

size_t SectionList::size() const {
    size_t result = 0;
    for (auto section: m_sections) {
        result += section->size();
    }

    return result;
}

SLIterator &SLIterator::operator++() {
    if (!ptr)
        return *this;

    size_t size = ptr->m_sections[index]->size();
    index += (offset + 1) / size;
    offset = (offset + 1) % size;

    return *this;
}

SLIterator SLIterator::operator++(int) {
    if (!ptr)
        return *this;

    SLIterator prev { *this };

    size_t size = ptr->m_sections[index]->size();
    index += (offset + 1) / size;
    offset = (offset + 1) % size;

    return prev;
}

SLIterator::difference_type SLIterator::operator-(const SLIterator &rhs) {
    if (!ptr)
        return 0;

    if (ptr != rhs.ptr)
        throw runtime_error("Subtraction between unrelated iterators!");
    if (!ptr || !rhs.ptr)
        throw runtime_error("Cannot subtract with a default-value iterator!");

    auto lhs_sections =
        ptr->m_sections
        | views::transform([] (auto &section) { return section->size(); })
        | views::take(index);
    auto rhs_sections =
        rhs.ptr->m_sections
        | views::transform([] (auto &section) { return section->size(); })
        | views::take(rhs.index);
    auto lhs_val =
        accumulate(lhs_sections.begin(), lhs_sections.end(), 0) + offset;
    auto rhs_val =
        accumulate(rhs_sections.begin(), rhs_sections.end(), 0) + rhs.offset;

    return lhs_val - rhs_val;
}

bool SLIterator::operator==(const SLIterator &other) const {
    return ptr == other.ptr && index == other.index && offset == other.offset;
}

SLIterator::value_type SLIterator::operator*() const {
    if (!ptr)
        throw out_of_range("Can't dereference an default-value iterator");
    if (index >= ptr->m_sections.size())
        throw out_of_range("iterator index is out of bounds");
    if (offset >= ptr->m_sections[index]->size())
        throw out_of_range("iterator offset is out of bounds");
    return ptr->m_sections[index]->at(offset);
}
