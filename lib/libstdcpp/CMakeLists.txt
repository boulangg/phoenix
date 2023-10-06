cmake_minimum_required(VERSION 3.22)

project(libstdcpp)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

if (NOT TARGET unwind)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libunwind dependency/libunwind)
endif()

file(GLOB_RECURSE libstdcpp_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(libstdcpp_inc ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../libc/include)


# GDB pretty-printer
#file(GLOB gdb_printer_src ${CMAKE_CURRENT_SOURCE_DIR}/debug/*.cpp)
#message("gdb_printer_src: "${gdb_printer_src})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/debug/gdb-printers.py ${CMAKE_CURRENT_BINARY_DIR}/debug/ COPYONLY)

add_library(stdcpp STATIC ${libstdcpp_src})
target_include_directories(stdcpp PUBLIC ${libstdcpp_inc})
target_link_libraries(stdcpp unwind nostd_options)
set_target_properties(stdcpp PROPERTIES OUTPUT_NAME stdcpp)
set_target_properties(stdcpp PROPERTIES FOLDER lib)

add_library(stdcpp_shared SHARED ${libstdcpp_src} ${gdb_printer_src})
target_include_directories(stdcpp_shared PUBLIC ${libstdcpp_inc})
target_link_libraries(stdcpp_shared unwind_shared nostd_options)
set_target_properties(stdcpp_shared PROPERTIES OUTPUT_NAME stdcpp)
set_target_properties(stdcpp_shared PROPERTIES FOLDER lib)
