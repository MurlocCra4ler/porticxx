#pragma once

#include <bits/algorithm/modifying_sequence_operations.hpp>

namespace std {

template <class RandomAccessIter>
constexpr void sort(RandomAccessIter first, RandomAccessIter last) {
    auto len = last - first;
    if (len <= 1)
        return;

    auto insertion_sort = [](RandomAccessIter f, RandomAccessIter l) constexpr {
        for (auto i = f + 1; i < l; ++i) {
            auto key = *i;
            auto j = i;
            while (j > f && *(j - 1) > key) {
                *j = *(j - 1);
                --j;
            }
            *j = key;
        }
    };

    auto quicksort = [&](auto&& self, RandomAccessIter f,
                         RandomAccessIter l) constexpr -> void {
        auto n = l - f;
        if (n <= 16) { // für kleine Bereiche effizienter
            insertion_sort(f, l);
            return;
        }
        auto pivot = f + n / 2;
        auto pivot_value = *pivot;
        std::iter_swap(pivot, l - 1); // Pivot ans Ende

        RandomAccessIter i = f;
        for (RandomAccessIter j = f; j < l - 1; ++j) {
            if (*j < pivot_value) {
                std::iter_swap(i, j);
                ++i;
            }
        }
        std::iter_swap(i, l - 1); // Pivot an endgültige Stelle

        self(self, f, i);
        self(self, i + 1, l);
    };

    quicksort(quicksort, first, last);
}

} // namespace std