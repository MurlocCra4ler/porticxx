#include <cstdint>
#include <memory>

int main() {
    std::allocator<int> alloc;
    int* ptr = alloc.allocate(1);
    ptr[0] = 0;

    for (int i = 1; i < INT16_MAX; i++) {
        int* new_ptr = alloc.allocate(i + 1);
        __builtin_memcpy(new_ptr, ptr, i * sizeof(int));
        new_ptr[i] = i;
        alloc.deallocate(ptr, i);
        ptr = new_ptr;
    }

    for (int i = 0; i < INT16_MAX; i++) {
        if (ptr[i] != i) {
            return -1;
        }
    }

    return 0;
}
