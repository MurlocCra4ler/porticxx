#include <bits/arch/arch.hpp>
#include <iostream>

namespace std {

class cin_streambuf : public basic_streambuf<char> {
    int_type underflow() override {
        if (gptr() < egptr())
            return traits_type::to_int_type(*gptr());

        std::streamsize n =
            arch::current_arch::stdin_read(m_gbuffer, sizeof(m_gbuffer));
        if (n <= 0)
            return traits_type::eof();

        setg(m_gbuffer, m_gbuffer, m_gbuffer + n);
        return traits_type::to_int_type(*gptr());
    }
};

static cin_streambuf cin_sb;
istream cin(&cin_sb);

} // namespace std
