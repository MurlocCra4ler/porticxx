#pragma once

#include <bits/tuple/functions.hpp>
#include <bits/tuple/tuple.hpp>
#include <bits/utility/tags.hpp>

namespace std {

template <class T1, class T2> struct pair {
    using first_type = T1;
    using second_type = T2;

    T1 first;
    T2 second;

    pair(const pair&) = default;
    pair(pair&&) = default;

    template <class... Args1, class... Args2>
    pair(piecewise_construct_t, tuple<Args1...> first_args,
         tuple<Args2...> second_args) {
        apply(
            [this](Args1&&... args) {
                ::new (const_cast<remove_const_t<T1>*>(&first))
                    T1(std::forward<Args1>(args)...);
            },
            std::move(first_args));

        apply(
            [this](Args2&&... args) {
                ::new (static_cast<void*>(&second))
                    T2(std::forward<Args2>(args)...);
            },
            std::move(second_args));
    }
};

} // namespace std