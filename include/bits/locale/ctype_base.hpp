#pragma once

#include <cstdint>

namespace std {

class ctype_base {
public:
    typedef uint16_t mask;

    static const mask space = 1 << 0;
    static const mask print = 1 << 1;
    static const mask cntrl = 1 << 2;
    static const mask upper = 1 << 3;
    static const mask lower = 1 << 4;
    static const mask alpha = 1 << 5;
    static const mask digit = 1 << 6;
    static const mask punct = 1 << 7;
    static const mask xdigit = 1 << 8;
    static const mask blank = 1 << 9;
    static const mask alnum = alpha | digit;
    static const mask graph = alnum | punct;
};

} // namespace std