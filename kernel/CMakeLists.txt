cmake_minimum_required(VERSION 3.22)

project(kernel)
enable_language(C CXX ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE kernel_src
        ${CMAKE_CURRENT_SOURCE_DIR}/boot/limine/*.cpp)
set(lib_inc 
        ${CMAKE_CURRENT_SOURCE_DIR}/boot/limine)
set(linker_file
        ${CMAKE_CURRENT_SOURCE_DIR}/boot/limine/linker.ld)

add_executable(kernel EXCLUDE_FROM_ALL ${kernel_src})
target_include_directories(kernel PUBLIC ${lib_inc})
target_link_libraries(kernel c_kernel nostd_options kernel_options)
target_link_options(kernel PUBLIC "-T${linker_file}")
#target_link_libraries(kernel c_kernel stdcpp nostd_options kernel_options)
set_target_properties(kernel PROPERTIES FOLDER kernel)
