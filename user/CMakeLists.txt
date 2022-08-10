cmake_minimum_required(VERSION 3.22)

project(user)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

if (NOT TARGET c)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libc dependency/libc)
endif()

if (NOT TARGET stdcpp)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libstdcpp dependency/libstdcpp)
endif()

list_dir(${CMAKE_CURRENT_SOURCE_DIR}/apps app_names)

foreach(app_name ${app_names})

if (NOT TARGET ${app_name})
	set(${app_name}_base_dir ${CMAKE_CURRENT_SOURCE_DIR}/apps/${app_name})
	file(GLOB_RECURSE ${app_name}_src ${${app_name}_base_dir}/*.cpp ${${app_name}_base_dir}/*.c ${${app_name}_base_dir}/*.S)
	add_executable(${app_name} ${${app_name}_src})
	target_link_libraries(${app_name} c stdcpp)
endif()

endforeach()
