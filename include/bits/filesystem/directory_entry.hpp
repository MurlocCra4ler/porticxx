#pragma once

#include <bits/arch/arch.hpp>
#include <bits/filesystem/path.hpp>

namespace std::filesystem {

class directory_entry {
public:
    // constructors and destructor
    directory_entry() noexcept = default;
    directory_entry(const directory_entry&) = default;
    directory_entry(directory_entry&&) noexcept = default;
    explicit directory_entry(const filesystem::path& p) : path_(p) {}
    directory_entry(const filesystem::path& p, error_code& ec);
    ~directory_entry() = default;

    // assignments
    directory_entry& operator=(const directory_entry&) = default;
    directory_entry& operator=(directory_entry&&) noexcept = default;

    // modifiers
    void assign(const filesystem::path& p);
    void assign(const filesystem::path& p, error_code& ec);
    void replace_filename(const filesystem::path& p);
    void replace_filename(const filesystem::path& p, error_code& ec);
    void refresh();
    void refresh(error_code& ec) noexcept;

    // observers
    const filesystem::path& path() const noexcept { return path_; }
    operator const filesystem::path&() const noexcept;
    bool exists() const;
    bool exists(error_code& ec) const noexcept;
    bool is_block_file() const;
    bool is_block_file(error_code& ec) const noexcept;
    bool is_character_file() const;
    bool is_character_file(error_code& ec) const noexcept;
    bool is_directory() const;
    bool is_directory(error_code& ec) const noexcept;
    bool is_fifo() const;
    bool is_fifo(error_code& ec) const noexcept;
    bool is_other() const;
    bool is_other(error_code& ec) const noexcept;
    bool is_regular_file() const;
    bool is_regular_file(error_code& ec) const noexcept;
    bool is_socket() const;
    bool is_socket(error_code& ec) const noexcept;
    bool is_symlink() const;
    bool is_symlink(error_code& ec) const noexcept;
    // uintmax_t file_size() const;
    // uintmax_t file_size(error_code& ec) const noexcept;
    // uintmax_t hard_link_count() const;
    // uintmax_t hard_link_count(error_code& ec) const noexcept;
    // file_time_type last_write_time() const;
    // file_time_type last_write_time(error_code& ec) const noexcept;
    // file_status status() const;
    // file_status status(error_code& ec) const noexcept;
    // file_status symlink_status() const;
    // file_status symlink_status(error_code& ec) const noexcept;

    bool operator==(const directory_entry& rhs) const noexcept {
        return path_ == rhs.path_;
    }
    strong_ordering operator<=>(const directory_entry& rhs) const noexcept;

    // inserter
    template <class CharT, class Traits>
    friend basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& os, const directory_entry& d);

private:
    filesystem::path path_;

    friend class directory_iterator; // exposition only
};

} // namespace std::filesystem