cmake_minimum_required(VERSION 3.22)

project(unittestframework)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_config/compiler_flags.cmake)

# Target for test lib
file(GLOB_RECURSE libunittest_src ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/*.cpp)
set(libunittest_inc ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)

add_library(unittest_shared SHARED EXCLUDE_FROM_ALL ${libunittest_src})
target_include_directories(unittest_shared PUBLIC ${libunittest_inc})
target_link_libraries(unittest_shared c_shared stdcpp_shared unwind_shared nostd_options)
set_target_properties(unittest_shared PROPERTIES FOLDER test)

# Target for test runner
file(GLOB_RECURSE unittest_runner_src ${CMAKE_CURRENT_SOURCE_DIR}/runner/*.cpp)

add_executable(unittest_runner EXCLUDE_FROM_ALL ${unittest_runner_src} ${libunittest_src})
target_include_directories(unittest_runner PUBLIC ${libunittest_inc})
target_link_libraries(unittest_runner dl) # c stdcpp unwind nostd_options)
set_target_properties(unittest_runner PROPERTIES FOLDER test)
set_target_properties(unittest_runner PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	VS_DEBUGGER_COMMAND_ARGUMENTS "bar.txt")
