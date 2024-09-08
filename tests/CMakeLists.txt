cmake_minimum_required(VERSION 3.22)

project(tests)
enable_language(CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_config/compiler_flags.cmake)

# List all tests in current folder
file(GLOB_RECURSE unittestsuite_src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(unittestsuite SHARED EXCLUDE_FROM_ALL ${unittestsuite_src})
target_link_libraries(unittestsuite unittest_shared c_shared stdcpp_shared unwind_shared nostd_shared_options)
set_target_properties(unittestsuite PROPERTIES FOLDER test)

add_dependencies(unittestsuite unittest_runner)

enable_testing()

add_test(NAME unit_test_framework 
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../tools/UnitTestFramework/unittest_runner ${CMAKE_CURRENT_BINARY_DIR}/libunittestsuite.so)
	