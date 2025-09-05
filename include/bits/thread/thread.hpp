#pragma once

#include <bits/arch/arch.hpp>
#include <bits/memory/unique_pointer.hpp>
#include <bits/type_traits/type_transformations.hpp>
#include <tuple>
#include <utility>

namespace std {

class thread {
public:
    // class thread::id
    class id;
    using native_handle_type = arch::current_arch::thread_handle_type;

    // construct/copy/destroy
    thread() noexcept;

    template <class F, class... Args> explicit thread(F&& f, Args&&... args) {
        using functor_type = decay_t<F>;
        using tuple_type = tuple<decay_t<Args>...>;

        struct thread_state {
            functor_type func;
            tuple_type args;
        };

        auto* state = new thread_state{
            std::forward<F>(f), forward_as_tuple(forward<Args>(args)...)};

        auto trampoline = [](void* p) -> void* {
            unique_ptr<thread_state> st(static_cast<thread_state*>(p));
            std::apply(st->func, st->args);
            return nullptr;
        };

        m_handle = arch::current_arch::thread_create(trampoline, state);
    }

    ~thread();
    thread(const thread&) = delete;
    thread(thread&&) noexcept;
    thread& operator=(const thread&) = delete;
    thread& operator=(thread&&) noexcept;

    // members
    void swap(thread&) noexcept;
    bool joinable() const noexcept;
    void join();
    void detach();
    id get_id() const noexcept;
    native_handle_type native_handle();

    // static members
    static unsigned int hardware_concurrency() noexcept;

private:
    native_handle_type m_handle{};
};

} // namespace std
