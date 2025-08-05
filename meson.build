project('porticxx', 'cpp')

arch_name = meson.get_external_property('arch', 'unknown')
arch_sources = []

if arch_name == 'wasm32-stubs'
    arch_sources += files(
        'src/arch/wasm32_stubs/arch.cpp',
        'src/arch/wasm32_stubs/runtime.cpp',
        'src/arch/wasm32_stubs/syscall.cpp',
    )
elif arch_name == 'x86_64'
    arch_sources += files(
        'src/arch/x86_64/arch.cpp',
        'src/arch/x86_64/runtime.cpp',
    )
else
    error('Unknown architecture: ' + arch_name)
endif

sources = files(
    'src/exception.cpp',
    'src/iostream.cpp',
    'src/bin_allocator.cpp',
    'src/system_error.cpp',
    'src/cxx_abi/dummy.cpp',
    'src/cxx_abi/new.cpp',
    'src/cxx_abi/rtti_dummy.cpp',
    'src/ios/basic_ios.cpp',
    'src/ios/ios_base.cpp',
    'src/ostream/basic_ostream.cpp',
    'src/string/char_traits.cpp',
)

porticxx_lib = static_library(
    'porticxx',
    sources: [arch_sources, sources],
    include_directories: ['include', 'src/include'])

porticxx_dep = declare_dependency(include_directories: 'include', link_with: porticxx_lib)

subdir('examples')
