#include <sstream>
#include <iostream>
#include <stdexcept>
#include <numeric>

#include "artwork.hpp"
#include "iter_utils.hpp"

using namespace std;

Artwork::Artwork(const string &raw, int offset)
    : offset(offset)
{
    int j = 0;
    for (int i = 0; (i = raw.find("\n", i)) != string::npos; ++i) {
        lines.push_back(raw.substr(j, i - j));
        j = i + 1;
    }
}

Artwork::Artwork(const string &raw, int offset, const char *raw_fg, const char *raw_bg) 
    : Artwork(raw, offset) 
{
    color(raw_fg, raw_bg);    
}

Artwork::iterator Artwork::begin() const {
    return Artwork::iterator { this, 0, false };
}

Artwork::iterator Artwork::end() const {
    return Artwork::iterator { this, 0, true };
}

vector<string> Artwork::split_lines(string_view raw) {
    int j = 0;
    vector<string> result;

    for (int i = 0; (i = raw.find("\n", i)) != string::npos; ++i) {
        result.push_back(string { raw.substr(j, i - j) });
        j = i + 1;
    }
    return result;
}

void Artwork::color(const char *raw_fg, const char *raw_bg) {
    char curr_fg {' '};    
    char curr_bg {' '};    
    vector<string> fg {split_lines(raw_fg)};
    vector<string> bg {split_lines(raw_bg)};
    ZipRange color_delims {fg, bg};

    auto split_size = [](int ac, string &s) { return ac + s.size(); };
    int fg_size = accumulate(fg.begin(), fg.end(), 0, split_size);
    int bg_size = accumulate(bg.begin(), bg.end(), 0, split_size);
    int lines_size = accumulate(lines.begin(), lines.end(), 0, split_size);
    if (fg_size != bg_size)
      throw runtime_error("Delimiters are not of equal size");
    if (fg_size != lines_size || bg_size != lines_size)
      throw runtime_error("Delimiters are not equal in size to the artwork");

    auto delim_lines {color_delims.begin()};

    vector<string> colored_lines {};

    for (string line : lines) {
        size_t line_width {line.size()};
        stringstream colored_line {};
        pair<string, string> delim_line {*delim_lines};
        for (int i {}; i < line_width; ++i) {
            const char &delim_fg {delim_line.first[i]};
            const char &delim_bg {delim_line.second[i]};

            if (!artwork::COLORS.contains(delim_fg) ||
                !artwork::COLORS.contains(delim_bg)
            ) {
                throw runtime_error("Invalid color delimiter");
            }

            if (delim_fg != curr_fg || delim_bg != curr_bg) {
                curr_fg = delim_fg;
                curr_bg = delim_bg;
                colored_line << "\x1b[" << artwork::COLORS.at(curr_fg).first << ";" 
                          << artwork::COLORS.at(curr_bg).second << "m";
            }
            colored_line << line[i];
        }

        colored_line << "\x1b[0m";
        curr_fg = ' ';
        curr_bg = ' ';

        ++delim_lines;
        colored_lines.push_back(colored_line.str());
    }

    lines = colored_lines;
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

string Artwork::iterator::operator*() const {
    if (ptr && index < ptr->lines.size())
        return ptr->lines[index];
    else
        return string(ptr->offset, ' ');
}
