cmake_minimum_required(VERSION 3.19)

project(phoenix)
enable_language(C CXX ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/compiler_flags.cmake)

add_subdirectory(kernel)
add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools/UnitTestFramework)
add_subdirectory(user)

set(terminal ${CMAKE_CURRENT_SOURCE_DIR}/scripts/terminal.sh)

set(kernel_bin phoenix.bin)
set(kernel_file ${CMAKE_CURRENT_BINARY_DIR}/kernel/${kernel_bin})

set(disk_name ${CMAKE_CURRENT_BINARY_DIR}/disk.iso)
set(disk_dir ${CMAKE_CURRENT_SOURCE_DIR}/iso)
file(MAKE_DIRECTORY ${disk_dir})

set(drive_name ${CMAKE_CURRENT_BINARY_DIR}/drive.img)
set(drive_size 16M)
set(drive_create_script ${CMAKE_CURRENT_SOURCE_DIR}/scripts/disk.sh)

set(qemu qemu-system-x86_64)
set(qemu_opts "-boot d -m 256 -cdrom ${disk_name} -hda ${drive_name} -d int,cpu_reset")
set(qemu_opts_debug "${qemu_opts} -s -S")

add_custom_command(	OUTPUT ${drive_name}
					COMMAND ${drive_create_script} CREATE ${drive_name} ${drive_size})
					
add_custom_command(	OUTPUT ${disk_name}
					COMMAND cp ${kernel_file} ${disk_dir}/boot
					COMMAND grub-mkrescue -o ${disk_name} ${disk_dir}
					DEPENDS kernel)

add_custom_target(	launch
					COMMAND ${terminal} -e "${qemu} ${qemu_opts}" &
					DEPENDS ${drive_name} ${disk_name})
set_target_properties(launch PROPERTIES FOLDER kernel ${disk_name} ${drive_name})


set(kernel_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${kernel_debug_dir})

set(kernel_app_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/kernel)
file(MAKE_DIRECTORY ${kernel_app_debug_dir})

set(user_app_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/user)
file(MAKE_DIRECTORY ${user_app_debug_dir})

add_custom_target( 	debug_bin
					COMMAND ${CMAKE_COMMAND} -E copy
                			${kernel_file}
                			${kernel_debug_dir}
					COMMAND ${CMAKE_COMMAND} -E copy
                			${CMAKE_CURRENT_BINARY_DIR}/kernel/kernel_apps/*
                			${kernel_app_debug_dir}
					COMMAND ${CMAKE_COMMAND} -E copy
                			${CMAKE_CURRENT_BINARY_DIR}/kernel/user_apps/*
                			${user_app_debug_dir}
                	DEPENDS kernel)
set_target_properties(debug_bin PROPERTIES FOLDER internal)

add_custom_target(	debug
					COMMAND ${terminal} -e "${qemu} ${qemu_opts_debug}" &
					COMMAND sleep 1
					COMMAND ${terminal} -e "gdb ${kernel_file}" &
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
					DEPENDS ${drive_name} ${disk_name} debug_bin)
set_target_properties(debug PROPERTIES FOLDER kernel)
