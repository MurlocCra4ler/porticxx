#pragma once

#include <bits/arch/arch.hpp>
#include <bits/filesystem/directory_entry.hpp>
#include <filesystem>
#include <system_error>

namespace std::filesystem {

class directory_iterator {
public:
    using iterator_category = input_iterator_tag;
    using value_type = directory_entry;
    using difference_type = ptrdiff_t;
    using pointer = const directory_entry*;
    using reference = const directory_entry&;

    // member functions
    directory_iterator() noexcept = default;
    explicit directory_iterator(const path& p) : path_(p) {
        native_stream_ = std::arch::current_arch::fs_dir_open(p.c_str());
        at_end_ = !std::arch::current_arch::fs_dir_read(native_stream_,
                                                        native_entry_);
        filesystem::path file(
            arch::current_arch::fs_dir_entry_name(native_entry_));
        cur_ = directory_entry(path_ / file);
    }
    // directory_iterator(const path& p, directory_options options);
    directory_iterator(const path& p, error_code& ec);
    // directory_iterator(const path& p, directory_options options,
    //                    error_code& ec);
    directory_iterator(const directory_iterator& rhs) = default;
    directory_iterator(directory_iterator&& rhs) = default;
    ~directory_iterator() = default;

    directory_iterator& operator=(const directory_iterator& rhs);
    directory_iterator& operator=(directory_iterator&& rhs) noexcept;

    const directory_entry& operator*() const { return cur_; }
    const directory_entry* operator->() const { return &cur_; }
    directory_iterator& operator++() {
        if (arch::current_arch::fs_dir_read(native_stream_, native_entry_)) {
            cur_ = directory_entry(
                path_ / arch::current_arch::fs_dir_entry_name(native_entry_));
            at_end_ = false;
        } else {
            at_end_ = true;
        }
        return *this;
    }
    directory_iterator& increment(error_code& ec);

    bool operator==(const directory_iterator& rhs) const noexcept {
        if (at_end_ && rhs.at_end_)
            return true;
        if (at_end_ != rhs.at_end_)
            return false;
        return cur_ == rhs.cur_;
    }

private:
    filesystem::path path_;
    directory_entry cur_;
    arch::current_arch::dir_stream_type native_stream_;
    arch::current_arch::dir_entry_type native_entry_;
    bool at_end_ = true;
};

inline directory_iterator begin(directory_iterator iter) noexcept {
    return iter;
}

inline directory_iterator end(directory_iterator) noexcept { return {}; }

} // namespace std::filesystem
