cmake_minimum_required(VERSION 3.22)

project(unittestframework)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

# Include c lib
if (NOT TARGET c_shared)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libc dependency/libc)
endif()

# Include unwind lib
if (NOT TARGET unwind_shared)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libunwind dependency/libunwind)
endif()

# Include stdcpp lib
if (NOT TARGET stdcpp_shared)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/libstdcpp dependency/libstdcpp)
endif()

# Target for test lib
file(GLOB_RECURSE libunittest_src ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/*.cpp)
set(libunittest_inc ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)

add_library(unittest_shared SHARED ${libunittest_src})
target_include_directories(unittest_shared PUBLIC ${libunittest_inc})
target_link_libraries(unittest_shared c_shared stdcpp_shared unwind_shared nostd_options)
set_target_properties(unittest_shared PROPERTIES FOLDER test)

# Target for test runner
file(GLOB_RECURSE unittest_runner_src ${CMAKE_CURRENT_SOURCE_DIR}/runner/*.cpp)

add_executable(unittest_runner ${unittest_runner_src} ${libunittest_src})
target_include_directories(unittest_runner PUBLIC ${libunittest_inc})
target_link_libraries(unittest_runner dl) # c stdcpp unwind nostd_options)
set_target_properties(unittest_runner PROPERTIES FOLDER test)
set_target_properties(unittest_runner PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	VS_DEBUGGER_COMMAND_ARGUMENTS "bar.txt")
