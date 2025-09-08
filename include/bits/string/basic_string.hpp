
#pragma once

#include <bits/arch/arch.hpp>
#include <bits/iterator/adaptor_classes.hpp>
#include <bits/memory/allocators.hpp>
#include <bits/string/char_traits.hpp>
#include <bits/string_view/basic_string_view.hpp>

namespace std {

template <class CharT, class Traits = char_traits<CharT>,
          class Allocator = allocator<CharT>>
class basic_string {
public:
    using traits_type = Traits;
    using value_type = CharT;
    using allocator_type = Allocator;
    using size_type = typename allocator_traits<Allocator>::size_type;
    using difference_type =
        typename allocator_traits<Allocator>::difference_type;
    using pointer = typename allocator_traits<Allocator>::pointer;
    using const_pointer = typename allocator_traits<Allocator>::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;

    using iterator = CharT*;
    using const_iterator = const CharT*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr basic_string() noexcept(noexcept(Allocator()))
        : basic_string(Allocator()) {}
    constexpr explicit basic_string(const Allocator& a)
        : allocator_(a), size_(0), capacity_(INPLACE_SIZE - 1),
          using_inplace_(true) {
        inplace_[0] = Traits::to_char_type(0);
    }

    constexpr basic_string(const CharT* s, const Allocator& a = Allocator())
        : allocator_(a), size_(0) {
        while (s[size_] != Traits::to_char_type(0)) {
            ++size_;
        }

        if (size_ <= INPLACE_SIZE) {
            std::arch::current_arch::memcpy(inplace_, s, size_ + 1);
            capacity_ = INPLACE_SIZE - 1;
            using_inplace_ = true;
        } else {
            data_ = allocator_.allocate(size_ + 1);
            std::arch::current_arch::memcpy(data_, s, size_ + 1);
            capacity_ = size_;
            using_inplace_ = false;
        }
    }
    // constexpr basic_string(const CharT* s, size_type count, const Allocator&
    // alloc = Allocator());
    constexpr basic_string(size_type n, CharT c,
                           const Allocator& a = Allocator());

    // iterators
    constexpr iterator begin() noexcept {
        return using_inplace_ ? inplace_ : data_;
    }
    constexpr const_iterator begin() const noexcept {
        return using_inplace_ ? inplace_ : data_;
    }
    constexpr iterator end() noexcept { return begin() + size_; }
    constexpr const_iterator end() const noexcept { return begin() + size_; }

    // capacity
    constexpr size_type size() const noexcept { return size_; }
    constexpr size_type length() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr void resize(size_type n, CharT c);
    constexpr void resize(size_type n);
    template <class Operation>
    constexpr void resize_and_overwrite(size_type n, Operation op);
    constexpr size_type capacity() const noexcept;
    constexpr void reserve(size_type res_arg);

    // element access
    constexpr const_reference operator[](size_type pos) const {
        return using_inplace_ ? inplace_[pos] : data_[pos];
    }
    constexpr reference operator[](size_type pos) {
        return using_inplace_ ? inplace_[pos] : data_[pos];
    }
    constexpr const_reference at(size_type n) const;
    constexpr reference at(size_type n);

    constexpr void shrink_to_fit() {
        if (using_inplace_) {
            return;
        }

        if (size_ < INPLACE_SIZE) {
            arch::current_arch::memcpy(inplace_, data_, size_ + 1);

            allocator_.deallocate(data_, capacity_ + 1);
            using_inplace_ = true;
            capacity_ = INPLACE_SIZE - 1;
        } else if (capacity_ > size_) {
            CharT* ptr = allocator_.allocate(size_ + 1);
            arch::current_arch::memcpy(ptr, data_, size_ + 1);

            allocator_.deallocate(data_, capacity_ + 1);
            data_ = ptr;
            capacity_ = size_;
        }
    }

    constexpr void clear() noexcept {
        if (using_inplace_)
            inplace_[0] = Traits::to_char_type(0);
        else
            data_[0] = Traits::to_char_type(0);
        size_ = 0;
        shrink_to_fit();
    }

    constexpr bool empty() const noexcept { return size_ == 0; }

    // modifiers
    constexpr basic_string& operator+=(CharT c) {
        push_back(c);
        return *this;
    }

    constexpr void push_back(CharT c) {
        if (capacity_ <= size_) {
            grow();
        }

        if (using_inplace_) {
            inplace_[size_++] = c;
            inplace_[size_] = Traits::to_char_type(0);
        } else {
            data_[size_++] = c;
            data_[size_++] = Traits::to_char_type(0);
        }
    }

    // string operations
    constexpr const CharT* c_str() const noexcept;
    constexpr const CharT* data() const noexcept {
        return using_inplace_ ? inplace_ : data_;
    }
    constexpr CharT* data() noexcept {
        return using_inplace_ ? inplace_ : data_;
    }
    constexpr operator basic_string_view<CharT, Traits>() const noexcept {
        return std::basic_string_view<CharT, Traits>(data(), size());
    }

private:
    static constexpr size_t INPLACE_SIZE = 1 << 6;

    Allocator allocator_;

    union {
        CharT* data_;
        CharT inplace_[INPLACE_SIZE];
    };

    size_t size_;
    size_t capacity_;
    bool using_inplace_;

    constexpr void grow() {
        size_t n = (capacity_ + 1) << 1;
        CharT* ptr = allocator_.allocate(n);

        if (using_inplace_) {
            arch::current_arch::memcpy(ptr, inplace_, size_ + 1);
            data_ = ptr;
            using_inplace_ = false;
        } else if (data_ != ptr) {
            arch::current_arch::memcpy(ptr, data_, size_ + 1);
            data_ = ptr;
        }

        capacity_ = n - 1;
    }
};

using string = basic_string<char>;
using u8string = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;
using wstring = basic_string<wchar_t>;

template <class CharT, class Traits, class Alloc>
bool operator==(const std::basic_string<CharT, Traits, Alloc>& lhs,
                const std::basic_string<CharT, Traits, Alloc>& rhs) {
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }

    return true;
}

/* Compare two basic_string objects */
template <class CharT, class Traits, class Alloc>
bool operator!=(const std::basic_string<CharT, Traits, Alloc>& lhs,
                const std::basic_string<CharT, Traits, Alloc>& rhs) {
    return !(lhs == rhs);
}

/* Compare a basic_string object and null-terminated array of T */
template <class CharT, class Traits, class Alloc>
bool operator==(const std::basic_string<CharT, Traits, Alloc>& lhs,
                const CharT* rhs) {
    if (!rhs)
        return false;

    auto it = lhs.begin();
    for (; *rhs != CharT(); ++rhs, ++it) {
        if (it == lhs.end() || !Traits::eq(*it, *rhs)) {
            return false;
        }
    }

    return it = lhs.end();
}

template <class A> struct hash;

template <class A>
struct hash<std::basic_string<char, std::char_traits<char>, A>> {
    using string_type = basic_string<char, std::char_traits<char>, A>;
    using result_type = size_t;

    result_type operator()(const string_type& s) const noexcept {
        uint64_t hash = 0xcbf29ce484222325;
        const uint64_t prime = 0x100000001b3;

        for (char c : s) {
            hash ^= static_cast<unsigned char>(c);
            hash *= prime;
        }
        return static_cast<result_type>(hash);
    }
};

} // namespace std