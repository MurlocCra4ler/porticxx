
#pragma once

#include <cstring>

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
    static constexpr size_type npos = size_type(-1);

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
    constexpr basic_string(const CharT* s, size_type count,
                           const Allocator& a = Allocator())
        : allocator_(a), size_(0) {
        append(s, count);
    }
    basic_string(std::nullptr_t) = delete;
    basic_string(const basic_string& other)
        : size_(other.size_), capacity_(other.capacity_),
          using_inplace_(other.using_inplace_) {
        if (using_inplace_) {
            // copy inplace buffer
            for (size_type i = 0; i < size_; ++i)
                inplace_[i] = other.inplace_[i];
            inplace_[size_] = Traits::to_char_type(0);
        } else {
            // allocate heap and copy
            data_ = allocator_.allocate(capacity_);
            for (size_type i = 0; i < size_; ++i)
                data_[i] = other.data_[i];
            data_[size_] = Traits::to_char_type(0);
        }
    }
    basic_string(basic_string&& other) noexcept
        : allocator_(std::move(other.allocator_)), size_(other.size_),
          capacity_(other.capacity_), using_inplace_(other.using_inplace_) {
        if (using_inplace_) {
            // copy inplace buffer
            for (size_type i = 0; i < size_; ++i)
                inplace_[i] = other.inplace_[i];
            inplace_[size_] = CharT(0);
        } else {
            // move heap pointer
            data_ = other.data_;

            // clear other
            other.data_ = nullptr;
            other.using_inplace_ = true;
            other.capacity_ = INPLACE_SIZE - 1;
        }

        other.size_ = 0;
        other.inplace_[0] = Traits::to_char_type(0);
    }

    basic_string& operator=(const basic_string& other) {
        if (this == &other)
            return *this;

        // free memory if heap is used
        if (!using_inplace_ && data_) {
            allocator_.deallocate(data_, capacity_);
        }

        size_ = other.size_;
        capacity_ = other.capacity_;
        using_inplace_ = other.using_inplace_;

        if (using_inplace_) {
            for (size_type i = 0; i < size_; ++i)
                inplace_[i] = other.inplace_[i];
            inplace_[size_] = CharT(0);
        } else {
            data_ = allocator_.allocate(capacity_);
            for (size_type i = 0; i < size_; ++i)
                data_[i] = other.data_[i];
            data_[size_] = CharT(0);
        }

        return *this;
    }

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
    constexpr size_type capacity() const noexcept { return capacity_; }
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

    constexpr const CharT& front() const;
    constexpr CharT& front();
    constexpr const CharT& back() const;
    constexpr CharT& back() { return data()[size_ - 1]; }

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
    constexpr basic_string& operator+=(const basic_string& str) {
        return append(str);
    }
    template <class T> constexpr basic_string& operator+=(const T& t);
    constexpr basic_string& operator+=(const CharT* s) {
        size_type n = strlen(s);
        return append(s, n);
    }
    constexpr basic_string& operator+=(CharT c) {
        push_back(c);
        return *this;
    }
    // constexpr basic_string& operator+=(initializer_list<CharT>);
    constexpr basic_string& append(const basic_string& str) {
        append(str.c_str(), str.size_);
        return *this;
    }
    constexpr basic_string& append(const basic_string& str, size_type pos,
                                   size_type n = npos);

    template <class T> constexpr basic_string& append(const T& t);
    template <class T>
    constexpr basic_string& append(const T& t, size_type pos,
                                   size_type n = npos);
    constexpr basic_string& append(const CharT* s, size_type n) {
        if (capacity_ < size_ + n) {
            grow(size_ + n);
        }

        arch::current_arch::memcpy(&data()[size_], s, n);
        size_ += n;
        data()[size_] = Traits::to_char_type(0);
        return *this;
    }
    constexpr basic_string& append(const CharT* s) {
        size_type n = strlen(s);
        append(s, n);
        return *this;
    }
    constexpr basic_string& append(size_type n, CharT c);
    template <class InputIt>
    constexpr basic_string& append(InputIt first, InputIt last);
    /* template <container-compatible-range<CharT> R>
    constexpr basic_string& append_range(R&& rg);
    constexpr basic_string& append(initializer_list<CharT>); */

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

    constexpr basic_string& erase(size_type pos = 0, size_type n = npos) {
        // clamp pos
        if (pos > size_)
            pos = size_;

        // clamp n
        if (n == npos || pos + n > size_)
            n = size_ - pos;

        // shift remaining characters
        for (size_type i = pos; i < size_ - n; ++i) {
            data()[i] = data()[i + n];
        }

        size_ -= n;
        data()[size_] = Traits::to_char_type(0);
        return *this;
    }
    constexpr iterator erase(const_iterator p);
    constexpr iterator erase(const_iterator first, const_iterator last);

    // string operations
    constexpr const CharT* c_str() const noexcept { return data(); }
    constexpr const CharT* data() const noexcept {
        return using_inplace_ ? inplace_ : data_;
    }
    constexpr CharT* data() noexcept {
        return using_inplace_ ? inplace_ : data_;
    }
    constexpr operator basic_string_view<CharT, Traits>() const noexcept {
        return std::basic_string_view<CharT, Traits>(data(), size());
    }

    constexpr basic_string substr(size_type pos = 0,
                                  size_type n = npos) const& {
        if (pos > size_)
            pos = size_;
        if (n > size_ - pos)
            n = size_ - pos;
        return basic_string().append(c_str() + pos, n);
    }
    constexpr basic_string substr(size_type pos = 0, size_type n = npos) && {
        if (pos > size_)
            pos = size_;
        if (n > size_ - pos)
            n = size_ - pos;

        size_ = 0;
        return basic_string().append(c_str() + pos, n);
    }

private:
    static constexpr size_type INPLACE_SIZE = 1 << 6;

    Allocator allocator_;

    union {
        CharT* data_;
        CharT inplace_[INPLACE_SIZE];
    };

    size_type size_ = 0;
    size_type capacity_ = INPLACE_SIZE - 1;
    bool using_inplace_ = true;

    constexpr void grow(size_type new_cap = 0) {
        size_t n = (new_cap > 0) ? new_cap : (capacity_ + 1) << 1;
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

template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(const basic_string<CharT, Traits, Allocator>& lhs,
          const basic_string<CharT, Traits, Allocator>& rhs) {
    std::basic_string<CharT, Traits, Allocator> r = lhs;
    r.append(rhs);
    return r;
}
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(basic_string<CharT, Traits, Allocator>&& lhs,
          const basic_string<CharT, Traits, Allocator>& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(const basic_string<CharT, Traits, Allocator>& lhs,
          basic_string<CharT, Traits, Allocator>&& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(basic_string<CharT, Traits, Allocator>&& lhs,
          basic_string<CharT, Traits, Allocator>&& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(const CharT* lhs, const basic_string<CharT, Traits, Allocator>& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(const CharT* lhs, basic_string<CharT, Traits, Allocator>&& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(CharT lhs, const basic_string<CharT, Traits, Allocator>& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(CharT lhs, basic_string<CharT, Traits, Allocator>&& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(const basic_string<CharT, Traits, Allocator>& lhs, const CharT* rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(basic_string<CharT, Traits, Allocator>&& lhs, const CharT* rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(const basic_string<CharT, Traits, Allocator>& lhs, CharT rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(basic_string<CharT, Traits, Allocator>&& lhs, CharT rhs);
/*
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(const basic_string<CharT, Traits, Allocator>& lhs,
          type_identity_t<basic_string_view<CharT, Traits>> rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(basic_string<CharT, Traits, Allocator>&& lhs,
          type_identity_t<basic_string_view<CharT, Traits>> rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(type_identity_t<basic_string_view<CharT, Traits>> lhs,
          const basic_string<CharT, Traits, Allocator>& rhs);
template <class CharT, class Traits, class Allocator>
constexpr basic_string<CharT, Traits, Allocator>
operator+(type_identity_t<basic_string_view<CharT, Traits>> lhs,
          basic_string<CharT, Traits, Allocator>&& rhs); */

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