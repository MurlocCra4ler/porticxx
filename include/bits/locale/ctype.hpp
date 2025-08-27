#pragma once

#include <bits/locale/locale.hpp>
#include <bits/locale/ctype_base.hpp>

namespace std {

template <class CharT>
class ctype : public locale::facet, public ctype_base {
public:
    bool is(mask m, CharT c) const {
        if ((m & space) && (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v'))
            return true;
        if ((m & digit) && (c >= '0' && c <= '9'))
            return true;
        if ((m & alpha) && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
            return true;
        return false;
    }
};

}