cmake_minimum_required(VERSION 3.19)

project(libutils)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(lib_inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(utils STATIC EXCLUDE_FROM_ALL ${lib_src})
target_include_directories(utils PUBLIC ${lib_inc})
target_link_libraries(utils c stdcpp nostd_options)
set_target_properties(utils PROPERTIES OUTPUT_NAME utils)
set_target_properties(utils PROPERTIES FOLDER lib)

add_library(utils_gnu STATIC EXCLUDE_FROM_ALL ${lib_src})
target_include_directories(utils_gnu PUBLIC ${lib_inc})
target_link_libraries(utils_gnu)
set_target_properties(utils_gnu PROPERTIES OUTPUT_NAME utils)
set_target_properties(utils_gnu PROPERTIES FOLDER lib)

add_library(utils_shared SHARED EXCLUDE_FROM_ALL ${lib_src})
target_include_directories(utils_shared PUBLIC ${lib_inc})
target_link_libraries(utils_shared c_shared stdcpp_shared nostd_shared_options) 
set_target_properties(utils_shared PROPERTIES OUTPUT_NAME utils)
set_target_properties(utils_shared PROPERTIES FOLDER lib)
