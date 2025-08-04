#pragma once

#include <bits/string/char_traits.hpp>

namespace std {

template<class CharT, class Traits = char_traits<CharT>>
class basic_streambuf {
public:
    using char_type   = CharT;
    using int_type    = typename Traits::int_type;
    using pos_type    = typename Traits::pos_type;
    using off_type    = typename Traits::off_type;
    using traits_type = Traits;
 
    virtual ~basic_streambuf();

    // buffer and positioning
    int pubsync();

    // put area
    int_type sputc(char_type c);
    streamsize sputn(const char_type* s, streamsize n);

protected:
    // put area access
    char_type* pbase() const;
    char_type* pptr() const;
    char_type* epptr() const;
    void pbump(int n);

    // buffer management and positioning
    virtual int sync() { return 0; }

    // put area
    virtual int_type overflow(int_type c = Traits::eof());

private:
    char_type* m_pbase = nullptr;
    char_type* m_pptr = nullptr;
    char_type* m_epptr = nullptr;
};

template<class CharT, class Traits>
basic_streambuf<CharT, Traits>::~basic_streambuf() = default;

// public: buffer and positioning
template<class CharT, class Traits>
int basic_streambuf<CharT, Traits>::pubsync() {
    return sync();
}

// public: put area
template<class CharT, class Traits>
basic_streambuf<CharT, Traits>::int_type basic_streambuf<CharT, Traits>::sputc(char_type c) {
    if (pptr() < epptr()) {
        *pptr() = c;
        pbump(1);
        return traits_type::to_int_type(c);
    } else {
        return overflow(traits_type::to_int_type(c));
    }
}

template<class CharT, class Traits>
streamsize basic_streambuf<CharT, Traits>::sputn(const char_type* s, streamsize n) {
    streamsize written = 0;
    for (; written < n; ++written) {
        if (traits_type::eq_int_type(overflow(traits_type::to_int_type(s[written])), traits_type::eof())) {
            break; 
        }
    }

    return written;
}

// protected: put area access
template<class CharT, class Traits>
basic_streambuf<CharT, Traits>::char_type* basic_streambuf<CharT, Traits>::pbase() const {
    return m_pbase;
}

template<class CharT, class Traits>
basic_streambuf<CharT, Traits>::char_type* basic_streambuf<CharT, Traits>::pptr() const {
    return m_pptr;
}

template<class CharT, class Traits>
basic_streambuf<CharT, Traits>::char_type* basic_streambuf<CharT, Traits>::epptr() const {
    return m_epptr;
}

template<class CharT, class Traits>
void basic_streambuf<CharT, Traits>::pbump(int n) {
    m_pptr += n;
}

// protected: put area
template<class CharT, class Traits>
basic_streambuf<CharT, Traits>::int_type basic_streambuf<CharT, Traits>::overflow(int_type c) {
    if (traits_type::eq_int_type(c, traits_type::eof())) {
        return traits_type::not_eof(c);
    }
    
    return c;
}

}
