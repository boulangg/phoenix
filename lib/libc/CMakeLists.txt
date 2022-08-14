cmake_minimum_required(VERSION 3.22)

project(libc)
enable_language(C ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
set(lib_inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE glue_src ${CMAKE_CURRENT_SOURCE_DIR}/glue/*.S)
set(glue_inc ${CMAKE_CURRENT_SOURCE_DIR}/glue)

file(GLOB_RECURSE glue_kernel_src ${CMAKE_CURRENT_SOURCE_DIR}/glue_kernel/*.c)
set(glue_kernel_inc ${CMAKE_CURRENT_SOURCE_DIR}/glue_kernel)

add_library(c STATIC ${lib_src} ${glue_src})
target_include_directories(c PUBLIC ${lib_inc} ${glue_inc})
target_link_libraries(c kernel_options)

add_library(ckernel STATIC ${lib_src} ${glue_kernel_src})
target_include_directories(ckernel PUBLIC ${lib_inc} ${glue_kernel_inc})
set_target_properties(ckernel PROPERTIES EXCLUDE_FROM_ALL 1) 
target_link_libraries(ckernel kernel_options)