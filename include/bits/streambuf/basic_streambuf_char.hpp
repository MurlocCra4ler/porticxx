#pragma once

#include <bits/streambuf/basic_streambuf.hpp>

namespace std {

template<>
class basic_streambuf<char, char_traits<char>> {
public:
    using char_type   = char;
    using int_type    = typename char_traits<char>::int_type;
    using pos_type    = typename char_traits<char>::pos_type;
    using off_type    = typename char_traits<char>::off_type;
    using traits_type = char_traits<char>;
 
    basic_streambuf() { 
        // Get-Area
        m_eback = m_gbuffer;
        m_gptr = m_gbuffer;
        m_egptr = m_gbuffer;

        // Put-Area
        m_pbase = m_pbuffer;
        m_pptr = m_pbuffer;
        m_epptr = m_pbuffer + sizeof(m_pbuffer);
    }

    virtual ~basic_streambuf() = default;

    // buffer and positioning
    int pubsync() { return sync(); }

    // get and put areas
    // get area
    streamsize in_avail();
    int_type snextc() {
        return traits_type::eq_int_type(sbumpc(), traits_type::eof())
            ? traits_type::eof()
            : sgetc();
    }
    
    int_type sbumpc() {
        if (gptr() < egptr()) {
            typename char_traits<char>::int_type c = char_traits<char>::to_int_type(*gptr());
            gbump(1);
            return c;
        }

        return uflow();
    }

    int_type sgetc() { return gptr() < egptr() ? traits_type::to_int_type(*gptr()) : underflow(); }
    streamsize sgetn(char_type* s, streamsize n);

    // put area
    int_type sputc(char_type c) {
        if (m_pptr < m_epptr) {
            *m_pptr++ = c;
            return traits_type::to_int_type(c);
        }
        return overflow(traits_type::to_int_type(c));
    }

    streamsize sputn(const char_type* s, streamsize n);

protected:
    static constexpr size_t BUFFER_SIZE = 1024;

    char m_gbuffer[BUFFER_SIZE];
    char m_pbuffer[BUFFER_SIZE];

    // get area access
    char_type* eback() const { return m_eback; }
    char_type* gptr()  const { return m_gptr; }
    char_type* egptr() const { return m_egptr; }
    void gbump(int n) { m_gptr += n; }
    void setg(char_type* gbeg, char_type* gnext, char_type* gend) { 
        m_eback = gbeg;
        m_gptr = gnext;
        m_egptr = gend;
    }

    // put area access
    char_type* pbase() const { return m_pbase; }
    char_type* pptr() const { return m_pptr; }
    char_type* epptr() const { return m_epptr; }
    void pbump(int n) { m_pptr += n; }

    // buffer management and positioning
    virtual int sync() { return 0; }

    // get area
    virtual int_type underflow() { return traits_type::eof(); }
    virtual int_type uflow() {
        int_type c = underflow();
        if (!traits_type::eq_int_type(c, traits_type::eof())) gbump(1);
        return c;
    }

    // put area
    virtual int_type overflow(int_type c = traits_type::eof()) {
        if (traits_type::eq_int_type(c, traits_type::eof()))
            return traits_type::not_eof(c);

        return traits_type::to_int_type(c);
    }

private:
    char* m_eback;
    char* m_gptr;
    char* m_egptr;

    char* m_pbase;
    char* m_pptr;
    char* m_epptr;
};

}
