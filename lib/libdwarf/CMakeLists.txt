cmake_minimum_required(VERSION 3.22)

project(libdwarf)
enable_language(C)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

if (NOT TARGET c)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libc dependency/libc)
endif()

file(GLOB_RECURSE lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
set(lib_inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(dwarf STATIC ${lib_src})
target_include_directories(dwarf PUBLIC ${lib_inc})
target_link_libraries(dwarf c kernel_options)
set_target_properties(dwarf PROPERTIES FOLDER lib)
