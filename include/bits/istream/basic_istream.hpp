#pragma once

#include <bits/ios/basic_ios.hpp>

namespace std {

/****************************/
/* Class std::basic_istream */
/****************************/

template <class CharT, class Traits = char_traits<CharT>>
class basic_istream : virtual public basic_ios<CharT, Traits> {
public:
    using char_type = CharT;
    using int_type = typename Traits::int_type;
    using pos_type = typename Traits::pos_type;
    using off_type = typename Traits::off_type;
    using traits_type = Traits;

    explicit basic_istream(basic_streambuf<CharT, Traits>* sb)
        : basic_ios<CharT>(sb) {}
    virtual ~basic_istream() = default;

    // prefix/suffix
    class sentry;
};

} // namespace std