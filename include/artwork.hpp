#ifndef ARTWORK_HPP
#define ARTWORK_HPP

#include <iterator>
#include <ranges>
#include <string>
#include <vector>

class Artwork {
public:
    Artwork(const std::string &raw, int offset);

    /**
     * An infinite iterator over the artwork's lines.
     *
     * Space-filled lines are produced when the iterator exceeds the true
     * length of the artwork.
     */
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::string;

        iterator()
            : ptr { nullptr }
            , index { 0 }
            , is_end { true }
        {}

        iterator(const Artwork *p, std::size_t start_index = 0,
                 bool is_end = false)
            : ptr { p }
            , index { start_index }
            , is_end { is_end }
        {}

        const Artwork *ptr;
        std::size_t index;
        bool is_end;

        /**
         * Indicates if the artwork has been completely traversed.
         *
         * @retval true All following values including the current one are
         *              filler lines
         * @retval false The current line is guaranteed to be artwork
         */
        bool isComplete() const;

        iterator &operator++();
        iterator operator++(int);
        bool operator==(const iterator &other) const;
        value_type operator*() const;
    };

    iterator begin()const;
    iterator end() const;
    std::size_t artwork_lines() const { return lines.size(); };

private:
    std::vector<std::string> lines;
    int offset;
};

static_assert(std::forward_iterator<Artwork::iterator>);
static_assert(std::ranges::forward_range<Artwork>);

namespace artwork {

// TODO: Color
constexpr int UCI_OFFSET = 17;
constexpr const char *UCI = static_cast<const char*>(R"(
 _   _  ____ ___ 
| | | |/ ___|_ _|
| | | | |    | | 
| |_| | |___ | | 
 \___/ \____|___|
)") + 1; // Erase the extra newline at the beginning

}

#endif
