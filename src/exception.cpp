#include <exception>

namespace std {

const char* exception::what() const noexcept {
    return "exception";    
}

}
