cmake_minimum_required(VERSION 3.22)

project(kernel)
enable_language(C CXX ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE kernel_src
        ${CMAKE_CURRENT_SOURCE_DIR}/boot/limine/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/boot/limine/*.S
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.S
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.S)
set(lib_inc 
        ${CMAKE_CURRENT_SOURCE_DIR}/boot/limine
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts)
set(linker_file
        ${CMAKE_CURRENT_SOURCE_DIR}/boot/limine/linker.ld)

set(font_file ${CMAKE_CURRENT_SOURCE_DIR}/fonts/font.psf)
set(font_out_file ${CMAKE_CURRENT_BINARY_DIR}/fonts/font.psf.o)

add_custom_command(
    OUTPUT ${font_out_file}
    COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/fonts
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/fonts
    COMMAND "${CMAKE_OBJCOPY}" -O elf64-x86-64 -B i386 -I binary ${font_file} ${font_out_file}
    DEPENDS ${font_file}
)

SET_SOURCE_FILES_PROPERTIES(
  ${font_out_file}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

add_executable(kernel EXCLUDE_FROM_ALL ${kernel_src} ${font_out_file})
target_include_directories(kernel PUBLIC ${lib_inc})
set_target_properties(kernel PROPERTIES LINK_DEPENDS ${linker_file})
target_link_libraries(kernel c_kernel stdcpp_kernel nostd_options kernel_options)
target_link_options(kernel PUBLIC -nostdlib -static -pie -Wl,--no-dynamic-linker -T${linker_file} -g)
set_target_properties(kernel PROPERTIES FOLDER kernel)
