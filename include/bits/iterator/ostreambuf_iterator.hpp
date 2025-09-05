#pragma once

#include <bits/iterator/primitives.hpp>
#include <bits/streambuf/basic_streambuf.hpp>
#include <bits/string/char_traits.hpp>

namespace std {

template <class CharT, class Traits> class basic_ostream;

template <class CharT, class Traits = char_traits<CharT>>
class ostreambuf_iterator {
public:
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = ptrdiff_t;
    using pointer = void;
    using reference = void;
    using char_type = CharT;
    using traits_type = Traits;
    using streambuf_type = basic_streambuf<CharT, Traits>;
    using ostream_type = basic_ostream<CharT, Traits>;

    constexpr ostreambuf_iterator() noexcept = default;
    ostreambuf_iterator(ostream_type& s) noexcept : sbuf_(s.rdbuf()) {}
    ostreambuf_iterator(streambuf_type* s) noexcept : sbuf_(s) {}
    ostreambuf_iterator& operator=(CharT c) {
        if (!sbuf_ || Traits::eq_int_type(sbuf_->sputc(c), Traits::eof())) {
            sbuf_ = nullptr;
        }
        return *this;
    }

    ostreambuf_iterator& operator*() { return *this; }
    ostreambuf_iterator& operator++() { return *this; }
    ostreambuf_iterator& operator++(int) { return *this; }
    bool failed() const noexcept { return sbuf_ == nullptr; }

private:
    streambuf_type* sbuf_ = nullptr;
};

} // namespace std