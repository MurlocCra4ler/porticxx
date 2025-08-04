#pragma once

#include <bits/ios/ios_base.hpp>
#include <bits/string/char_traits.hpp>
#include <bits/streambuf/basic_streambuf.hpp>

namespace std {

/************************/
/* Class std::basic_ios */
/************************/

template<class CharT, class Traits = char_traits<CharT>>
class basic_ios : public ios_base {
public:
    using char_type   = CharT;
    using int_type    = typename Traits::int_type;
    using pos_type    = typename Traits::pos_type;
    using off_type    = typename Traits::off_type;
    using traits_type = Traits;

    // flags functions
    void setstate(iostate state);

    // constructor/destructor
    explicit basic_ios(basic_streambuf<CharT, Traits>* sb);
    virtual ~basic_ios();

    // members
    basic_streambuf<CharT, Traits>* rdbuf() const;

    char_type widen(char c) const;

private:
    iostate m_state = goodbit;
    iostate m_exceptions = goodbit;

    basic_streambuf<CharT, Traits>* m_rdbuf;
};

// flags functions
template <class CharT, class Traits>
void basic_ios<CharT, Traits>::setstate(iostate state) {
    m_state |= state;

    if ((m_state & m_exceptions) != 0) {
        throw ios_base::failure("basic_ios::setstate: exception mask matched");
    }
}

// constructor/destructor
template<class CharT, class Traits>
basic_ios<CharT, Traits>::basic_ios(basic_streambuf<CharT, Traits>* sb) : m_rdbuf(sb) {}

template<class CharT, class Traits>
basic_ios<CharT, Traits>::~basic_ios() = default;

// members
template<class CharT, class Traits>
basic_streambuf<CharT, Traits>* basic_ios<CharT, Traits>::rdbuf() const {
    return m_rdbuf;
}

template<class CharT, class Traits>
basic_ios<CharT, Traits>::char_type basic_ios<CharT, Traits>::widen(char c) const {
    // TODO: return this->getloc().template use_facet<std::ctype<CharT>>().widen(c);
    return static_cast<char_type>(c);
}

}