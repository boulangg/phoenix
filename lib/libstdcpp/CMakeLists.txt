cmake_minimum_required(VERSION 3.22)

project(libstdcpp)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE libstdcpp_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(libstdcpp_inc ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../libc/include ${CMAKE_CURRENT_SOURCE_DIR}/debug)

# GDB pretty-printer
set(gdb_printer_src ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.S ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.py)
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.S APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.py)

add_library(stdcpp STATIC EXCLUDE_FROM_ALL ${libstdcpp_src})
target_include_directories(stdcpp PUBLIC ${libstdcpp_inc})
target_link_libraries(stdcpp unwind nostd_options)
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
