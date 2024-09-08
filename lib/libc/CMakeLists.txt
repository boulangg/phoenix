cmake_minimum_required(VERSION 3.22)

project(libc)
enable_language(C ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE libc_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
set(libc_inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE libc_glue_src ${CMAKE_CURRENT_SOURCE_DIR}/glue/*.S)
set(libc_glue_inc ${CMAKE_CURRENT_SOURCE_DIR}/glue)

file(GLOB_RECURSE libc_glue_kernel_src ${CMAKE_CURRENT_SOURCE_DIR}/glue_kernel/*.c)
set(libc_glue_kernel_inc ${CMAKE_CURRENT_SOURCE_DIR}/glue_kernel)

add_library(c STATIC EXCLUDE_FROM_ALL ${libc_src} ${libc_glue_src})
target_include_directories(c PUBLIC ${libc_inc} ${libc_glue_inc})
target_link_libraries(c nostd_options)
set_target_properties(c PROPERTIES OUTPUT_NAME c)
set_target_properties(c PROPERTIES FOLDER lib)

add_library(c_shared SHARED EXCLUDE_FROM_ALL ${libc_src} ${CMAKE_CURRENT_SOURCE_DIR}/glue/syscall.S)
target_include_directories(c_shared PUBLIC ${libc_inc} ${libc_glue_inc})
target_link_libraries(c_shared nostd_shared_options)
set_target_properties(c_shared PROPERTIES OUTPUT_NAME c)
set_target_properties(c_shared PROPERTIES FOLDER lib)

add_library(c_kernel STATIC EXCLUDE_FROM_ALL ${libc_src} ${libc_glue_kernel_src})
target_include_directories(c_kernel PUBLIC ${libc_inc} ${libc_glue_kernel_inc})
target_link_libraries(c_kernel kernel_options)
set_target_properties(c_kernel PROPERTIES FOLDER lib_kernel)
