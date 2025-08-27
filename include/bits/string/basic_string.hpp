
#pragma once

#include <bits/memory/allocators.hpp>
#include <bits/string/char_traits.hpp>
#include <bits/iterator/adaptor_classes.hpp>
#include <bits/string_view/basic_string_view.hpp>

namespace std {

template<
    class CharT,
    class Traits = char_traits<CharT>,
    class Allocator = allocator<CharT>
>
class basic_string {
public:
    using traits_type            = Traits;
    using value_type             = CharT;
    using allocator_type         = Allocator;
    using size_type              = typename allocator_traits<Allocator>::size_type;
    using difference_type        = typename allocator_traits<Allocator>::difference_type;
    using pointer                = typename allocator_traits<Allocator>::pointer;
    using const_pointer          = typename allocator_traits<Allocator>::const_pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
 
    using iterator               = CharT*;
    using const_iterator         = const CharT*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator()) {}
    constexpr explicit basic_string(const Allocator& a)
        : m_allocator(a), m_size(0), m_capacity(sizeof(m_inplace) - 1), m_using_inplace(true) {
        m_inplace[0] = Traits::to_char_type(0);
    }
    
    constexpr basic_string(const CharT* s, const Allocator& a = Allocator());
    //constexpr basic_string(const CharT* s, size_type count, const Allocator& alloc = Allocator());
    constexpr basic_string(size_type n, CharT c, const Allocator& a = Allocator());

    // iterators
    constexpr iterator       begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator       end() noexcept;
    constexpr const_iterator end() const noexcept;

    // capacity
    constexpr size_type size() const noexcept { return m_size; }
    constexpr size_type length() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr void resize(size_type n, CharT c);
    constexpr void resize(size_type n);
    template<class Operation> constexpr void resize_and_overwrite(size_type n, Operation op);
    constexpr size_type capacity() const noexcept;
    constexpr void reserve(size_type res_arg);
    
    constexpr void shrink_to_fit() {
        if (m_using_inplace) {
            return;
        }

        if (m_size < INPLACE_SIZE) {
            __builtin_memcpy(m_inplace, m_data, m_size + 1);
            
            m_allocator.deallocate(m_data, m_capacity + 1);
            m_using_inplace = true;
            m_capacity = INPLACE_SIZE - 1;
        } else if (m_capacity > m_size) {
            CharT* ptr = m_allocator.allocate(m_size + 1);
            __builtin_memcpy(ptr, m_data, m_size + 1);

            m_allocator.deallocate(m_data, m_capacity + 1);
            m_data = ptr;
            m_capacity = m_size;
        }
    }

    constexpr void clear() noexcept {
        if (m_using_inplace) m_inplace[0] = Traits::to_char_type(0);
        else m_data[0] = Traits::to_char_type(0);
        m_size = 0;
        shrink_to_fit();
    }

    constexpr bool empty() const noexcept { return m_size == 0; }

    // modifiers
    constexpr basic_string& operator+=(CharT c) {
        push_back(c);
        return *this;
    }

    constexpr void push_back(CharT c) {
        if (m_capacity <= m_size) {
            grow();
        }

        if (m_using_inplace) {
            m_inplace[m_size++] = c;
            m_inplace[m_size] = Traits::to_char_type(0);
        } else {
            m_data[m_size++] = c;
            m_data[m_size++] = Traits::to_char_type(0);
        }
    }

    // string operations
    constexpr const CharT* c_str() const noexcept;
    constexpr const CharT* data() const noexcept { return m_using_inplace ? m_inplace : m_data; }
    constexpr CharT* data() noexcept { return m_using_inplace ? m_inplace : m_data; }
    constexpr operator basic_string_view<CharT, Traits>() const noexcept {
        return std::basic_string_view<CharT, Traits>(data(), size());
    }

private:
    static constexpr size_t INPLACE_SIZE = 1 << 6; 

    Allocator m_allocator;

    union {
        CharT* m_data;
        CharT  m_inplace[4];
    };

    size_t m_size;
    size_t m_capacity;
    bool m_using_inplace;

    constexpr void grow() {
        size_t n = (m_capacity + 1) << 1;
        CharT* ptr = m_allocator.allocate(n);

        if (m_using_inplace) {
            __builtin_memcpy(ptr, m_inplace, m_size + 1);
            m_data = ptr;
            m_using_inplace = false;
        } else if (m_data != ptr) {
            __builtin_memcpy(ptr, m_data, m_size + 1);
            m_data = ptr;
        }

        m_capacity = n - 1;
    }
};

using string    = basic_string<char>;
using u8string  = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;
using wstring   = basic_string<wchar_t>;

}