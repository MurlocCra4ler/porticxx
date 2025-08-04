#pragma once

#include <bits/ios/basic_ios.hpp>

namespace std {

/****************************/
/* Class std::basic_ostream */
/****************************/

template<class CharT, class Traits = char_traits<CharT>>
class basic_ostream : virtual public basic_ios<CharT, Traits> {
public:
    using char_type   = CharT;

    // constructor/destructor
    explicit basic_ostream(basic_streambuf<char_type, Traits>* sb);
    virtual ~basic_ostream();

    // formatted output
    basic_ostream& operator<<(basic_ostream& (*pf)(basic_ostream&));
    basic_ostream& operator<<(basic_ios<CharT, Traits>& (*pf)(basic_ios<CharT, Traits>&));
    basic_ostream& operator<<(ios_base& (*pf)(ios_base&));
 
    basic_ostream& operator<<(bool n);
    basic_ostream& operator<<(short n);
    basic_ostream& operator<<(unsigned short n);
    basic_ostream& operator<<(int n);
    basic_ostream& operator<<(unsigned int n);
    basic_ostream& operator<<(long n);
    basic_ostream& operator<<(unsigned long n);
    basic_ostream& operator<<(long long n);
    basic_ostream& operator<<(unsigned long long n);
    basic_ostream& operator<<(float f);
    basic_ostream& operator<<(double f);
    basic_ostream& operator<<(long double f);
 
    basic_ostream& operator<<(const void* p);
    basic_ostream& operator<<(const volatile void* p);
    basic_ostream& operator<<(nullptr_t);
 
    // unformatted output
    basic_ostream& put(char_type c);
    basic_ostream& write(const char_type* s, streamsize n);
 
    basic_ostream& flush();
};

// unformatted output
template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::put(char_type c) {
    if (this->rdbuf()->sputc(c) == Traits::eof()) {
        this->setstate(std::ios_base::badbit);
    }
    return *this;
}

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::flush() {
    if (this->rdbuf()->pubsync() == -1) {
        this->setstate(ios_base::badbit);
    }
    return *this;
}

template<class Traits>
std::basic_ostream<char, Traits>& operator<<(std::basic_ostream<char, Traits>& os, const char* s) {
    if (!s) {
        return os << "(null)";
    }

    return os.write(s, Traits::length(s));
}

}