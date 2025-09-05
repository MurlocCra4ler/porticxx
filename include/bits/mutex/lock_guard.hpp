#pragma once

namespace std {

template <class Mutex> class lock_guard {
public:
    using mutex_type = Mutex;

    explicit lock_guard(mutex_type& m) : mutex_(m) { mutex_.lock(); }
    // lock_guard(mutex_type& m, adopt_lock_t);
    ~lock_guard() { mutex_.unlock(); }

    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

private:
    mutex_type& mutex_;
};

} // namespace std