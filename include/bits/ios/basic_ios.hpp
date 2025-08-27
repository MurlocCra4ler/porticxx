#pragma once

#include <bits/ios/ios_base.hpp>
#include <bits/string/char_traits.hpp>
#include <bits/streambuf/basic_streambuf.hpp>

namespace std {

/************************/
/* Class std::basic_ios */
/************************/

template<class CharT, class Traits>
class basic_ostream; 

template<class CharT, class Traits = char_traits<CharT>>
class basic_ios : public ios_base {
public:
    using char_type   = CharT;
    using int_type    = typename Traits::int_type;
    using pos_type    = typename Traits::pos_type;
    using off_type    = typename Traits::off_type;
    using traits_type = Traits;

    // flags functions
    explicit operator bool() const { return !fail(); }
    result<void> setstate(iostate state);
    bool good() const { return (m_state & (failbit | badbit | eofbit)) == 0; }
    bool eof() const { return (m_state & eofbit) != 0; }
    bool fail() const { return (m_state & (failbit | badbit)) != 0; }
    bool bad() const { return (m_state & badbit) != 0; }

    // constructor/destructor
    explicit basic_ios(basic_streambuf<CharT, Traits>* sb);
    virtual ~basic_ios();

    // members
    basic_ostream<CharT, Traits>* tie() const { return m_tie; }
    basic_ostream<CharT, Traits>* tie(basic_ostream<CharT, Traits>* tiestr) {
        basic_ostream<CharT, Traits>* prev = m_tie;
        m_tie = tiestr;
        return prev;
    }

    basic_streambuf<CharT, Traits>* rdbuf() const;

    char_type widen(char c) const;

private:
    iostate m_state = goodbit;
    iostate m_exceptions = goodbit;

    basic_streambuf<CharT, Traits>* m_rdbuf = nullptr;
    basic_ostream<CharT, Traits>* m_tie = nullptr;
};

// flags functions
template <class CharT, class Traits>
result<void> basic_ios<CharT, Traits>::setstate(iostate state) {
    m_state |= state;

    if ((m_state & m_exceptions) != 0) {
        return std::make_exception_ptr(ios_base::failure("basic_ios::setstate: exception mask matched"));
    }

    return {};
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