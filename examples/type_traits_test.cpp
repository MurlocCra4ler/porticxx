#include <type_traits>

class base {};
class derived : public base {};

int main() {
    constexpr bool is_convertible = std::is_convertible_v<derived*, base*>;
    static_assert(is_convertible, "derived class should be convertible in base class");
}
