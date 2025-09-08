#pragma once

#include <bits/compare/classes.hpp>
#include <bits/ios/classes.hpp>

namespace std {

/***************/
/* char_traits */
/***************/

template <class CharT> struct char_traits;

template <> struct char_traits<char> {
    using char_type = char;
    using int_type = int;
    using off_type = streamoff;
    using pos_type = streampos;
    using state_type = mbstate_t;
    using comparison_category = strong_ordering;

    static constexpr void assign(char_type& c1, const char_type& c2) noexcept;
    static constexpr bool eq(char_type c1, char_type c2) noexcept {
        return c1 == c2;
    }
    static constexpr bool lt(char_type c1, char_type c2) noexcept;

    static constexpr int compare(const char_type* s1, const char_type* s2,
                                 size_t n);
    static constexpr size_t length(const char_type* s);
    static constexpr const char_type* find(const char_type* s, size_t n,
                                           const char_type& a);
    static constexpr char_type* move(char_type* s1, const char_type* s2,
                                     size_t n);
    static constexpr char_type* copy(char_type* s1, const char_type* s2,
                                     size_t n);
    static constexpr char_type* assign(char_type* s, size_t n, char_type a);

    static constexpr int_type not_eof(int_type c) noexcept;
    static constexpr char_type to_char_type(int_type c) noexcept;
    static constexpr int_type to_int_type(char_type c) noexcept;
    static constexpr bool eq_int_type(int_type c1, int_type c2) noexcept;
    static constexpr int_type eof() noexcept;
};

constexpr size_t char_traits<char>::length(const char_type* s) {
    size_t len = 0;
    while (*s++)
        ++len;
    return len;
}

constexpr char_traits<char>::char_type*
char_traits<char>::copy(char_type* s1, const char_type* s2, size_t n) {
    __builtin_memcpy(s1, s2, n);
    return s1;
}

constexpr char_traits<char>::int_type
char_traits<char>::not_eof(int_type c) noexcept {
    return c == eof() ? static_cast<int_type>(0) : c;
}

constexpr char_traits<char>::char_type
char_traits<char>::to_char_type(int_type c) noexcept {
    return static_cast<char_type>(c);
}

constexpr char_traits<char>::int_type
char_traits<char>::to_int_type(char_type c) noexcept {
    return static_cast<int_type>(c);
}

constexpr bool char_traits<char>::eq_int_type(int_type c1,
                                              int_type c2) noexcept {
    return c1 == c2;
}

constexpr char_traits<char>::int_type char_traits<char>::eof() noexcept {
    return static_cast<int_type>(-1);
}

} // namespace std