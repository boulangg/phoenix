cmake_minimum_required(VERSION 3.19)

project(libstdcpp)
enable_language(ASM CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE libstdcpp_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(libstdcpp_inc ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../libc/include ${CMAKE_CURRENT_SOURCE_DIR}/debug)

# GDB pretty-printer
set(gdb_printer_src ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.S ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.py)
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.S APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.py)

add_library(stdcpp_debug STATIC EXCLUDE_FROM_ALL ${gdb_printer_src})
target_include_directories(stdcpp_debug PUBLIC ${libstdcpp_inc})
set_target_properties(stdcpp_debug PROPERTIES OUTPUT_NAME stdcpp_debug)
set_target_properties(stdcpp_debug PROPERTIES FOLDER lib)

add_library(stdcpp_debug_wrapper INTERFACE)
target_link_libraries(stdcpp_debug_wrapper INTERFACE stdcpp_debug)
target_link_options(stdcpp_debug_wrapper INTERFACE "SHELL:-Wl,--whole-archive $<TARGET_FILE:stdcpp_debug> -Wl,--no-whole-archive")


add_library(stdcpp STATIC EXCLUDE_FROM_ALL ${libstdcpp_src})
target_include_directories(stdcpp PUBLIC ${libstdcpp_inc})
target_link_libraries(stdcpp PUBLIC unwind nostd_options)
set_target_properties(stdcpp PROPERTIES OUTPUT_NAME stdcpp)
set_target_properties(stdcpp PROPERTIES FOLDER lib)

add_library(stdcpp_shared SHARED EXCLUDE_FROM_ALL ${libstdcpp_src} ${gdb_printer_src})
target_include_directories(stdcpp_shared PUBLIC ${libstdcpp_inc})
target_link_libraries(stdcpp_shared unwind_shared nostd_options)
set_target_properties(stdcpp_shared PROPERTIES OUTPUT_NAME stdcpp)
set_target_properties(stdcpp_shared PROPERTIES FOLDER lib)

add_library(stdcpp_kernel STATIC EXCLUDE_FROM_ALL ${libstdcpp_src} ${gdb_printer_src})
target_include_directories(stdcpp_kernel PUBLIC ${libstdcpp_inc})
target_link_libraries(stdcpp_kernel unwind_kernel kernel_options)
set_target_properties(stdcpp_kernel PROPERTIES FOLDER lib_kernel)
