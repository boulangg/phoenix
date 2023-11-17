cmake_minimum_required(VERSION 3.22)

project(acpica)
enable_language(C)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE acpica_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
set(acpica_inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE acpica_glue_src ${CMAKE_CURRENT_SOURCE_DIR}/glue/*.c)
set(acpica_glue_inc ${CMAKE_CURRENT_SOURCE_DIR}/glue)

add_library(acpica STATIC EXCLUDE_FROM_ALL ${acpica_src} ${acpica_glue_src})
target_include_directories(acpica PUBLIC ${acpica_inc} ${acpica_glue_inc})
target_compile_options(acpica PRIVATE "-Wno-unused-parameter" "-Wno-format-truncation")
target_link_libraries(acpica c_kernel kernel_options)
set_target_properties(acpica PROPERTIES OUTPUT_NAME acpica)
set_target_properties(acpica PROPERTIES FOLDER lib_kernel)
