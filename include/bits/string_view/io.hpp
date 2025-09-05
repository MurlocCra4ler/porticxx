#pragma once

#include <bits/ostream/basic_ostream.hpp>
#include <bits/string_view/basic_string_view.hpp>

namespace std {

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           std::basic_string_view<CharT, Traits> v) {
    for (auto c : v) {
        const basic_ostream<CharT>& result = os.put(c);
        if (result.fail())
            break;
    }

    return os;
}

} // namespace std
