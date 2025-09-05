#pragma once

#include <bits/memory/bin_allocator.hpp>

namespace std::impl_allocator {

template <typename T, typename Arch = arch::current_arch> class obj_allocator {
public:
    static obj_allocator<T>& instance() {
        static obj_allocator<T> allocator;
        return allocator;
    }

    T* allocate(size_t n) {
        int curr = m_free_list_head;
        while (curr != -1) {
            T* ptr;
            if (allocate_from_segment(curr, n, ptr))
                return ptr;
            curr = m_segments[curr].next;
        }

        while (true) {
            size_t old_capacity = m_obj_memory.size / sizeof(T);

            auto& bin_allocator =
                ::std::impl_allocator::bin_allocator<Arch>::instance();
            bin_allocator.grow(m_obj_memory);

            size_t new_capacity = m_obj_memory.size / sizeof(T);
            int new_seg_idx = alloc_segment();
            m_segments[new_seg_idx] = {old_capacity,
                                       new_capacity - old_capacity, -1};
            insert_segment(new_seg_idx, m_segments[new_seg_idx]);

            T* ptr;
            if (allocate_from_segment(new_seg_idx, n, ptr))
                return ptr;

            merge_segments();
        }
    }

    void deallocate(T* ptr, size_t n) {
        size_t start = ptr - m_base;

        int new_seg_idx = alloc_segment();
        segment_t& new_seg = m_segments[new_seg_idx];
        new_seg = {start, n, -1};

        insert_segment(new_seg_idx, new_seg);
        merge_segments();
    }

private:
    static constexpr size_t MAX_SEGMENTS = Arch::max_segments();

    struct segment_t {
        size_t start;
        size_t length;
        int next;
    };

    T* m_base;

    segment_t m_segments[MAX_SEGMENTS];
    int m_free_list_head;
    int m_num_used;

    size_t num_segments;
    int m_free_stack[MAX_SEGMENTS];
    int m_free_stack_top;

    memory_descriptor m_obj_memory;

    obj_allocator() : m_free_list_head(0), m_num_used(1), m_free_stack_top(-1) {
        bin_allocator<Arch>& bin_allocator =
            ::std::impl_allocator::bin_allocator<Arch>::instance();
        bin_allocator.allocate(m_obj_memory);

        m_base = reinterpret_cast<T*>(m_obj_memory.ptr);
        m_segments[0] = {0, m_obj_memory.size / sizeof(T), -1};
    }

    int alloc_segment() {
        if (m_free_stack_top >= 0) {
            return m_free_stack[m_free_stack_top--];
        }

        return m_num_used++;
    }

    void free_segment(int idx) { m_free_stack[++m_free_stack_top] = idx; }

    bool allocate_from_segment(int idx, size_t n, T*& out_ptr) {
        segment_t& seg = m_segments[idx];
        if (seg.length < n)
            return false;

        size_t start = seg.start;
        seg.start += n;
        seg.length -= n;

        if (seg.length == 0) {
            int prev = -1, curr = m_free_list_head;
            while (curr != -1 && curr != idx) {
                prev = curr;
                curr = m_segments[curr].next;
            }
            if (curr != -1) {
                if (prev == -1)
                    m_free_list_head = seg.next;
                else
                    m_segments[prev].next = seg.next;
                free_segment(idx);
            }
        }

        out_ptr = m_base + start;
        return true;
    }

    void insert_segment(int idx, segment_t& seg) {
        if (m_free_list_head == -1 ||
            seg.start < m_segments[m_free_list_head].start) {
            seg.next = m_free_list_head;
            m_free_list_head = idx;
        } else {
            int curr = m_free_list_head;
            while (m_segments[curr].next != -1 &&
                   m_segments[m_segments[curr].next].start < seg.start) {
                curr = m_segments[curr].next;
            }

            seg.next = m_segments[curr].next;
            m_segments[curr].next = idx;
        }
    }

    void merge_segments() {
        int curr = m_free_list_head;
        while (curr != -1 && m_segments[curr].next != -1) {
            segment_t& s1 = m_segments[curr];
            segment_t& s2 = m_segments[s1.next];

            if (s1.start + s1.length == s2.start) {
                s1.length += s2.length;
                int dead = s1.next;
                s1.next = s2.next;
                free_segment(dead);
            } else {
                curr = s1.next;
            }
        }
    }
};

} // namespace std::impl_allocator
