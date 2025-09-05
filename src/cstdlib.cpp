#include <bits/arch/arch.hpp>
#include <cstdlib>

namespace std {

[[noreturn]] void abort() noexcept { arch::current_arch::abort(); }

} // namespace std