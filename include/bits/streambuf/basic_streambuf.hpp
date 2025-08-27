#pragma once

#include <algorithm>
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

    // get and put areas
    // get area
    streamsize in_avail();
    int_type snextc();
    int_type sbumpc();
    int_type sgetc();
    streamsize sgetn(char_type* s, streamsize n);

    // put area
    int_type sputc(char_type c);
    streamsize sputn(const char_type* s, streamsize n);

protected:
    // get area access
    char_type* eback() const { return m_eback; }
    char_type* gptr()  const { return m_gptr; }
    char_type* egptr() const { return m_egptr; }
    void gbump(int n) { m_gptr += n; }

    // put area access
    char_type* pbase() const { return m_pbase; }
    char_type* pptr() const { return m_pptr; }
    char_type* epptr() const { return m_epptr; }
    void pbump(int n) { m_pptr += n; }

    // buffer management and positioning
    virtual int sync() { return 0; }

    // get area
    virtual int_type underflow();
    virtual int_type uflow();

    // put area
    virtual int_type overflow(int_type c = Traits::eof());

private:
    char_type* m_eback = nullptr;
    char_type* m_gptr = nullptr;
    char_type* m_egptr = nullptr;

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

template<class CharT, class Traits>
typename basic_streambuf<CharT, Traits>::int_type
basic_streambuf<CharT, Traits>::snextc() {
    if (gptr() < egptr()) {
        gbump(1);
        return gptr() < egptr() ? Traits::to_int_type(*gptr()) : uflow();
    }

    return uflow();
}

template<class CharT, class Traits>
typename basic_streambuf<CharT, Traits>::int_type
basic_streambuf<CharT,Traits>::sbumpc() {
    if (gptr() < egptr()) {
        typename Traits::int_type c = Traits::to_int_type(*gptr());
        gbump(1);
        return c;
    }

    return uflow();
}

template<class CharT, class Traits>
typename basic_streambuf<CharT, Traits>::int_type
basic_streambuf<CharT,Traits>::sgetc() {
    if (gptr() < egptr()) return Traits::to_int_type(*gptr());
    return underflow();
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

// protected: put area
template<class CharT, class Traits>
basic_streambuf<CharT, Traits>::int_type basic_streambuf<CharT, Traits>::overflow(int_type c) {
    if (traits_type::eq_int_type(c, traits_type::eof())) {
        return traits_type::not_eof(c);
    }
    
    return c;
}

}
