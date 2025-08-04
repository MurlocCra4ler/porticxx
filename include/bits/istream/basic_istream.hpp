#pragma once

#include <bits/ios/basic_ios.hpp>

namespace std {

/****************************/
/* Class std::basic_istream */
/****************************/

template<class CharT, class Traits = char_traits<CharT>>
class basic_istream : virtual public basic_ios<CharT, Traits> {};

}