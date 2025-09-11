#include <cstring>

namespace std {

size_t strlen(const char* start) {
    const char* end = start;
    while (*end != '\0')
        ++end;
    return end - start;
}

char* strncpy(char* dest, const char* src, size_t count) {
    size_t i = 0;
    // copy characters
    for (; i < count && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    // null-pad the rest
    for (; i < count; i++) {
        dest[i] = '\0';
    }

    return dest;
}

} // namespace std