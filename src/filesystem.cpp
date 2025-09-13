#include <filesystem>

namespace std::filesystem {

path current_path() {
    constexpr size_t PATH_LEN = arch::current_arch::max_path_len();
    char buffer[PATH_LEN];

    arch::current_arch::fs_dir_get_current(buffer);
    return buffer;
}

void current_path(const path& p) {
    arch::current_arch::fs_dir_set_current(p.c_str());
}

} // namespace std::filesystem