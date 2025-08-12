#include <bits/exception/exception.hpp>

namespace std {

const char* exception::what() const noexcept {
    return "exception";    
}

}
