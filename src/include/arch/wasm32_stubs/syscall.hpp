#pragma once

#include <cstddef>

namespace wasm32_stubs {


[[noreturn]] void exit(int exit_code);

size_t write(int fd, void* buf, size_t size);

size_t read(int fd, void* buf, size_t buf_size);

}
