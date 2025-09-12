#pragma once

namespace std {

/******************/
/* Sign modifiers */
/******************/

// make_signed
template <class T> struct make_signed;

template <class T> using make_signed_t = typename make_signed<T>::type;

// make_unsigned
template <class T> struct make_unsigned;

template <> struct make_unsigned<long> {
    using type = unsigned long;
};
template <> struct make_unsigned<unsigned long> {
    using type = unsigned long;
};

template <class T> using make_unsigned_t = typename make_unsigned<T>::type;

} // namespace std