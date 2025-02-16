cmake_minimum_required(VERSION 3.19)

project(lib)
enable_language(C CXX ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

add_subdirectory(libc)
add_subdirectory(libstdcpp)
add_subdirectory(libunwind)
add_subdirectory(libutils)
