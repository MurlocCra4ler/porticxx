#pragma once

#include <cuchar>

namespace std {

/************************/
/* Forward declarations */
/************************/

template<class CharT> struct char_traits;
template<> struct char_traits<char>;
template<> struct char_traits<char8_t>;
template<> struct char_traits<char16_t>;
template<> struct char_traits<char32_t>;
template<> struct char_traits<wchar_t>;

template<class State> class fpos;
using streampos = fpos<mbstate_t>;

}
