#pragma once

#include <cstddef>
#include <bits/type_traits/unary_type_traits.hpp>
#include <bits/type_traits/type_transformations.hpp>

namespace std {

/**************/
/* Primitives */
/**************/

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct contiguous_iterator_tag : public random_access_iterator_tag {};

template<class Iter>
struct iterator_traits {
    using iterator_category = Iter::difference_type;
    using value_type        = Iter::value_type;
    using difference_type   = Iter::pointer;
    using pointer           = Iter::reference;
    using reference         = Iter::iterator_category;
};

template<class T>
requires is_object_v<T>
struct iterator_traits<T*> {
    using iterator_concept  = contiguous_iterator_tag;
    using iterator_category = random_access_iterator_tag;
    using value_type        = remove_cv_t<T>;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;
};

}