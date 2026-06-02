/**
 * General utilities for working with iterators.
 *
 * Because C++23 is not used in this project, this provides implementations
 * roughly equivalent in functionality to certain features from the ranges
 * library.
 *
 * @file
 */

#ifndef ITER_UTILS_HPP
#define ITER_UTILS_HPP

#include <cstddef>
#include <iterator>
#include <ranges>
#include <utility>

/**
 * Zip two ranges that yield values of type `L` and `R`, respectively, and
 * produce a single range which yields values of `std::pair<L, R>`. If one
 * range is longer than the other, the result is truncated to the shortest of
 * the two.
 */
template<std::ranges::forward_range Left, std::ranges::forward_range Right>
class ZipRange {
public:
    ZipRange(Left &left, Right &right)
        : m_left { left }
        , m_right { right }
    {}

    struct iterator {
        using left_iter_t = std::ranges::iterator_t<Left>;
        using right_iter_t = std::ranges::iterator_t<Right>;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<std::iter_value_t<left_iter_t>,
                                     std::iter_value_t<right_iter_t>>;

        iterator(left_iter_t left = {}, left_iter_t left_end = {},
                 right_iter_t right = {}, right_iter_t right_end = {})
            : m_left_iter { left }
            , m_left_end_iter { left_end }
            , m_right_iter { right }
            , m_right_end_iter { right_end }
        {}

        left_iter_t m_left_iter;
        left_iter_t m_left_end_iter;
        right_iter_t m_right_iter;
        right_iter_t m_right_end_iter;

        /**
         * Ensure that if either the left or right iterator has reached the
         * end, both iterators will be equal to their respective ends after
         * completion
         */
        void maybe_complete() {
            if (m_left_iter == m_left_end_iter)
                m_right_iter = m_right_end_iter;
            else if (m_right_iter == m_right_end_iter)
                m_left_iter = m_left_end_iter;
        }

        iterator &operator++() {
            ++m_left_iter;
            ++m_right_iter;
            maybe_complete();

            return *this;
        };
        iterator operator++(int) {
            left_iter_t l = m_left_iter++;
            right_iter_t r = m_right_iter++;
            maybe_complete();

            return { l, r };
        }

        bool operator==(const iterator &other) const {
            return m_left_iter == other.m_left_iter
                   && m_right_iter == other.m_right_iter;
        };

        value_type operator*() const {
            return std::make_pair(*m_left_iter, *m_right_iter);
        }
    };

    iterator begin() {
        return { m_left.begin(), m_left.end(), m_right.begin(),
                 m_right.end() };
    }
    iterator end() {
        return { m_left.end(), m_left.end(), m_right.end(), m_right.end() };
    }

private:
    Left &m_left;
    Right &m_right;
};

// Basic proof that the range interface is implemented
static_assert(std::forward_iterator<ZipRange<std::string, std::string>::iterator>);
static_assert(std::ranges::range<ZipRange<std::string, std::string>>);

#endif
