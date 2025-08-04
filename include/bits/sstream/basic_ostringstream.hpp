#pragma once

#include <bits/string/basic_string.hpp>
#include <bits/istream/basic_iostream.hpp>

namespace std {

template<class CharT, class Traits = char_traits<CharT>,
         class Allocator = allocator<CharT>>
class basic_ostringstream : public basic_ostream<CharT, Traits> {
public:
    using char_type      = CharT;
    using int_type       = typename Traits::int_type;
    using pos_type       = typename Traits::pos_type;
    using off_type       = typename Traits::off_type;
    using traits_type    = Traits;
    using allocator_type = Allocator;

    // members
    basic_string<CharT, Traits, Allocator> str() const &;
};

}