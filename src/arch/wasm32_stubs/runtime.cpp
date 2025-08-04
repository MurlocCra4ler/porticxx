#include <arch.hpp>

__attribute__((import_module("asyncify"), import_name("start_unwind"))) void _asyncify_start_unwind(void*);
__attribute__((import_module("asyncify"), import_name("stop_unwind"))) void _asyncify_stop_unwind();
__attribute__((import_module("asyncify"), import_name("start_rewind"))) void _start_rewind(void*);
__attribute__((import_module("asyncify"), import_name("stop_rewind"))) void _asyncify_stop_rewind();

extern "C" void __wasm_call_ctors();
//extern "C" void __wasm_call_dtors();

extern int main();

struct {
    void* stack_ptr;
    void* stack_limit;
} static asyncify_data;
static bool is_runtime_suspended;

__attribute__((noinline))
extern "C" void runtime_resume() {
    if (is_runtime_suspended) {
        _start_rewind(&asyncify_data);
    }

    int ret = main();
    arch::exit(ret);

    _asyncify_stop_unwind();
}

__attribute__((noinline))
extern "C" void runtime_yield() {
    if (!is_runtime_suspended) {  
        is_runtime_suspended = true;
        _asyncify_start_unwind(&asyncify_data.stack_ptr);
    } else { 
        _asyncify_stop_rewind();
        is_runtime_suspended = false;
    }
}

__attribute__((export_name("_init")))
extern "C" void _init() {
    
}

__attribute__((export_name("_start")))
extern "C" void _start() {
    // call constructors
    __wasm_call_ctors();

    // run main
    runtime_resume();

    __builtin_unreachable();
}
