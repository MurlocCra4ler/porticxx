#pragma once

#include <bits/string/basic_string.hpp>
#include <bits/istream/basic_iostream.hpp>
#include <bits/memory/allocators.hpp>

namespace std {

template<class CharT, class Traits = char_traits<CharT>,
         class Allocator = allocator<CharT>>
class basic_stringstream : public basic_iostream<CharT, Traits> {
public:
    // members
    basic_string<CharT, Traits, Allocator> str() const &;
};

}