/** @file */

#ifndef SECTION_HPP
#define SECTION_HPP

#include <iterator>
#include <memory>
#include <string>

#include "directory_info.hpp"
#include "weather_info.hpp"

/**
 * Basic type indicator for controlling line output formats.
 */
enum class SectionLineType
{
    /** A line with a `key: value` format */
    KV,
    /** A blank line... what did you expect */
    BLANK,
};

struct SectionLine
{
    std::string label;
    std::string value;
    SectionLineType type;
};

class Section
{
  public:
    /**
     * Get the number of lines in this section
     */
    virtual std::size_t size() const = 0;

    /**
     * Get the contents of the line at index `offset`.
     *
     * @return A single line of output
     */
    virtual SectionLine at(std::size_t offset) const = 0;
};

class SectionList
{
  public:
    SectionList(std::vector<std::shared_ptr<Section>> sections);

    void add(std::shared_ptr<Section> section);

    class iterator
    {
      public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = SectionLine;

        iterator(
            const SectionList* p = nullptr, std::size_t start_index = 0,
            std::size_t start_offset = 0
        )
            : ptr {p}
            , index {start_index}
            , offset {start_offset}
        {
        }

        iterator& operator++();
        iterator operator++(int);
        difference_type operator-(const iterator& rhs);
        bool operator==(const iterator& other) const;
        value_type operator*() const;

      private:
        const SectionList* ptr;
        std::size_t index;
        std::size_t offset;
    };

    /**
     * NOTE: If a section list contains no sections with a nonzero number of
     * lines then this is equivalent to call to end()
     */
    iterator begin() const;
    iterator end() const;

  private:
    std::vector<std::shared_ptr<Section>> m_sections;

    std::size_t size() const;

    /**
     * Find the index of the first section which contains a nonzero number of
     * lines.
     */
    std::size_t first_index(std::size_t start_index) const;
};

static_assert(std::forward_iterator<SectionList::iterator>);
static_assert(std::ranges::forward_range<SectionList>);

class BlankSection : public Section
{
  public:
    BlankSection(std::size_t num_lines)
        : m_num {num_lines}
    {
    }

    /**
     * @copydoc Section::size()
     */
    std::size_t size() const override { return m_num; };

    /**
     * @copydoc Section::at()
     */
    SectionLine at(std::size_t offset) const override
    {
        if (offset > m_num)
            throw std::out_of_range("BlankSection offset is not in range");
        return {"", "", SectionLineType::BLANK};
    }

  private:
    std::size_t m_num;
};

class DirectoryInfoSection : public Section
{
  public:
    DirectoryInfoSection(const DirectoryInfo& info);

    /**
     * @copydoc Section::size()
     */
    std::size_t size() const override { return 4; };

    /**
     * @copydoc Section::at()
     */
    SectionLine at(std::size_t offset) const override;

  private:
    const DirectoryInfo& m_info;
};

class WeatherInfoSection : public Section
{
  public:
    WeatherInfoSection(const WeatherInfo& info);

    /**
     * @copydoc Section::size()
     */
    std::size_t size() const override { return 4; };

    /**
     * @copydoc Section::at()
     */
    SectionLine at(std::size_t offset) const override;

  private:
    const WeatherInfo& m_info;
};

#endif
