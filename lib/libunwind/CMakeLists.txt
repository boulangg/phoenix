cmake_minimum_required(VERSION 3.19)

project(libunwind)
enable_language(C ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/*.S)
set(lib_inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(unwind STATIC EXCLUDE_FROM_ALL ${lib_src})
target_include_directories(unwind PUBLIC ${lib_inc})
target_link_libraries(unwind c nostd_options)
set_target_properties(unwind PROPERTIES OUTPUT_NAME unwind)
set_target_properties(unwind PROPERTIES FOLDER lib)

add_library(unwind_shared SHARED EXCLUDE_FROM_ALL ${lib_src})
target_include_directories(unwind_shared PUBLIC ${lib_inc})
target_link_libraries(unwind_shared c_shared nostd_shared_options)
set_target_properties(unwind_shared PROPERTIES OUTPUT_NAME unwind)
set_target_properties(unwind_shared PROPERTIES FOLDER lib)

add_library(unwind_kernel STATIC EXCLUDE_FROM_ALL ${lib_src})
target_include_directories(unwind_kernel PUBLIC ${lib_inc})
target_link_libraries(unwind_kernel c_kernel kernel_options)
set_target_properties(unwind_kernel PROPERTIES FOLDER lib_kernel)
