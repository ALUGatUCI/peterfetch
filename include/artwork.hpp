/** @file */

#ifndef ARTWORK_HPP
#define ARTWORK_HPP

#include <iterator>
#include <map>
#include <ranges>
#include <string>
#include <vector>

class Artwork
{
  public:
    Artwork(const std::string& raw, int offset);
    Artwork(
        const std::string& raw, int offset, const char* raw_fg,
        const char* raw_bg
    );

    /**
     * An infinite iterator over the artwork's lines.
     *
     * Space-filled lines are produced when the iterator exceeds the true
     * length of the artwork.
     */
    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::string;

        iterator()
            : ptr {nullptr}
            , index {0}
            , is_end {true}
        {
        }

        iterator(
            const Artwork* p, std::size_t start_index = 0, bool is_end = false
        )
            : ptr {p}
            , index {start_index}
            , is_end {is_end}
        {
        }

        const Artwork* ptr;
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

        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        value_type operator*() const;
    };

    iterator begin() const;
    iterator end() const;
    std::size_t artwork_lines() const { return lines.size(); };
    std::vector<std::string> split_lines(std::string_view raw);
    void color(const char* fg, const char* bg);

  private:
    std::vector<std::string> lines;
    int offset;
};

static_assert(std::forward_iterator<Artwork::iterator>);
static_assert(std::ranges::forward_range<Artwork>);

namespace artwork
{

// TODO: Color
constexpr int UCI_OFFSET = 17;
constexpr const char* UCI = static_cast<const char*>(R"(
 _   _  ____ ___ 
| | | |/ ___|_ _|
| | | | |    | | 
| |_| | |___ | | 
 \___/ \____|___|
)") + 1; // Erase the extra newline at the beginning

/**
 * Contains delimiters for foreground and background colors.
 *
 * Delimiters are keys to std::pair<int, int>:
 * first -> foreground color
 * second -> background color
 */
const std::map<char, const std::pair<const int, const int>> COLORS = {
    {' ', {39, 49} }, // default
    {'0', {30, 40} }, // black
    {'1', {31, 41} }, // red
    {'2', {32, 42} }, // green
    {'3', {33, 43} }, // yellow
    {'4', {34, 44} }, // blue
    {'5', {35, 45} }, // magenta
    {'6', {36, 46} }, // cyan
    {'7', {37, 47} }, // white
    {'8', {90, 100}}, // gray
    {'9', {91, 101}}, // bright red
    {'A', {92, 102}}, // bright green
    {'B', {93, 103}}, // bright yellow
    {'C', {94, 104}}, // bright blue
    {'D', {95, 105}}, // bright magenta
    {'E', {96, 106}}, // bright cyan
    {'F', {97, 107}}, // bright white
};

constexpr const char* UCI_FG = static_cast<const char*>(R"(
 4   4  4444 444 
4 4 4 44 44444 44
4 4 4 4 4    4 4 
4 444 4 4444 4 4 
 44444 4444444444
)") + 1;

constexpr const char* UCI_BG = static_cast<const char*>(R"(
                 
 3   3 33333 333 
 3   3 3      3  
 3   3 3      3  
 33333 33333 333 
)") + 1;

} // namespace artwork

#endif
