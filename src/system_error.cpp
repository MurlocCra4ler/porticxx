#include <system_error>

namespace std {

system_error::system_error(error_code ec, const string& what_arg) : runtime_error(what_arg) {}

system_error::system_error(error_code ec, const char* what_arg) : runtime_error(what_arg) {}

const char* system_error::what() const noexcept {
    return "system_error";
}

}