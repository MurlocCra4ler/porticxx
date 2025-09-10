#pragma once

#include <bits/arch/arch.hpp>
#include <bits/filesystem/directory_entry.hpp>

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
    explicit directory_iterator(const path& p) : cur_(p) {}
    // directory_iterator(const path& p, directory_options options);
    directory_iterator(const path& p, error_code& ec);
    // directory_iterator(const path& p, directory_options options,
    //                    error_code& ec);
    directory_iterator(const directory_iterator& rhs)
        : cur_(rhs.cur_), nativ_stream_(rhs.nativ_stream_) {}
    directory_iterator(directory_iterator&& rhs) noexcept
        : cur_(std::move(rhs.cur_)),
          nativ_stream_(std::move(rhs.nativ_stream_)), at_end_(rhs.at_end_) {
        rhs.at_end_ = true;
    }
    ~directory_iterator() = default;

    directory_iterator& operator=(const directory_iterator& rhs);
    directory_iterator& operator=(directory_iterator&& rhs) noexcept;

    const directory_entry& operator*() const { return cur_; }
    const directory_entry* operator->() const { return &cur_; }
    directory_iterator& operator++() {
        if (arch::current_arch::fs_read_dir(nativ_stream_,
                                            cur_.native_entry_)) {
            at_end_ = false;
        } else {
            at_end_ = true;
        }
        return *this;
    }
    directory_iterator& increment(error_code& ec);

    bool operator==(const directory_iterator& rhs) const noexcept {
        if (at_end_ && rhs.at_end_) return true;
        if (at_end_ != rhs.at_end_) return false;
        return cur_ == rhs.cur_;
    }

private:
    directory_entry cur_;
    arch::current_arch::dir_stream_type nativ_stream_;
    bool at_end_ = false;
};

inline directory_iterator begin(directory_iterator iter) noexcept {
    return iter;
}

inline directory_iterator end(directory_iterator) noexcept { return {}; }

} // namespace std::filesystem
