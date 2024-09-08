cmake_minimum_required(VERSION 3.22)

project(user)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

# List all user apps in 'apps'' folder
list_dir(${CMAKE_CURRENT_SOURCE_DIR}/apps user_app_names)

add_custom_target(user_apps)

# Create target for all user apps in 'apps'' folder
foreach(app_name ${user_app_names})
	if (NOT TARGET ${app_name})
		set(${app_name}_base_dir ${CMAKE_CURRENT_SOURCE_DIR}/apps/${app_name})
		file(GLOB_RECURSE ${app_name}_src 
				${${app_name}_base_dir}/*.cpp 
				${${app_name}_base_dir}/*.c)
				
		# Create a target for the user app build with custom standard lib implementation
		add_executable(${app_name} EXCLUDE_FROM_ALL ${${app_name}_src})
		target_link_libraries(${app_name} c stdcpp unwind nostd_options)
		set_target_properties(${app_name} PROPERTIES FOLDER user)

		# Create a target for the user app build with gcc standard lib implementation
		if (VS_TARGET_FILTER STREQUAL "gnustd")
			add_executable(${app_name}_gnustd EXCLUDE_FROM_ALL ${${app_name}_src})
			set_target_properties(${app_name}_gnustd PROPERTIES FOLDER user_gnu)
		endif()

		# Add to global target
		add_dependencies(user_apps ${app_name})
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
