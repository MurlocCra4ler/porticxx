#pragma once

#include <bits/ios/ios_base.hpp>
#include <bits/streambuf/basic_streambuf.hpp>
#include <bits/string/char_traits.hpp>

namespace std {

template <class CharT, class Traits> class basic_ostream;

template <class CharT, class Traits = char_traits<CharT>>
class basic_ios : public ios_base {
public:
    using char_type = CharT;
    using int_type = typename Traits::int_type;
    using pos_type = typename Traits::pos_type;
    using off_type = typename Traits::off_type;
    using traits_type = Traits;

    // flags functions
    explicit operator bool() const { return !fail(); }
    void clear(iostate state = goodbit) {
        state_ = rdbuf() ? state : state | ios_base::badbit;
    }
    result<void> setstate(iostate state) {
        state_ |= state;

        if ((state_ & exceptions_) != 0) {
            return std::make_exception_ptr(ios_base::failure(
                "basic_ios::setstate: exception mask matched"));
        }

        return {};
    }
    bool good() const { return (state_ & (failbit | badbit | eofbit)) == 0; }
    bool eof() const { return (state_ & eofbit) != 0; }
    bool fail() const { return (state_ & (failbit | badbit)) != 0; }
    bool bad() const { return (state_ & badbit) != 0; }

    // constructor/destructor
    explicit basic_ios(basic_streambuf<CharT, Traits>* sb) { init(sb); }
    virtual ~basic_ios() = default;

    // members
    basic_ostream<CharT, Traits>* tie() const { return tie_; }
    basic_ostream<CharT, Traits>* tie(basic_ostream<CharT, Traits>* tiestr) {
        basic_ostream<CharT, Traits>* prev = tie_;
        tie_ = tiestr;
        return prev;
    }

    basic_streambuf<CharT, Traits>* rdbuf() const { return rdbuf_; }
    basic_streambuf<CharT, Traits>* rdbuf(basic_streambuf<CharT, Traits>* sb) {
        basic_streambuf<CharT, Traits>* old = rdbuf_;
        rdbuf_ = sb;
        clear();
        return old;
    }

    char_type fill() const { return fill_; }
    char_type fill(char_type ch) {
        char_type prev = fill_;
        fill_ = ch;
        return prev;
    }

    char_type widen(char c) const {
        return std::use_facet<std::ctype<char_type>>(this->getloc()).widen(c);
    }

    basic_ios(const basic_ios&) = delete;
    basic_ios& operator=(const basic_ios&) = delete;

protected:
    basic_ios() = default;
    void init(basic_streambuf<CharT, Traits>* sb) {
        rdbuf(sb);
        tie(nullptr);
        flags(std::ios_base::skipws | std::ios_base::dec);
        width(0);
        precision(6);

        fill_ = this->widen(' ');
    }

private:
    iostate state_;
    iostate exceptions_;

    basic_streambuf<CharT, Traits>* rdbuf_;
    basic_ostream<CharT, Traits>* tie_;

    char_type fill_;
};

} // namespace std
