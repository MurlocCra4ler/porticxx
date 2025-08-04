#pragma once

#include <bits/memory/allocators.hpp>
#include <bits/iterator/primitives.hpp>
#include <bits/string/char_traits.hpp>

namespace std {

template<class CharT, class Traits = char_traits<CharT>>
class basic_string_view {
public:
    // types
    using Traits_type            = Traits;
    using value_type             = CharT;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using const_iterator         = const CharT*;
    using iterator               = const_iterator;
    //using const_reverse_iterator = reverse_iterator<const_iterator>;
    //using reverse_iterator       = const_reverse_iterator;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    static constexpr size_type npos = size_type(-1);

    // construction and assignment
    constexpr basic_string_view() noexcept;
    constexpr basic_string_view(const basic_string_view&) noexcept = default;
    constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;
    constexpr basic_string_view(const CharT* str);
    constexpr basic_string_view(nullptr_t) = delete;
    constexpr basic_string_view(const CharT* str, size_type len);
    template<class It, class End>
        constexpr basic_string_view(It begin, End end);
    template<class R>
        constexpr explicit basic_string_view(R&& r);

    // iterator support
    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    //constexpr const_reverse_iterator rbegin() const noexcept;
    //constexpr const_reverse_iterator rend() const noexcept;
    //constexpr const_reverse_iterator crbegin() const noexcept;
    //constexpr const_reverse_iterator crend() const noexcept;

    // capacity
    constexpr size_type size() const noexcept;
    constexpr size_type length() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr bool empty() const noexcept;

    // element access
    constexpr const_reference operator[](size_type pos) const;
    constexpr const_reference at(size_type pos) const;
    constexpr const_reference front() const;
    constexpr const_reference back() const;
    constexpr const_pointer data() const noexcept;

    // modifiers
    constexpr void remove_prefix(size_type n);
    constexpr void remove_suffix(size_type n);
    constexpr void swap(basic_string_view& s) noexcept;

    // string operations
    constexpr size_type copy(CharT* s, size_type n, size_type pos = 0) const;

    constexpr basic_string_view substr(size_type pos = 0, size_type n = npos) const;

    constexpr int compare(basic_string_view s) const noexcept;
    constexpr int compare(size_type pos1, size_type n1, basic_string_view s) const;
    constexpr int compare(size_type pos1, size_type n1, basic_string_view s,
                            size_type pos2, size_type n2) const;
    constexpr int compare(const CharT* s) const;
    constexpr int compare(size_type pos1, size_type n1, const CharT* s) const;
    constexpr int compare(size_type pos1, size_type n1, const CharT* s,
                            size_type n2) const;

    constexpr bool starts_with(basic_string_view x) const noexcept;
    constexpr bool starts_with(CharT x) const noexcept;
    constexpr bool starts_with(const CharT* x) const;
    constexpr bool ends_with(basic_string_view x) const noexcept;
    constexpr bool ends_with(CharT x) const noexcept;
    constexpr bool ends_with(const CharT* x) const;

    constexpr bool contains(basic_string_view x) const noexcept;
    constexpr bool contains(CharT x) const noexcept;
    constexpr bool contains(const CharT* x) const;

    // searching
    constexpr size_type find(basic_string_view s, size_type pos = 0) const noexcept;
    constexpr size_type find(CharT c, size_type pos = 0) const noexcept;
    constexpr size_type find(const CharT* s, size_type pos, size_type n) const;
    constexpr size_type find(const CharT* s, size_type pos = 0) const;
    constexpr size_type rfind(basic_string_view s, size_type pos = npos) const noexcept;
    constexpr size_type rfind(CharT c, size_type pos = npos) const noexcept;
    constexpr size_type rfind(const CharT* s, size_type pos, size_type n) const;
    constexpr size_type rfind(const CharT* s, size_type pos = npos) const;

    constexpr size_type find_first_of(basic_string_view s,
                                        size_type pos = 0) const noexcept;
    constexpr size_type find_first_of(CharT c, size_type pos = 0) const noexcept;
    constexpr size_type find_first_of(const CharT* s, size_type pos, size_type n) const;
    constexpr size_type find_first_of(const CharT* s, size_type pos = 0) const;
    constexpr size_type find_last_of(basic_string_view s,
                                        size_type pos = npos) const noexcept;
    constexpr size_type find_last_of(CharT c, size_type pos = npos) const noexcept;
    constexpr size_type find_last_of(const CharT* s, size_type pos, size_type n) const;
    constexpr size_type find_last_of(const CharT* s, size_type pos = npos) const;
    constexpr size_type find_first_not_of(basic_string_view s,
                                            size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of(const CharT* s, size_type pos,
                                            size_type n) const;
    constexpr size_type find_first_not_of(const CharT* s, size_type pos = 0) const;
    constexpr size_type find_last_not_of(basic_string_view s,
                                        size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of(const CharT* s, size_type pos,
                                            size_type n) const;
    constexpr size_type find_last_not_of(const CharT* s, size_type pos = npos) const;

private:
    const_pointer data_;
    size_type size_;
};

}