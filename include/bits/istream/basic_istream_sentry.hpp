#pragma once

#include <bits/istream/basic_istream.hpp>

namespace std {

template <class CharT, class Traits>
class basic_istream<CharT, Traits>::sentry {
public:
    explicit sentry(basic_istream& is, bool noskipws = false) : m_ok(false) {
        if (!is.good())
            return;
        if (auto* tied = is.tie())
            tied->flush();

        if (!noskipws && (is.flags() & ios_base::skipws)) {
            auto& inbuf = *is.rdbuf();
            typename Traits::int_type c = inbuf.sgetc();
            while (!Traits::eq_int_type(c, Traits::eof()) &&
                   isspace(Traits::to_char_type(c), is.getloc())) {
                c = inbuf.snextc();
            }
            if (Traits::eq_int_type(c, Traits::eof())) {
                is.setstate(ios_base::eofbit | ios_base::failbit);
                return;
            }
        }

        m_ok = true;
    }

    ~sentry() = default;
    explicit operator bool() const { return m_ok; }
    sentry(const sentry&) = delete;
    sentry& operator=(const sentry&) = delete;

private:
    bool m_ok;
};

} // namespace std