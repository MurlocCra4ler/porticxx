#pragma once

#include <bits/ios/fwd.hpp>
#include <cstddef>

namespace std {

/***********/
/* Classes */
/***********/

using streamoff = std::ptrdiff_t;
using streamsize = std::size_t;

template <class StateT> class fpos {
public:
    StateT state() const;

    void state(StateT);

private:
    StateT m_st;
};

using streampos = fpos<mbstate_t>;

} // namespace std
