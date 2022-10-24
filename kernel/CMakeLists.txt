cmake_minimum_required(VERSION 3.22)

project(kernel)
enable_language(C CXX ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

file(GLOB_RECURSE kernel_src 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.S)
set(arch_dir ${CMAKE_CURRENT_SOURCE_DIR}/arch/x64)
set(lib_inc 
		${CMAKE_CURRENT_SOURCE_DIR}/src 
		${arch_dir})
set(scripts_dir ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
set(linker_gen_src ${arch_dir}/linker.gen)
set(kernel_name phoenix.bin)
set(kernel_app_names init;ls)

set(user_dir ${CMAKE_CURRENT_BINARY_DIR}/dependency/user)

if (NOT TARGET ckernel)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libc dependency/libc)
endif()

if (NOT TARGET stdcpp)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libstdcpp dependency/libstdcpp)
endif()

if (NOT TARGET user)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../user dependency/user)
endif()

set(gen_dir ${CMAKE_CURRENT_BINARY_DIR}/gen)
file(MAKE_DIRECTORY ${gen_dir})

# Generate kernel apps
set(kernel_apps_dir ${CMAKE_CURRENT_BINARY_DIR}/kernel_apps)
file(MAKE_DIRECTORY ${kernel_apps_dir})

add_custom_target(kernel_apps_dir_populate)
set_target_properties(kernel_apps_dir_populate PROPERTIES FOLDER kernel_internal)
foreach(app_name ${kernel_app_names})
if (NOT TARGET ${app_name}_cp_kernel)
	add_custom_command(	OUTPUT ${kernel_apps_dir}/${app_name}
						COMMAND ${CMAKE_COMMAND} -E copy
                				${user_dir}/${app_name}
                				${kernel_apps_dir}
						DEPENDS ${app_name})
	add_custom_target(${app_name}_cp_kernel DEPENDS ${kernel_apps_dir}/${app_name})
	add_dependencies(kernel_apps_dir_populate ${app_name}_cp_kernel)
	set_target_properties(${app_name}_cp_kernel PROPERTIES FOLDER kernel_internal)
endif()
endforeach()

set(user_apps_src ${gen_dir}/user_apps.S)
add_custom_command( OUTPUT ${user_apps_src}
					COMMAND ${scripts_dir}/user_apps.sh ${kernel_apps_dir} ${user_apps_src}
					DEPENDS kernel_apps_dir_populate)

# Generate initrd

list_dir(${CMAKE_CURRENT_SOURCE_DIR}/../user/apps user_app_names)
set(user_apps_dir ${CMAKE_CURRENT_BINARY_DIR}/user_apps)
file(MAKE_DIRECTORY ${user_apps_dir})

add_custom_target(user_apps_dir_populate)
set_target_properties(user_apps_dir_populate PROPERTIES FOLDER kernel_internal)
foreach(app_name ${user_app_names})
if (NOT TARGET ${app_name}_cp_user)
	add_custom_command(	OUTPUT ${user_apps_dir}/${app_name}
						COMMAND ${CMAKE_COMMAND} -E copy
                				${user_dir}/${app_name}
                				${user_apps_dir}
						DEPENDS ${app_name})
	add_custom_target(${app_name}_cp_user DEPENDS ${user_apps_dir}/${app_name})
	add_dependencies(user_apps_dir_populate ${app_name}_cp_user)
	set_target_properties(${app_name}_cp_user PROPERTIES FOLDER kernel_internal)
endif()
endforeach()

set(initrd_dir ${CMAKE_CURRENT_BINARY_DIR}/initrd)
file(MAKE_DIRECTORY ${initrd_dir})
set(initrd_disk ${initrd_dir}/initrd.iso)


set(terminal ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/terminal.sh)

add_custom_command(	OUTPUT ${initrd_disk}
					COMMAND	${terminal} -e "${scripts_dir}/initrd.sh ${initrd_disk} ${user_apps_dir}"
					DEPENDS ${scripts_dir}/initrd.sh ${user_app_names})
add_custom_target(initrd_disk DEPENDS ${initrd_disk} user_apps_dir_populate)
set_target_properties(initrd_disk PROPERTIES FOLDER kernel)
					
set(initrd_src ${initrd_dir}/initrd.S)
add_custom_command( OUTPUT ${initrd_src}
					COMMAND	${scripts_dir}/initrd.s.sh ${initrd_disk} ${initrd_src}
					DEPENDS ${scripts_dir}/initrd.sh ${initrd_disk})

# Generate linker
set(linker_src ${gen_dir}/linker.ld)

add_custom_command(	OUTPUT ${linker_src}
					COMMAND	${CMAKE_CXX_COMPILER} -P -E -I${arch_dir} -x c ${linker_gen_src} -o ${linker_src}
					DEPENDS ${linker_gen_src})
					
add_custom_target(linker_file DEPENDS ${linker_src})
set_target_properties(linker_file PROPERTIES FOLDER kernel_internal)

# Generate Kernel file
SET(CMAKE_EXE_LINKER_FLAGS  "-nodefaultlibs -g -specs=${CMAKE_CURRENT_SOURCE_DIR}/specs -no-pie -T${linker_src}")

add_executable(kernel ${kernel_src} ${initrd_src} ${user_apps_src})
target_include_directories(kernel PUBLIC ${lib_inc})
target_link_libraries(kernel c_kernel stdcpp nostd_options kernel_options)
add_dependencies(kernel initrd_disk linker_file)
set_target_properties(kernel 
			PROPERTIES 	PREFIX ""
						OUTPUT_NAME ${kernel_name}
						SUFFIX "")
set_target_properties(kernel PROPERTIES FOLDER kernel)
