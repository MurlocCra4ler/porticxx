#pragma once

#include <bits/arch/arch_base.hpp>
#include <bits/arch/default_arch.hpp>
#include <bits/arch/x86_64_arch.hpp>

namespace std::arch {

#ifdef ARCH_X86_64
using current_arch = arch_base<x86_64_arch>;
#else
using current_arch = arch_base<default_arch>;
#endif

} // namespace std::arch
