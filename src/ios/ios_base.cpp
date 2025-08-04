#include <bits/ios/ios_base.hpp>

namespace std {

ios_base::failure::failure(const string& msg, const error_code& ec) : system_error(ec, msg) {}
ios_base::failure::failure(const char* msg, const error_code& ec) : system_error(ec, msg) {}

}

