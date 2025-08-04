#pragma once

#include <cstddef>
#include <bits/type_traits/helpers.hpp>
#include <bits/type_traits/misc_transformations.hpp>
#include <bits/type_traits/sign_mods.hpp>
#include <bits/memory/pointer_traits.hpp>

namespace std {

/**************/
/* Allocators */
/**************/

template<class Pointer, class SizeType = size_t>
struct allocation_result {
    Pointer ptr;
    SizeType count;
};

template<class T>
class allocator {
public:
    using value_type                             = T;
    using size_type                              = size_t;
    using difference_type                        = ptrdiff_t;
    using propagate_on_container_move_assignment = true_type;
 
    constexpr allocator() noexcept;
    constexpr allocator(const allocator&) noexcept;
    template<class U>
    constexpr allocator(const allocator<U>&) noexcept;
    constexpr ~allocator();
    constexpr allocator& operator=(const allocator&) = default;
 
    constexpr T* allocate(size_t n);
    constexpr allocation_result<T*> allocate_at_least(size_t n);
    constexpr void deallocate(T* p, size_t n);
};

namespace impl_allocators {

// pointer_type
template <typename Alloc, typename = void>
struct pointer_type {
    using type = typename Alloc::value_type*; // fallback
};

template <typename Alloc>
struct pointer_type<Alloc, std::void_t<typename Alloc::pointer>> {
    using type = typename Alloc::pointer;
};

template <typename Alloc>
using pointer_type_t = typename pointer_type<Alloc>::type;

// const_pointer_type
template <typename Alloc, typename = void>
struct const_pointer_type {
    using type = std::pointer_traits<pointer_type_t<Alloc>>::template rebind<const typename Alloc::value_type>; // fallback
};

template <typename Alloc>
struct const_pointer_type<Alloc, std::void_t<typename Alloc::const_pointer_type>> {
    using type = typename Alloc::const_pointer_type;
};

template <typename Alloc>
using const_pointer_type_t = typename const_pointer_type<Alloc>::type;

// difference_type
template <typename Alloc, typename = void>
struct difference_type {
    using type = std::pointer_traits<pointer_type_t<Alloc>>::difference_type; // fallback
};

template <typename Alloc>
struct difference_type<Alloc, std::void_t<typename Alloc::difference_type>> {
    using type = typename Alloc::difference_type;
};

template <typename Alloc>
using difference_type_t = typename difference_type<Alloc>::type;

// size_type
template <typename Alloc, typename = void>
struct size_type {
    using type = make_unsigned_t<difference_type_t<Alloc>>; // fallback
};

template <typename Alloc>
struct size_type<Alloc, std::void_t<typename Alloc::size_type>> {
    using type = typename Alloc::size_type;
};

template <typename Alloc>
using size_type_t = typename size_type<Alloc>::type;

}

template<class Alloc>
struct allocator_traits {
public:
    using allocator_type = Alloc;
 
    using value_type = typename Alloc::value_type;
 
    using pointer           = impl_allocators::pointer_type_t<Alloc>;
    using const_pointer     = impl_allocators::const_pointer_type_t<Alloc>;
    //using void_pointer                           = /* see description */;
    //using const_void_pointer                     = /* see description */;
 
    using difference_type   = impl_allocators::difference_type_t<Alloc>;
    using size_type         = impl_allocators::size_type_t<Alloc>;
 
    //using propagate_on_container_copy_assignment = /* see description */;
    //using propagate_on_container_move_assignment = /* see description */;
    //using propagate_on_container_swap            = /* see description */;
    //using is_always_equal                        = /* see description */;
};

}