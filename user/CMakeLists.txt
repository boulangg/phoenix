cmake_minimum_required(VERSION 3.22)

project(user)
enable_language(CXX)

if (NOT DEFINED VS_TARGET_FILTER)
	set(VS_TARGET_FILTER "") 
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

# Include c lib
if (NOT TARGET c)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libc dependency/libc)
endif()

# Include unwind lib
if (NOT TARGET unwind)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libunwind dependency/libunwind)
endif()

# Include stdcpp lib
if (NOT TARGET stdcpp)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libstdcpp dependency/libstdcpp)
endif()

# List all user apps in 'apps'' folder
list_dir(${CMAKE_CURRENT_SOURCE_DIR}/apps app_names)

# Create target for all user apps in 'apps'' folder
foreach(app_name ${app_names})
	if (NOT TARGET ${app_name})
		set(${app_name}_base_dir ${CMAKE_CURRENT_SOURCE_DIR}/apps/${app_name})
		file(GLOB_RECURSE ${app_name}_src 
				${${app_name}_base_dir}/*.cpp 
				${${app_name}_base_dir}/*.c)
				
		# Create a target for the user app build with custom standard lib implementation
		add_executable(${app_name} ${${app_name}_src})
		target_link_libraries(${app_name} c stdcpp unwind nostd_options)
		set_target_properties(${app_name} PROPERTIES FOLDER user)

		# Create a target for the user app build with gcc standard lib implementation
		if (VS_TARGET_FILTER STREQUAL "gnustd")
			add_executable(${app_name}_gnustd ${${app_name}_src})
			set_target_properties(${app_name}_gnustd PROPERTIES FOLDER user_gnu)
			set_target_properties(${app_name}_gnustd PROPERTIES EXCLUDE_FROM_ALL 1)
		endif()
	endif()
endforeach()

# Create global target for all user apps build with gcc standard lib implementation
if(NOT TARGET gnustd AND VS_TARGET_FILTER STREQUAL "gnustd")
	add_custom_target(gnustd)
	set_target_properties(gnustd PROPERTIES FOLDER user_gnu)
  
	foreach(app_name ${app_names})
		add_dependencies(gnustd ${app_name}_gnustd)
	endforeach()
endif()

# Basic tests
set(CTEST_OUTPUT_ON_FAILURE 1)
add_test(test_ls ls)
