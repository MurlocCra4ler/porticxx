#include <bits/arch/arch.hpp>
#include <cstdlib>

namespace std {

[[noreturn]] void abort() noexcept { arch::current_arch::abort(); }

[[noreturn]] void exit(int exit_code) { arch::current_arch::exit(exit_code); }

} // namespace std