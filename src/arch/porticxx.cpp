#include <arch.hpp>

extern "C" [[noreturn]] void porticxx_abort_exception(std::exception_ptr err) {
    arch::abort_exception(err);
}
