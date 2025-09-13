#pragma once

#include <bits/utility/functions.hpp>

namespace std {

template <class ForwardIter1, class ForwardIter2>
constexpr void iter_swap(ForwardIter1 a, ForwardIter2 b) {
    std::swap(*a, *b);
}

} // namespace std
