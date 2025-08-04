#pragma once

#include <bits/istream/basic_istream.hpp>
#include <bits/ostream/basic_ostream.hpp>

namespace std {

/****************************/
/* Class std::basic_istream */
/****************************/

template<class CharT, class Traits = char_traits<CharT>>
  class basic_iostream
    : public basic_istream<CharT, Traits>,
      public basic_ostream<CharT, Traits> {};

}