#include <bits/variant/bad_variant_access.hpp>

namespace std {

constexpr const char* bad_variant_access::what() const noexcept {
    return "bad_variant_access";
}

} // namespace std