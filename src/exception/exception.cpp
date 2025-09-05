#include <bits/exception/exception.hpp>

namespace std {

const char* exception::what() const noexcept { return "exception"; }

[[noreturn]] void terminate() noexcept { __builtin_trap(); }

} // namespace std
