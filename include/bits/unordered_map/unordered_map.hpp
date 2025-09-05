#pragma once

#include <bits/functional/equal_to.hpp>
#include <bits/memory/allocators.hpp>
#include <iterator>

namespace std {

template <class Key> struct hash;

namespace _impl_unordered_map {

template <typename Key, typename T> struct node {
    using value_type = pair<const Key, T>;

    value_type value;
    node* next = nullptr;

    template <class... Args>
    node(Args&&... args) : value(std::forward<Args>(args)...) {}
};

template <typename Node> class node_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename Node::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    reference operator*() const { return curr->value; }
    pointer operator->() const { return &curr->value; }

    friend bool operator==(const node_iterator& a, const node_iterator& b) {
        return a.curr == b.curr;
    }

    friend bool operator!=(const node_iterator& a, const node_iterator& b) {
        return !(a == b);
    }

private:
    using node_type = Node;

    node_type* curr;
};

} // namespace _impl_unordered_map

template <class Key, class T, class Hash = hash<Key>,
          class Pred = equal_to<Key>,
          class Allocator = allocator<pair<const Key, T>>>
class unordered_map {
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = pair<const Key, T>;
    using hasher = Hash;
    using key_equal = Pred;
    using allocator_type = Allocator;
    using pointer = typename allocator_traits<Allocator>::pointer;
    using const_pointer = typename allocator_traits<Allocator>::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using node_type = _impl_unordered_map::node<key_type, mapped_type>;

    using iterator = _impl_unordered_map::node_iterator<node_type>;
    using const_iterator = _impl_unordered_map::node_iterator<const node_type>;
    using local_iterator = _impl_unordered_map::node_iterator<node_type>;
    using const_local_iterator =
        _impl_unordered_map::node_iterator<const node_type>;

    // iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    // capacity
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept;

    // map operations
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    template <class K> iterator find(const K& k);
    template <class K> const_iterator find(const K& k) const;
    size_type count(const key_type& k) const;
    template <class K> size_type count(const K& k) const;
    bool contains(const key_type& k) const;
    template <class K> bool contains(const K& k) const;
    pair<iterator, iterator> equal_range(const key_type& k);
    pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
    template <class K> pair<iterator, iterator> equal_range(const K& k);
    template <class K>
    pair<const_iterator, const_iterator> equal_range(const K& k) const;

    // element access
    mapped_type& operator[](const key_type& k);
    mapped_type& operator[](key_type&& k);
    template <class K> mapped_type& operator[](K&& k);
    mapped_type& at(const key_type& k);
    const mapped_type& at(const key_type& k) const;
    template <class K> mapped_type& at(const K& k);
    template <class K> const mapped_type& at(const K& k) const;

private:
    allocator_type allocator_;

    size_type capacity_;
    size_type size_;
    node_type* elements_;
    node_type* free_list_;

    size_t num_buckets_;
    node_type* buckets_;
};

} // namespace std