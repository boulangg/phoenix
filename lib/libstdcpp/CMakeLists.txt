cmake_minimum_required(VERSION 3.22)

project(libstdcpp)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(lib_inc ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../libc/include)

add_library(stdcpp STATIC ${lib_src})
target_include_directories(stdcpp PUBLIC ${lib_inc})
target_link_libraries(stdcpp kernel_options)


