#include <cstring>

namespace std {

std::size_t strlen(const char* start) {
    const char* end = start;
    while (*end != '\0') ++end;
    return end - start;
}

}