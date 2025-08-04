#include <cstdint>

namespace std::impl {

constexpr uint16_t PAGE_SIZE = 1 << 12;
constexpr uint16_t NUM_PAGES = 1 << 12;
constexpr uint32_t MEM_SIZE = PAGE_SIZE * NUM_PAGES;

struct MemoryDescriptor {
    int idx;
    uint32_t size;
    void* memory;
};

class BinaryAllocator {
public:
    static BinaryAllocator& instance() {
        static BinaryAllocator allocater;
        return allocater;
    }

    bool allocate(uint32_t size, MemoryDescriptor& result);

    bool grow(MemoryDescriptor& memory);

    void shrink(MemoryDescriptor& memory);

    void free(MemoryDescriptor& memory);

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

    BinaryAllocator();
};

BinaryAllocator::BinaryAllocator() {
    for (uint32_t i = 0; i < NUM_NODES; i++) {
        uint32_t level = 0;
        for (uint32_t idx = i + 1; idx > 1; idx >>= 1) {
            level++;
        }

        nodes[i].free_pages = NUM_PAGES >> level;
    }
}

bool BinaryAllocator::allocate(uint32_t size, MemoryDescriptor& result) {
    if (size == 0 || size > MEM_SIZE) {
        return false;
    }

    uint16_t pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    int idx = 0;
    NodeDescriptor* node = &nodes[idx];
    uint32_t offset_pages = 0;
    uint16_t block_size = NUM_PAGES;

    if (node->free_pages < pages_needed) {
        return false;
    }   

    while (true) {
        int left_idx = NodeDescriptor::get_left(idx);
        int right_idx = NodeDescriptor::get_right(idx);

        if (left_idx >= NUM_NODES) break; 

        NodeDescriptor* left = &nodes[left_idx];
        NodeDescriptor* right = &nodes[right_idx];
        block_size >>= 1;

        if (left->free_pages >= right->free_pages) {
            if (left->free_pages < pages_needed) {
                break;
            }

            idx = left_idx;
            node = left;
        } else {
            if (right->free_pages < pages_needed) {
                break;
            }

            idx = right_idx;
            node = right;
            offset_pages += block_size;
        }
    }

    result.idx = idx;
    result.size = node->free_pages * PAGE_SIZE;
    result.memory = &heap[offset_pages * PAGE_SIZE];;

    uint16_t pages_allocated = node->free_pages;
    node->free_pages = 0;

    while (idx != 0) {
        idx = NodeDescriptor::get_parent(idx);
        node = &nodes[idx];
        node->free_pages -= pages_allocated;
    }
    
    return true;
}

void BinaryAllocator::free(MemoryDescriptor& memory) {
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

bool BinaryAllocator::grow(MemoryDescriptor& memory) {
    if (memory.idx == 0) {
        return false;
    }

    int right_idx = NodeDescriptor::get_right(NodeDescriptor::get_parent(memory.idx));
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

void BinaryAllocator::shrink(MemoryDescriptor& memory) {
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

}
