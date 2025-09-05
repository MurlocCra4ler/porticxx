#pragma once

#include <bits/iterator/adaptor_classes.hpp>

namespace std {

/*********************/
/* Adaptor Functions */
/*********************/

template <class Container>
std::insert_iterator<Container> inserter(Container& c,
                                         typename Container::iterator i) {
    return std::insert_iterator<Container>(c, i);
}

template <class Container>
std::back_insert_iterator<Container> back_inserter(Container& c) {
    return std::back_insert_iterator<Container>(c);
}

} // namespace std