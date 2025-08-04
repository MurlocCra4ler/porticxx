sources = files(
    'src/exception.cpp',
    'src/iostream.cpp',
    'src/bin_allocator.cpp',
    'src/system_error.cpp',
    'src/arch/wasm32_stubs/arch.cpp',
    'src/arch/wasm32_stubs/runtime.cpp',
    'src/arch/wasm32_stubs/syscall.cpp',
    'src/cxx_abi/dummy.cpp',
    'src/cxx_abi/new.cpp',
    'src/cxx_abi/rtti_dummy.cpp',
    'src/ios/basic_ios.cpp',
    'src/ios/ios_base.cpp',
    'src/ostream/basic_ostream.cpp',
    'src/string/char_traits.cpp',
)

cxx_lib = static_library(
    'cxx',
    sources: sources,
    include_directories: ['include', 'src/include'])

cxx_dep = declare_dependency(include_directories: 'include', link_with: cxx_lib)
