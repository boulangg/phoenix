cmake_minimum_required(VERSION 3.22)

project(tests)
enable_language(CXX)

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

# Include unit test framework lib
if (NOT TARGET unittest)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../tools/UnitTestFramework dependency/UnitTestFramework)
endif()

# List all tests in current folder
file(GLOB_RECURSE unittestsuite_src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(unittestsuite SHARED ${unittestsuite_src})
#target_link_libraries(unittestsuite unittest)
target_link_libraries(unittestsuite unittest c_shared stdcpp_shared unwind_shared nostd_shared_options)
#target_link_libraries(unittestsuite unittest c_shared stdcpp_shared unwind_shared nostd_shared_options)
set_target_properties(unittestsuite PROPERTIES FOLDER test)

enable_testing()

add_test(NAME unit_test_framework 
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/dependency/UnitTestFramework/unittest_runner ${CMAKE_CURRENT_BINARY_DIR}/libunittestsuite.so)
	