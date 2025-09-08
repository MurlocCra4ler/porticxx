#pragma once

#include <bits/ostream/basic_ostream.hpp>
#include <bits/string/basic_string.hpp>
#include <bits/string/io.hpp>

namespace std::filesystem {

class path {
public:
    using value_type = char;
    using string_type = basic_string<value_type>;

    enum format { native_format, generic_format, auto_format };

    path() noexcept;
    path(const path& p);
    path(path&& p) noexcept;
    path(string_type&& source, format fmt = auto_format);
    template <class Source>
    path(const Source& source, format fmt = auto_format) : data_(source) {}
    template <class InputIt>
    path(InputIt first, InputIt last, format fmt = auto_format);
    template <class Source>
    path(const Source& source, const locale& loc, format fmt = auto_format);
    template <class InputIt>
    path(InputIt first, InputIt last, const locale& loc,
         format fmt = auto_format);
    ~path() = default;

private:
    string_type data_;

    template <class CharT, class Traits>
    friend basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& os, const path& p) {
        return os << p.data_;
    }
};

} // namespace std::filesystem
