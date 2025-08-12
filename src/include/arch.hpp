#pragma once

#include <cstddef>
#include <bits/exception/exception_ptr.hpp>

namespace arch {

// Writes `size` bytes from `data` to the output device (e.g. stdout).
// Returns number of bytes actually written, or -1 on error.
std::ptrdiff_t write_out(const void* data, std::size_t size);

// Reads up to `size` bytes into `data` from the input device (e.g. stdin).
// Returns number of bytes actually read, or -1 on error.
std::ptrdiff_t read_in(void* data, std::size_t size);

// Exits the program.
[[noreturn]] void exit(int exit_code);


// Aborts the program with an exception.
[[noreturn]] void abort_exception(std::exception_ptr err);

}
