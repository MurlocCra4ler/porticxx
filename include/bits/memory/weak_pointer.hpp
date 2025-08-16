#pragma once

namespace std {

template<class T>
class weak_ptr {
public:
    using element_type = remove_extent_t<T>;
 
    // constructors
    constexpr weak_ptr() noexcept;
    template<class Y>
    weak_ptr(const shared_ptr<Y>& r) noexcept;
    weak_ptr(const weak_ptr& r) noexcept;
    template<class Y>
    weak_ptr(const weak_ptr<Y>& r) noexcept;
    weak_ptr(weak_ptr&& r) noexcept;
    template<class Y>
    weak_ptr(weak_ptr<Y>&& r) noexcept;
 
    // destructor
    ~weak_ptr();
 
    // assignment
    weak_ptr& operator=(const weak_ptr& r) noexcept;
    template<class Y>
    weak_ptr& operator=(const weak_ptr<Y>& r) noexcept;
    template<class Y>
    weak_ptr& operator=(const shared_ptr<Y>& r) noexcept;
    weak_ptr& operator=(weak_ptr&& r) noexcept;
    template<class Y>
    weak_ptr& operator=(weak_ptr<Y>&& r) noexcept;
 
    // modifiers
    void swap(weak_ptr& r) noexcept;
    void reset() noexcept;
 
    // observers
    long use_count() const noexcept;
    bool expired() const noexcept;
    shared_ptr<T> lock() const noexcept;
    template<class U>
    bool owner_before(const shared_ptr<U>& b) const noexcept;
    template<class U>
    bool owner_before(const weak_ptr<U>& b) const noexcept;
    size_t owner_hash() const noexcept;
    template<class U>
    bool owner_equal(const shared_ptr<U>& b) const noexcept;
    template<class U>
    bool owner_equal(const weak_ptr<U>& b) const noexcept;
};

template<class T>
weak_ptr(shared_ptr<T>) -> weak_ptr<T>;

}
