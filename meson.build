project('porticxx', 'cpp')

arch_name = meson.get_external_property('arch', 'unknown')
arch_sources = files('src/arch/default_arch.cpp')
arch_cpp_args = []

if arch_name == 'wasm32-stubs'
    arch_sources += files(
        'src/arch/wasm32_stubs/arch.cpp',
        'src/arch/wasm32_stubs/runtime.cpp',
        'src/arch/wasm32_stubs/syscall.cpp',
    )

elif arch_name == 'x86_64'
    arch_sources += files(
        'src/arch/x86_64/runtime.cpp',
        'src/arch/x86_64/runtime.s',
        'src/arch/x86_64/io.cpp',
        'src/arch/x86_64/fs.cpp',
        'src/arch/x86_64/thread.cpp',
        'src/arch/x86_64/sys_call.s',
    )
    arch_cpp_args = ['-DPCXX_ARCH_X86_64']
else
    error('Unknown architecture: ' + arch_name)
endif

sources = files(
    'src/exception/exception.cpp',
    'src/exception/exception_ptr.cpp',
    'src/iostream/cout.cpp',
    'src/iostream/cerr.cpp',
    'src/iostream/cin.cpp',
    #'src/allocator/bin_allocator.cpp',
    'src/system_error.cpp',
    'src/cxx_abi/dummy.cpp',
    'src/cxx_abi/new.cpp',
    'src/cxx_abi/rtti_dummy.cpp',
    'src/ios/basic_ios.cpp',
    'src/ios/ios_base.cpp',
    'src/ostream/basic_ostream.cpp',
    'src/string/char_traits.cpp',
    'src/porticxx/runtime.cpp',
    'src/cstring.cpp',
    'src/cstdlib.cpp',
    'src/thread/thread.cpp',
    'src/mutex/mutex.cpp',
)

porticxx_lib = static_library(
    'porticxx',
    sources: [arch_sources, sources],
    include_directories: ['include', 'src/include'],
    cpp_args: arch_cpp_args)

porticxx_dep = declare_dependency(
    include_directories: 'include', 
    link_with: porticxx_lib
)

subdir('examples')
