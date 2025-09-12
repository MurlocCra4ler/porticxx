#include <cctype>

namespace std {

int isalnum(int c) { return isalpha(c) || isdigit(c); }

int isalpha(int c) { return islower(c) || isupper(c); }

int isdigit(int c) {
    if (c >= '0' && c <= '9')
        return c;
    return 0;
}

int islower(int c) {
    if (c >= 'a' && c <= 'z')
        return c;
    return 0;
}

int isupper(int c) {
    if (c >= 'A' && c <= 'Z')
        return c;
    return 0;
}

int isspace(int c) {
    if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' ||
        c == '\v')
        return c;
    return 0;
}

} // namespace std