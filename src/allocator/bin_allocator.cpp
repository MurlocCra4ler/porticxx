#include <cstdint>

namespace std::impl {

constexpr uint16_t PAGE_SIZE = 1 << 10;
constexpr uint16_t NUM_PAGES = 1 << 14;
constexpr uint32_t MEM_SIZE = PAGE_SIZE * NUM_PAGES;

struct memory_descriptor {
    int idx;
    size_t size;
    void* memory;
};

class bin_allocator {
public:
    static bin_allocator& instance() {
        static bin_allocator allocater;
        return allocater;
    }

    bool allocate(uint32_t size, memory_descriptor& result);

    bool grow(memory_descriptor& memory);

    void shrink(memory_descriptor& memory);

    void free(memory_descriptor& memory);

private:
    static constexpr uint32_t NUM_NODES = (NUM_PAGES << 1) - 1;

    struct NodeDescriptor {
        uint16_t free_pages;

        static int get_left(int idx) { return 2 * idx + 1; };
        static int get_right(int idx) { return 2 * idx + 2; };
        static int get_parent(int idx) { return (idx - 1) / 2; };
    };

    NodeDescriptor nodes[NUM_NODES];
    uint8_t heap[MEM_SIZE];

    bin_allocator();
};

bin_allocator::bin_allocator() {
    for (uint32_t i = 0; i < NUM_NODES; i++) {
        uint32_t level = 0;
        for (uint32_t idx = i + 1; idx > 1; idx >>= 1) {
            level++;
        }

        nodes[i].free_pages = NUM_PAGES >> level;
    }
}

bool bin_allocator::allocate(uint32_t size, memory_descriptor& result) {}

void bin_allocator::free(memory_descriptor& memory) {
    uint16_t pages_allocated = memory.size / PAGE_SIZE;

    int idx = memory.idx;
    NodeDescriptor* node = &nodes[idx];
    node->free_pages += pages_allocated;

    while (idx != 0) {
        idx = NodeDescriptor::get_parent(idx);
        node = &nodes[idx];
        node->free_pages += pages_allocated;
    }
}

bool bin_allocator::grow(memory_descriptor& memory) {
    if (memory.idx == 0) {
        return false;
    }

    int right_idx =
        NodeDescriptor::get_right(NodeDescriptor::get_parent(memory.idx));
    if (right_idx == memory.idx) {
        return false;
    }

    uint16_t allocated_pages = memory.size / PAGE_SIZE;
    NodeDescriptor& right_node = nodes[right_idx];

    if (right_node.free_pages != allocated_pages) {
        return false;
    }

    int node_idx = memory.idx;
    NodeDescriptor* node = &nodes[node_idx];
    node->free_pages += allocated_pages;

    while (node_idx != 0) {
        node_idx = NodeDescriptor::get_parent(node_idx);
        node = &nodes[node_idx];
        node->free_pages -= allocated_pages;
    }

    memory.idx = NodeDescriptor::get_parent(memory.idx);
    memory.size <<= 1;

    return true;
}

void bin_allocator::shrink(memory_descriptor& memory) {
    int left_idx = NodeDescriptor::get_left(memory.idx);

    if (left_idx >= NUM_NODES) {
        return;
    }

    int node_idx = left_idx;
    NodeDescriptor* node = &nodes[node_idx];

    uint32_t allocated_pages = node->free_pages;
    node->free_pages = 0;

    while (node_idx != 0) {
        node_idx = NodeDescriptor::get_parent(node_idx);
        node = &nodes[node_idx];
        node->free_pages += allocated_pages;
    }

    memory.idx = left_idx;
    memory.size = allocated_pages * PAGE_SIZE;
}

} // namespace std::impl
