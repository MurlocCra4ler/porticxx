#pragma once

#include <bits/utility/functions.hpp>

namespace std {

template <class ForwardIter1, class ForwardIter2>
constexpr void iter_swap(ForwardIter1 a, ForwardIter2 b) {
    std::swap(*a, *b);
}

template <class ForwardIt1, class ForwardIt2>
constexpr ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1,
                                 ForwardIt2 first2) {
    for (; first1 != last1; ++first1, ++first2)
        std::iter_swap(first1, first2);

    return first2;
}

} // namespace std
