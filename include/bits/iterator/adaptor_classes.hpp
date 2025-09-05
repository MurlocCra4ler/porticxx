#pragma once

#include <cstddef>
namespace std {

/*******************/
/* Adaptor Classes */
/*******************/

template <class Iter> class reverse_iterator {};

template <class Container> class back_insert_iterator {
public:
    // using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = Container;

    constexpr back_insert_iterator() noexcept = default;
    constexpr explicit back_insert_iterator(Container& x);
    constexpr back_insert_iterator&
    operator=(const typename Container::value_type& value);
    constexpr back_insert_iterator&
    operator=(typename Container::value_type&& value);

    constexpr back_insert_iterator& operator*();
    constexpr back_insert_iterator& operator++();
    constexpr back_insert_iterator operator++(int);

protected:
    Container* container = nullptr;
};

template <class Container> class insert_iterator {
public:
    // using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = Container;

    insert_iterator() = default;
    // constexpr insert_iterator(Container& x, ranges::iterator_t<Container> i);
    constexpr insert_iterator&
    operator=(const typename Container::value_type& value);
    constexpr insert_iterator&
    operator=(typename Container::value_type&& value);

    constexpr insert_iterator& operator*();
    constexpr insert_iterator& operator++();
    constexpr insert_iterator& operator++(int);

protected:
    Container* container = nullptr;
    // ranges::iterator_t<Container> iter = ranges::iterator_t<Container>();
};

} // namespace std