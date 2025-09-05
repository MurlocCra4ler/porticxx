#include <bits/arch/arch.hpp>
#include <iostream>

namespace std {

class cerr_streambuf : public basic_streambuf<char> {
public:
protected:
    int sync() override {
        ptrdiff_t n = pptr() - pbase();
        if (n > 0) {
            if (arch::current_arch::stderr_write(pbase(), n) !=
                static_cast<size_t>(n)) {
                return -1;
            }
            pbump(-n);
        }
        return 0;
    }

    int_type overflow(int_type c = traits_type::eof()) override {
        if (sync() == -1)
            return traits_type::eof();
        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            *pptr() = traits_type::to_char_type(c);
            pbump(1);
        }
        return c;
    }
};

static cerr_streambuf cerr_sb;
ostream cerr(&cerr_sb);

} // namespace std
