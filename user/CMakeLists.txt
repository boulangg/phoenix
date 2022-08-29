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
	target_link_libraries(${app_name} c stdcpp kernel_options nostd_options)
	
	target_link_libraries(${app_name})
	set_target_properties(${app_name} PROPERTIES FOLDER user)
	if (NOT DEFINED VS_TARGET_FILTER OR ${VS_TARGET_FILTER} STREQUAL "gnustd")
		add_executable(${app_name}_gnustd ${${app_name}_src})
		set_target_properties(${app_name}_gnustd PROPERTIES EXCLUDE_FROM_ALL 1)
		set_target_properties(${app_name}_gnustd PROPERTIES FOLDER user_gnu)
	endif()
endif()

if(NOT TARGET gnustd AND (NOT DEFINED VS_TARGET_FILTER OR ${VS_TARGET_FILTER} STREQUAL "gnustd"))
	add_custom_target(gnustd)
	set_target_properties(gnustd PROPERTIES FOLDER user_gnu)
  
	foreach(app_name ${app_names})
		add_dependencies(gnustd ${app_name}_gnustd)
	endforeach()
endif()

endforeach()
