#include <arch.hpp>
#include <arch/wasm32_stubs/syscall.hpp>

namespace arch {

void exit(int exit_code) { wasm32_stubs::exit(exit_code); }

} // namespace arch