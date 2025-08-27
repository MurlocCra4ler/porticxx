#pragma once

#include <bits/string/basic_string.hpp>
#include <bits/string_view/io.hpp>
#include <bits/istream/basic_istream.hpp>
#include <bits/ostream/basic_ostream.hpp>

namespace std {

template<class CharT, class Traits, class Allocator>
std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os,
               const std::basic_string<CharT, Traits, Allocator>& str) {
    return os << basic_string_view<CharT, Traits>(str);
}

template<class CharT, class Traits, class Allocator>
basic_istream<CharT, Traits>&
getline(basic_istream<CharT, Traits>& input,
        basic_string<CharT, Traits, Allocator>& str) {
    using istream_type = basic_istream<CharT, Traits>;
    using int_type     = typename istream_type::int_type;
    using traits_type  = Traits;

    str.clear();
    
    typename istream_type::sentry s(input, true);
    if (!s) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    int_type c = 0;
    bool extracted = false;

    while (true) {
        c = input.rdbuf()->sbumpc();

        if (traits_type::eq_int_type(c, traits_type::eof())) {
            if (!extracted) input.setstate(std::ios_base::failbit | std::ios_base::eofbit);
            else input.setstate(std::ios_base::eofbit);
            break;
        }

        if (c == traits_type::to_int_type('\n')) break;

        str.push_back(traits_type::to_char_type(c));
        extracted = true;
    }

    return input;
}

}