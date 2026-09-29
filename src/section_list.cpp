#include "section.hpp"

#include <memory>
#include <ranges>
#include <vector>

using SLIterator = SectionList::iterator;

SectionList::SectionList(std::vector<std::shared_ptr<Section>> sections)
    : m_sections {sections}
{
}

void SectionList::add(std::shared_ptr<Section> section)
{
    m_sections.push_back(section);
}

SLIterator SectionList::begin() const
{
    return SLIterator {this, first_index(0), 0};
}

SLIterator SectionList::end() const
{
    return SLIterator {this, m_sections.size(), 0};
}

size_t SectionList::size() const
{
    size_t result = 0;
    for (auto section : m_sections)
    {
        result += section->size();
    }

    return result;
}

size_t SectionList::first_index(size_t start_index) const
{
    size_t end_index = m_sections.size();

    if (start_index < 0 || start_index >= end_index)
        return end_index;

    size_t index = start_index;
    while (index < end_index && m_sections[index]->size() == 0)
        ++index;

    return index;
}

SLIterator& SLIterator::operator++()
{
    if (!ptr)
        return *this;

    size_t size = ptr->m_sections[index]->size();
    if (++offset >= size)
    {
        index = ptr->first_index(index + 1);
        offset = 0;
    }

    return *this;
}

SLIterator SLIterator::operator++(int)
{
    if (!ptr)
        return *this;

    SLIterator prev {*this};

    size_t size = ptr->m_sections[index]->size();
    if (++offset >= size)
    {
        index = ptr->first_index(index + 1);
        offset = 0;
    }

    return prev;
}

SLIterator::difference_type SLIterator::operator-(const SLIterator& rhs)
{
    if (!ptr)
        return 0;

    if (ptr != rhs.ptr)
        throw std::runtime_error("Subtraction between unrelated iterators!");
    if (!ptr || !rhs.ptr)
        throw std::runtime_error(
            "Cannot subtract with a default-value iterator!"
        );

    auto lhs_sections =
        ptr->m_sections
        | std::views::transform([](auto& section) { return section->size(); })
        | std::views::take(index);
    auto rhs_sections =
        rhs.ptr->m_sections
        | std::views::transform([](auto& section) { return section->size(); })
        | std::views::take(rhs.index);
    auto lhs_val =
        std::accumulate(lhs_sections.begin(), lhs_sections.end(), 0) + offset;
    auto rhs_val = std::accumulate(rhs_sections.begin(), rhs_sections.end(), 0)
                   + rhs.offset;

    return lhs_val - rhs_val;
}

bool SLIterator::operator==(const SLIterator& other) const
{
    return ptr == other.ptr && index == other.index && offset == other.offset;
}

SLIterator::value_type SLIterator::operator*() const
{
    if (!ptr)
        throw std::out_of_range("Can't dereference an default-value iterator");
    if (index >= ptr->m_sections.size())
        throw std::out_of_range("iterator index is out of bounds");
    if (offset >= ptr->m_sections[index]->size())
        throw std::out_of_range("iterator offset is out of bounds");
    return ptr->m_sections[index]->at(offset);
}
