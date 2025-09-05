#pragma once

#include <exception>

namespace std {

class bad_variant_access : public exception {
public:
    // see description for the specification of the special member functions
    const char* what() const noexcept override;
};

} // namespace std
