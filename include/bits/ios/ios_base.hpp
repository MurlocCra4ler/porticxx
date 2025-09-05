#pragma once

#include <cstdint>
#include <locale>
#include <system_error>

namespace std {

/***********************/
/* Class std::ios_base */
/***********************/

class ios_base {
public:
    class failure;

    // fmtflags
    using fmtflags = uint16_t;
    static constexpr fmtflags boolalpha = 1 << 0;
    static constexpr fmtflags dec = 1 << 1;
    static constexpr fmtflags fixed = 1 << 2;
    static constexpr fmtflags hex = 1 << 3;
    static constexpr fmtflags internal = 1 << 4;
    static constexpr fmtflags left = 1 << 5;
    static constexpr fmtflags oct = 1 << 6;
    static constexpr fmtflags right = 1 << 7;
    static constexpr fmtflags scientific = 1 << 8;
    static constexpr fmtflags showbase = 1 << 9;
    static constexpr fmtflags showpoint = 1 << 10;
    static constexpr fmtflags showpos = 1 << 11;
    static constexpr fmtflags skipws = 1 << 12;
    static constexpr fmtflags unitbuf = 1 << 13;
    static constexpr fmtflags uppercase = 1 << 14;
    static constexpr fmtflags adjustfield = left | right | internal;
    static constexpr fmtflags basefield = dec | oct | hex;
    static constexpr fmtflags floatfield = scientific | fixed;

    // iostate
    using iostate = uint8_t;
    static constexpr iostate badbit = 1 << 0;
    static constexpr iostate eofbit = 1 << 1;
    static constexpr iostate failbit = 1 << 2;
    static constexpr iostate goodbit = 0;

    // fmtflags state
    fmtflags flags() const { return flags_; }
    fmtflags flags(fmtflags fmtfl) {
        fmtflags old = flags_;
        flags_ = fmtfl;
        return old;
    }
    fmtflags setf(fmtflags fmtfl);
    fmtflags setf(fmtflags fmtfl, fmtflags mask);
    void unsetf(fmtflags mask);

    streamsize precision() const;
    streamsize precision(streamsize prec) {
        streamsize old = prec_;
        prec_ = prec;
        return old;
    }
    streamsize width() const;
    streamsize width(streamsize wide) {
        streamsize old = width_;
        width_ = wide;
        return old;
    }

    // locales
    locale imbue(const locale& loc) {
        locale old = loc_;
        loc_ = loc;
        return old;
    }
    locale getloc() const { return loc_; }

private:
    fmtflags flags_;
    streamsize prec_;
    streamsize width_;
    locale loc_;
};

enum class io_errc { stream = 1 };

class ios_base::failure : public system_error {
public:
    explicit failure(const string& msg, const error_code& ec = io_errc::stream);
    explicit failure(const char* msg, const error_code& ec = io_errc::stream);
};

} // namespace std