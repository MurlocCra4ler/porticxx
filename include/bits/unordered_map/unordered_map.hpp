#pragma once

#include <algorithm>
#include <bits/functional/equal_to.hpp>
#include <bits/memory/allocators.hpp>
#include <bits/utility/tags.hpp>
#include <initializer_list>
#include <iterator>

namespace std {

template <class Key> struct hash;

namespace _impl_unordered_map {

template <typename Key, typename T> struct node {
    using value_type = pair<const Key, T>;

    value_type* value;
    node* next = nullptr;
    node* prev = nullptr;
};

template <typename Node> class node_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename Node::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    node_iterator(Node* n) : cur_(n) { advance_to_valid(); }

    reference operator*() const { return *cur_->value; }
    pointer operator->() const { return cur_->value; }

    friend bool operator==(const node_iterator& a, const node_iterator& b) {
        return a.cur_ == b.cur_;
    }

    friend bool operator!=(const node_iterator& a, const node_iterator& b) {
        return !(a == b);
    }

    node_iterator& operator++() {
        if (cur_) {
            cur_ = cur_->next;
            advance_to_valid();
        }
        return *this;
    }

    node_iterator operator++(int) {
        node_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

private:
    Node* cur_ = nullptr;

    void advance_to_valid() {
        while (cur_ && cur_->value == nullptr)
            cur_ = cur_->next;
    }
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

    unordered_map() = default;

    unordered_map(std::initializer_list<value_type> init, size_type = 0,
                  const Hash& hash = Hash(), const key_equal& = key_equal(),
                  const Allocator& alloc = Allocator())
        : hash_(hasher()), equal_(key_equal()), pair_allocator_(alloc) {
        reserve(init.size());
        for (const auto& el : init) {
            insert(el);
        }
    }

    // iterators
    iterator begin() noexcept {
        if (empty())
            return end();
        return iterator(buckets_[0].next);
    }
    const_iterator begin() const noexcept {
        if (empty())
            return end();
        return const_iterator(buckets_[0].next);
    }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    // capacity
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept;

    template <class... Args> pair<iterator, bool> emplace(Args&&... args);
    template <class... Args>
    iterator emplace_hint(const_iterator position, Args&&... args);
    pair<iterator, bool> insert(const value_type& obj) {
        iterator it = find(obj.first);
        if (it != end()) {
            return {it, false};
        }

        node_type* node = allocate_element(obj.first);
        ::new (static_cast<void*>(node->value)) value_type(obj);
        return {iterator(node), true};
    }
    pair<iterator, bool> insert(value_type&& obj) {
        iterator it = find(obj.first);
        if (it != end()) {
            return {it, false};
        }

        node_type* node = allocate_element(obj.first);
        ::new (static_cast<void*>(node->value)) value_type(std::move(obj));
        return {iterator(node), true};
    }
    template <class P> pair<iterator, bool> insert(P&& obj);
    iterator insert(const_iterator hint, const value_type& obj);
    iterator insert(const_iterator hint, value_type&& obj);

    iterator erase(iterator pos) {
        if (pos == end())
            return pos;

        node_type* el = pos.cur_;
        free_element(el);
    }
    iterator erase(const_iterator pos);
    size_type erase(const key_type& key);
    template <class K> size_type erase(K&& x) {
        size_type count = 0;
        size_type bucket_idx = hash_(x) % num_buckets_;

        node_type* it = buckets_[bucket_idx].next;
        while (it && it->value) {
            node_type* next = it->next;
            if (equal_(it->value->first, x)) {
                free_element(it);
                count += 1;
            }

            it = next;
        }

        return count;
    }
    iterator erase(const_iterator first, const_iterator last);
    /*void swap(unordered_map&) noexcept(
      allocator_traits<Allocator>::is_always_equal::value&&
      is_nothrow_swappable_v<Hash>&& is_nothrow_swappable_v<Pred>); */
    void clear() noexcept;

    // map operations
    iterator find(const key_type& k) { return iterator(find_node(k)); }
    const_iterator find(const key_type& k) const {
        return const_iterator(find_node(k));
    }
    template <class K> iterator find(const K& k) {
        return iterator(find_node(k));
    }
    template <class K> const_iterator find(const K& k) const {
        return const_iterator(find_node(k));
    }
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
    mapped_type& operator[](const key_type& k) {
        auto [it, inserted] = insert(value_type(k, mapped_type{}));
        return it->second;
    }
    mapped_type& operator[](key_type&& k) {
        auto [it, inserted] = insert(value_type(std::move(k), mapped_type{}));
        return it->second;
    }
    template <class K> mapped_type& operator[](K&& k) {
        auto [it, inserted] =
            insert(value_type(std::forward<K>(k), mapped_type{}));
        return it->second;
    }
    mapped_type& at(const key_type& k);
    const mapped_type& at(const key_type& k) const;
    template <class K> mapped_type& at(const K& k);
    template <class K> const mapped_type& at(const K& k) const;

private:
    using node_allocator_type =
        typename allocator_traits<Allocator>::template rebind_alloc<node_type>;

    hasher hash_;
    key_equal equal_;

    allocator_type pair_allocator_;
    node_allocator_type node_allocator_;

    node_type* nodes_;

    node_type* buckets_;
    size_type num_buckets_;

    node_type* elements_;
    size_type capacity_ = 0;
    size_type size_ = 0;

    node_type* free_list_;

    float ml_ = 1.0;

    node_type* allocate_element(const key_type& k) {
        if (size_ >= capacity_) {
            reserve(std::max(size_ + 1, capacity_ << 1));
        }

        // find free element
        node_type* el;
        if (free_list_) {
            el = free_list_;
            free_list_ = free_list_->next;
        } else {
            el = &elements_[size_];
        }

        // find bucket
        size_type bucket_idx = hash_(k) % num_buckets_;
        node_type* bucket = &buckets_[bucket_idx];

        // insert new element at front
        el->prev = bucket;
        el->next = bucket->next;
        bucket->next = el;
        if (el->next)
            el->next->prev = el;

        // allocate value
        el->value = pair_allocator_.allocate(1);

        size_ += 1;
        return el;
    }

    void free_element(node_type* el) {
        // remove from bucket
        node_type* prev = el->prev;
        node_type* next = el->next;
        if (prev)
            prev->next = next;
        if (next)
            next->prev = prev;

        // add to free list
        el->next = free_list_;
        free_list_ = el;

        // deallocate value
        pair_allocator_.deallocate(el->value, 1);
        el->value = nullptr;
        size_ -= 1;
    }

    template <class K> node_type* find_node(const K& k) const {
        if (empty())
            return nullptr;

        size_type bucket_idx = hash_(k) % num_buckets_;
        for (node_type* it = buckets_[bucket_idx].next; it; it = it->next) {
            if (!it->value)
                // end of bucket
                return nullptr;
            if (equal_(it->value->first, k))
                return it;
        }
        return nullptr;
    }

    template <class Fn> void for_each_element(Fn&& fn) {
        if (empty())
            return;
        for (node_type* it = &buckets_[0]; it != nullptr; it = it->next) {
            if (it->value) {
                fn(*it);
            }
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity < 0)
            return;
        size_type required_buckets = static_cast<size_type>(new_capacity / ml_);
        if (required_buckets == 0)
            required_buckets = 1;

        node_type* new_nodes =
            node_allocator_.allocate(new_capacity + required_buckets);
        node_type* new_buckets = new_nodes;
        node_type* new_elements = new_nodes + required_buckets;

        // initialize buckets
        new_buckets[0].value = nullptr;
        new_buckets[0].prev = nullptr;
        new_buckets[required_buckets - 1].next = nullptr;

        for (size_type i = 1; i < required_buckets; ++i) {
            new_buckets[i].value = nullptr;
            new_buckets[i].prev = &new_buckets[i - 1];
            new_buckets[i - 1].next = &new_buckets[i];
        }

        // rehash elements
        size_type idx = 0;
        for_each_element([&](const node_type& old_el) {
            value_type* val = old_el.value;

            node_type* new_el = &new_elements[idx++];
            new_el->value = val;

            // find bucket
            size_type bucket_idx = hash_(val->first) % required_buckets;
            node_type* bucket = &new_buckets[bucket_idx];

            // insert new element at front
            new_el->prev = bucket;
            new_el->next = bucket->next;
            bucket->next = new_el;
            if (new_el->next)
                new_el->next->prev = new_el;
        });

        // free old memory
        if (capacity_ > 0) {
            node_allocator_.deallocate(nodes_, num_buckets_ + capacity_);
        }

        // update members
        nodes_ = new_nodes;
        buckets_ = new_buckets;
        elements_ = new_elements;
        num_buckets_ = required_buckets;
        capacity_ = new_capacity;
        free_list_ = nullptr;
    }
};

} // namespace std