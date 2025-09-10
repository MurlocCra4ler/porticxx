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

    path() noexcept = default;
    path(const path& p) : data_(p.data_) {}
    path(path&& p) noexcept : data_(std::move(p.data_)) {}
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

    // non-member operators
    friend bool operator==(const path& lhs, const path& rhs) noexcept { return lhs.data_ == rhs.data_; }
    friend strong_ordering operator<=>(const path& lhs, const path& rhs) noexcept;

    // native format observers
    const string_type& native() const noexcept;
    const value_type* c_str() const noexcept;
    operator string_type() const;

    template <class ECharT, class Traits = char_traits<ECharT>,
              class Allocator = allocator<ECharT>>
    basic_string<ECharT, Traits, Allocator>
    string(const Allocator& a = Allocator()) const;
    std::string string() const { return data_; }
    std::wstring wstring() const;
    std::u8string u8string() const;
    std::u16string u16string() const;
    std::u32string u32string() const;

private:
    string_type data_;

    template <class CharT, class Traits>
    friend basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& os, const path& p) {
        return os << p.data_;
    }
};

} // namespace std::filesystem
