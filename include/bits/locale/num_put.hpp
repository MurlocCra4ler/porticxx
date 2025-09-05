#pragma once

#include <bits/ios/ios_base.hpp>
#include <bits/iterator/ostreambuf_iterator.hpp>
#include <bits/locale/locale.hpp>

namespace std {

template <class CharT, class OutputIter = ostreambuf_iterator<CharT>>
class num_put : public locale::facet {
public:
    typedef CharT char_type;
    typedef OutputIter iter_type;

    explicit num_put(size_t refs = 0) {}

    iter_type put(iter_type s, ios_base& f, char_type fill, bool v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill, long v) const { return do_put(s, f, fill, v); }
    iter_type put(iter_type s, ios_base& f, char_type fill, long long v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill,
                  unsigned long v) const { return do_put(s, f, fill, v); }
    iter_type put(iter_type s, ios_base& f, char_type fill,
                  unsigned long long v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill, double v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill,
                  long double v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill,
                  const void* v) const;

    static locale::id id;

protected:
    ~num_put();

    virtual iter_type do_put(iter_type s, ios_base&, char_type fill,
                             bool v) const { return s; }
    virtual iter_type do_put(iter_type s, ios_base&, char_type,
                             long v) const {
        char buf[32];
        char* p = buf + sizeof(buf);

        bool neg = v < 0;
        unsigned long val = neg ? -static_cast<unsigned long>(v) : static_cast<unsigned long>(v);

        do {
            *--p = '0' + (val % 10);
            val /= 10;
        } while (val != 0);

        if (neg) *--p = '-';

        for (; p < buf + sizeof(buf); ++p) {
            *s++ = *p;
            if (s.failed()) break;
        }

        return s;
    }
    virtual iter_type do_put(iter_type s, ios_base&, char_type fill,
                             long long v) const { return s; }
    virtual iter_type do_put(iter_type s, ios_base&, char_type fill,
                             unsigned long) const { return s; }
    virtual iter_type do_put(iter_type s, ios_base&, char_type fill,
                             unsigned long long) const { return s; }
    virtual iter_type do_put(iter_type s, ios_base&, char_type fill,
                             double v) const { return s; }
    virtual iter_type do_put(iter_type s, ios_base&, char_type fill,
                             long double v) const { return s; }
    virtual iter_type do_put(iter_type s, ios_base&, char_type fill,
                             const void* v) const { return s; }
};

} // namespace std