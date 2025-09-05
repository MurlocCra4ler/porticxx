#pragma once

namespace std {

class mutex {
public:
    constexpr mutex() noexcept : next_(0), serving_(0) {}
    ~mutex() = default;

    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    void lock();
    bool try_lock();
    void unlock();

    using native_handle_type = void;
    native_handle_type native_handle();

private:
    int next_;
    int serving_;
};

} // namespace std
