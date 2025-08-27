#pragma once

#include <cstddef>
#include <cstdint>

#include <bits/arch/arch.hpp>

namespace std::impl_allocator {

struct memory_descriptor {
    int idx;
    size_t size;
    void* ptr;
};

template<typename Arch = arch::current_arch>
class bin_allocator {
public:
    static bin_allocator& instance() {
        static bin_allocator allocater;
        return allocater;
    }

    bool allocate(memory_descriptor& result) {
        int idx = 0;
        node_descriptor* node = &nodes[idx];
        uint32_t offset_pages = 0;
        size_t block_size = Arch::num_pages();

        if (node->free_pages < 1) {
            return false;
        }   

        while (true) {
            int left_idx = node_descriptor::get_left(idx);
            int right_idx = node_descriptor::get_right(idx);

            if (left_idx >= NUM_NODES) break; 

            node_descriptor* left = &nodes[left_idx];
            node_descriptor* right = &nodes[right_idx];
            block_size >>= 1;

            if (left->free_pages >= right->free_pages) {
                if (left->free_pages < 1) {
                    break;
                }

                idx = left_idx;
                node = left;
            } else {
                if (right->free_pages < 1) {
                    break;
                }

                idx = right_idx;
                node = right;
                offset_pages += block_size;
            }
        }

        result.idx = idx;
        result.size = node->free_pages * Arch::page_size();
        result.ptr = &m_heap[offset_pages * Arch::page_size()];;

        uint16_t pages_allocated = node->free_pages;
        node->free_pages = 0;

        while (idx != 0) {
            idx = node_descriptor::get_parent(idx);
            node = &nodes[idx];
            node->free_pages -= pages_allocated;
        }
        
        return true;
    }

    bool grow(memory_descriptor& memory) {
        if (memory.idx == 0) {
            return false;
        }

        int right_idx = node_descriptor::get_right(node_descriptor::get_parent(memory.idx));
        if (right_idx == memory.idx) {
            return false;
        }

        uint16_t allocated_pages = memory.size / Arch::page_size();
        node_descriptor& right_node = nodes[right_idx];

        if (right_node.free_pages != allocated_pages) {
            return false;
        }

        int node_idx = memory.idx;
        node_descriptor* node = &nodes[node_idx];
        node->free_pages += allocated_pages;

        while (node_idx != 0) {
            node_idx = node_descriptor::get_parent(node_idx);
            node = &nodes[node_idx];
            node->free_pages -= allocated_pages;
        }

        memory.idx = node_descriptor::get_parent(memory.idx);
        memory.size <<= 1;
        
        return true;
    }

    void shrink(memory_descriptor& memory);

    void free(memory_descriptor& memory);

private:
    static constexpr uint32_t NUM_NODES = (Arch::num_pages() << 1) - 1;

    struct node_descriptor {
        uint32_t free_pages;

        static int get_left(int idx) { return 2 * idx + 1; };
        static int get_right(int idx) { return 2 * idx + 2; };
        static int get_parent(int idx) { return (idx - 1) / 2; };
    };

    uint8_t* m_heap;
    node_descriptor nodes[NUM_NODES];

    bin_allocator() {
        m_heap = Arch::heap();

        for (uint32_t i = 0; i < NUM_NODES; i++) {
            uint32_t level = 0;
            for (uint32_t idx = i + 1; idx > 1; idx >>= 1) {
                level++;
            }

            nodes[i].free_pages = Arch::num_pages() >> level;
        }
    }
};

}
