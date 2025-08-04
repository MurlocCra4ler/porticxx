
#pragma once

#include <bits/memory/allocators.hpp>
#include <bits/string/char_traits.hpp>
#include <bits/iterator/adaptor_classes.hpp>

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
    //using const_pointer          = typename allocator_traits<Allocator>::const_pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
 
    using iterator               = CharT*;
    using const_iterator         = const CharT*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator()) {}
    constexpr explicit basic_string(const Allocator& a) noexcept;
    constexpr basic_string(const CharT* s, const Allocator& a = Allocator());
    //constexpr basic_string(const CharT* s, size_type count, const Allocator& alloc = Allocator());
    constexpr basic_string(size_type n, CharT c, const Allocator& a = Allocator());

    // iterators
    constexpr iterator       begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator       end() noexcept;
    constexpr const_iterator end() const noexcept;

    // modifiers
    constexpr basic_string& operator+=(CharT c);

    // string operations
    constexpr const CharT* c_str() const noexcept;
};

}