#pragma once

#include <cstddef>

#include <bits/type_traits/type_transformations.hpp>
#include <bits/memory/unique_pointer.hpp>

namespace std {

/******************/
/* Smart pointers */
/******************/

template<typename T> class shared_ptr;
template<typename T> class weak_ptr;

namespace impl_shared_ptr {

struct ctrl_block_base {
    size_t strong_count{1}; // TODO: use atomics
    size_t weak_count{1}; // weak_count >= strong_count

    virtual void destroy_object() noexcept = 0;
    virtual void delete_this() noexcept = 0;
    virtual ~ctrl_block_base() = default;
};

template<typename T, typename Deleter>
struct ctrl_block_ptr : ctrl_block_base {
    T* ptr;
    Deleter deleter;

    ctrl_block_ptr(T* p, Deleter d)
        : ptr(p), deleter(move(d)) {}

    void destroy_object() noexcept override {
        deleter(ptr);
    }
    void delete_this() noexcept override {
        delete this;
    }
};

template<typename T>
struct ctrl_block_inplace : ctrl_block_base {
    alignas(T) unsigned char storage[sizeof(T)];

    template<typename... Args>
    ctrl_block_inplace(Args&&... args) {
        new (storage) T(forward<Args>(args)...);
    }

    T* get_ptr() noexcept {
        return reinterpret_cast<T*>(storage);
    }

    void destroy_object() noexcept override {
        get_ptr()->~T();
    }
    void delete_this() noexcept override {
        delete this;
    }
};

}

template<class T>
class shared_ptr {
public:
    using element_type = remove_extent_t<T>;
    using weak_type = weak_ptr<T>;
 
    // constructors
    constexpr shared_ptr() noexcept = default;
    constexpr shared_ptr(nullptr_t) noexcept : shared_ptr() {}

    template<class Y>
    explicit shared_ptr(Y* p);
    template<class Y, class D>
    shared_ptr(Y* p, D d);
    template<class Y, class D, class A>
    shared_ptr(Y* p, D d, A a);
    template<class D>
    shared_ptr(nullptr_t p, D d);
    template<class D, class A>
    shared_ptr(nullptr_t p, D d, A a);
    template<class Y>
    shared_ptr(const shared_ptr<Y>& r, element_type* p) noexcept;
    template<class Y>
    shared_ptr(shared_ptr<Y>&& r, element_type* p) noexcept;
    
    shared_ptr(const shared_ptr& other) noexcept : ptr(other.ptr), ctrl(other.ctrl) {
        if (ctrl) ctrl->strong_count += 1; 
    }
    
    template<class Y>
    shared_ptr(const shared_ptr<Y>& r) noexcept;
    shared_ptr(shared_ptr&& r) noexcept;
    template<class Y>
    shared_ptr(shared_ptr<Y>&& r) noexcept;
    template<class Y>
    explicit shared_ptr(const weak_ptr<Y>& r);
    template<class Y, class D>
    shared_ptr(unique_ptr<Y, D>&& r);
 
    // destructor
    ~shared_ptr() {
        release();
    }
 
    // assignment
    shared_ptr& operator=(const shared_ptr& r) noexcept;
    template<class Y>
    shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
    shared_ptr& operator=(shared_ptr&& r) noexcept;
    template<class Y>
    shared_ptr& operator=(shared_ptr<Y>&& r) noexcept;
    template<class Y, class D>
    shared_ptr& operator=(unique_ptr<Y, D>&& r);

    // observers
    element_type* get() const noexcept {
        return ptr;
    }

    T& operator*() const noexcept {
        return *ptr;
    }
    
    T* operator->() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

private:
    T* ptr{nullptr};
    impl_shared_ptr::ctrl_block_base* ctrl{nullptr};

    void release() {
        if (!ctrl) return;
        if (--ctrl->strong_count == 1) {
            ctrl->destroy_object();
            if (--ctrl->weak_count == 1) {
                ctrl->delete_this();
            }
        }
        ctrl = nullptr;
        ptr = nullptr;
    }

    template<class U, class... Args>
    friend shared_ptr<U> make_shared(Args&&... args);
};

}
