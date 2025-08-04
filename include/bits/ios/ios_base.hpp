#pragma once

#include <cstdint>
#include <system_error>

namespace std {

/***********************/
/* Class std::ios_base */
/***********************/

class ios_base {
public:
    class failure;

    // iostate
    using iostate = uint32_t;
    static constexpr iostate badbit     = 1 << 0;
    static constexpr iostate eofbit     = 1 << 1;
    static constexpr iostate failbit    = 1 << 2;
    static constexpr iostate goodbit    = 0;
};

enum class io_errc {
    stream = 1
};

class ios_base::failure : public system_error {
public:
    explicit failure(const string& msg, const error_code& ec = io_errc::stream);
    explicit failure(const char* msg, const error_code& ec = io_errc::stream);
};

}