#include <bits/ostream/basic_ostream.hpp>

using namespace std;

template <>
basic_ostream<char>::basic_ostream(basic_streambuf<char>* sb)
    : basic_ios<char>(sb) {}

template <> basic_ostream<char>::~basic_ostream() = default;

template <>
basic_ostream<char>&
basic_ostream<char>::operator<<(basic_ostream& (*pf)(basic_ostream&)) {
    return pf(*this);
}

template <>
result_ref<basic_ostream<char>> basic_ostream<char>::write(const char_type* s,
                                                           streamsize n) {
    if (!this->rdbuf() || this->rdbuf()->sputn(s, n) != n) {
        return this->setstate(std::ios_base::badbit).map_to_ref(*this);
    }

    return *this;
}
