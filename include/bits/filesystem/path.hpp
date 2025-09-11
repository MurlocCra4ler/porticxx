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
    path(const path& p) = default;
    path(path&& p) noexcept = default;
    path(string_type&& source, format fmt = auto_format)
        : native_(std::move(source)) {}
    template <class Source>
    path(const Source& source, format fmt = auto_format) : native_(source) {}
    template <class InputIt>
    path(InputIt first, InputIt last, format fmt = auto_format);
    template <class Source>
    path(const Source& source, const locale& loc, format fmt = auto_format);
    template <class InputIt>
    path(InputIt first, InputIt last, const locale& loc,
         format fmt = auto_format);
    ~path() = default;

    // assignments
    path& operator=(const path& p) = default;
    path& operator=(path&& p) noexcept = default;
    path& operator=(string_type&& source);
    path& assign(string_type&& source);
    template <class Source> path& operator=(const Source& source);
    template <class Source> path& assign(const Source& source);
    template <class InputIt> path& assign(InputIt first, InputIt last);

    // appends
    path& operator/=(const path& p) {
        // If p is absolute, or has a different root name, replace this path
        // entirely.
        if (p.is_absolute() ||
            (p.has_root_name() && p.root_name() != root_name())) {
            native_ = p.native_;
            return *this;
        }

        // If p starts with a root directory (like "/foo"), keep our root name
        // but discard everything after it, then replace with p.
        if (p.has_root_directory()) {
            auto rn = this->root_directory();
            native_ = rn.native_ + p.native_;
            return *this;
        }

        // Otherwise, if we currently have a filename or we are absolute without
        // a root directory, append a separator if missing.
        if (has_filename() || (!has_root_directory() && is_absolute())) {
            if (!native_.empty() && native_.back() != '/')
                native_ += '/';
        }

        // Append p's path without its root name, if it has one.
        std::string to_append = p.native_;
        if (p.has_root_name()) {
            to_append.erase(0, p.root_name().string().size());
        }
        native_ += to_append;

        return *this;
    }
    template <class Source> path& operator/=(const Source& source);
    template <class Source> path& append(const Source& source);
    template <class InputIt> path& append(InputIt first, InputIt last);

    // non-member operators
    friend bool operator==(const path& lhs, const path& rhs) noexcept {
        return lhs.native_ == rhs.native_;
    }
    friend strong_ordering operator<=>(const path& lhs,
                                       const path& rhs) noexcept;

    friend path operator/(const path& lhs, const path& rhs) {
        return path(lhs) /= rhs;
    }

    // native format observers
    const string_type& native() const noexcept { return native_; }
    const value_type* c_str() const noexcept { return native_.c_str(); }
    operator string_type() const;

    template <class ECharT, class Traits = char_traits<ECharT>,
              class Allocator = allocator<ECharT>>
    basic_string<ECharT, Traits, Allocator>
    string(const Allocator& a = Allocator()) const;
    std::string string() const { return native_; }
    std::wstring wstring() const;
    std::u8string u8string() const;
    std::u16string u16string() const;
    std::u32string u32string() const;

    // decomposition
    path root_name() const {
        const char* rn;
        size_t len;
        arch::current_arch::fs_path_root_name(native_.c_str(), &rn, &len);
        return path(std::string(rn, len));
    }
    path root_directory() const {
        const char* rd;
        size_t len;
        arch::current_arch::fs_path_root_dir(native_.c_str(), &rd, &len);
        return path(std::string(rd, len));
    }
    path root_path() const;
    path relative_path() const;
    path parent_path() const;
    path filename() const {
        const char* f;
        size_t len;
        arch::current_arch::fs_path_filename(native_.c_str(), &f, &len);
        return path(std::string(f, len));
    }
    path stem() const;
    path extension() const;

    // query
    bool empty() const noexcept;
    bool has_root_name() const {
        return arch::current_arch::fs_path_root_name(native_.c_str(), nullptr,
                                                     nullptr);
    }
    bool has_root_directory() const {
        return arch::current_arch::fs_path_root_dir(native_.c_str(), nullptr,
                                                    nullptr);
    }
    bool has_root_path() const;
    bool has_relative_path() const;
    bool has_parent_path() const;
    bool has_filename() const {
        return arch::current_arch::fs_path_filename(native_.c_str(), nullptr,
                                                    nullptr);
    }
    bool has_stem() const;
    bool has_extension() const;
    bool is_absolute() const {
        return arch::current_arch::fs_path_is_absolute(native_.c_str());
    }
    bool is_relative() const { return !is_absolute(); }

private:
    string_type native_;

    template <class CharT, class Traits>
    friend basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& os, const path& p) {
        return os << p.native_;
    }
};

} // namespace std::filesystem
