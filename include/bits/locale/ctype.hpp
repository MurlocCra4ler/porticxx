#pragma once

#include <bits/locale/ctype_base.hpp>
#include <bits/locale/locale.hpp>

namespace std {

template <class CharT> class ctype;

template <> class ctype<char> : public locale::facet, public ctype_base {
public:
    typedef char char_type;

    explicit ctype(size_t refs = 0) {}

    bool is(mask m, char c) const { return do_is(m, c); }
    const char* is(const char* low, const char* high, mask* vec) const;
    const char* scan_is(mask m, const char* low, const char* high) const;
    const char* scan_not(mask m, const char* low, const char* high) const;

    char toupper(char c) const;
    const char* toupper(char* low, const char* high) const;
    char tolower(char c) const;
    const char* tolower(char* low, const char* high) const;

    char widen(char c) const { return do_widen(c); }
    const char* widen(const char* low, const char* high, char* to) const;
    char narrow(char c, char dfault) const;
    const char* narrow(const char* low, const char*, char dfault,
                       char* to) const;

    static locale::id id;

protected:
    ~ctype();
    virtual bool do_is(mask m, char c) const {
        if ((m & space) && (c == ' ' || c == '\n' || c == '\t' || c == '\r' ||
                            c == '\f' || c == '\v'))
            return true;
        if ((m & digit) && (c >= '0' && c <= '9'))
            return true;
        if ((m & alpha) && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
            return true;
        return false;
    }
    /*virtual const char* do_is(const char* low, const char* high,
                              mask* vec) const;
    virtual const char* do_scan_is(mask m, const char* low,
                                   const char* high) const;
    virtual const char* do_scan_not(mask m, const char* low,
                                    const char* high) const;

    virtual char do_toupper(char) const;
    virtual const char* do_toupper(char* low, const char* high) const;
    virtual char do_tolower(char) const;
    virtual const char* do_tolower(char* low, const char* high) const; */
    virtual char do_widen(char c) const { return c; }
    /*virtual const char* do_widen(const char* low, const char* high,
                                 char* dest) const;
    virtual char do_narrow(char, char dfault) const;
    virtual const char* do_narrow(const char* low, const char* high,
                                  char dfault, char* dest) const;*/
};

} // namespace std
