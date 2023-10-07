if(NOT DEFINED COMPILER_FLAGS)

set(COMPILER_FLAGS True)

enable_testing()

# C generic compiler/linker flags
# set(C_STANDARD 11)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -g -Wno-packed-bitfield-compat -fno-stack-protector -fno-builtin-fprintf -std=c11")
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")

# C++ generic compiler/linker flags
# set(CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -g -Wno-packed-bitfield-compat -fno-stack-protector -fno-builtin-fprintf -fno-use-cxa-atexit -std=c++17 -Wno-literal-suffix")
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")

# ASM generic compiler/linker flags
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} "-Wall -Wextra -g -Wno-packed-bitfield-compat -fno-stack-protector -fno-builtin-fprintf -fno-use-cxa-atexit -x assembler-with-cpp")

# Linker flags
SET(CMAKE_EXE_LINKER_FLAGS  "-g")

add_library(nostd_options INTERFACE)
target_link_options(nostd_options INTERFACE -nodefaultlibs -nostdlib -no-pie)
target_compile_options(nostd_options INTERFACE 
		$<$<COMPILE_LANGUAGE:C>:-nostdinc>
		$<$<COMPILE_LANGUAGE:CXX>:-nostdinc -nostdinc++>
)
		
add_library(nostd_shared_options INTERFACE)
target_link_options(nostd_shared_options INTERFACE -nodefaultlibs -fPIC)
target_compile_options(nostd_shared_options INTERFACE 
		$<$<COMPILE_LANGUAGE:C>:-nostdinc -ffreestanding>
		$<$<COMPILE_LANGUAGE:CXX>:-nostdinc -nostdinc++ -ffreestanding>
		-DIS_SHARED_LIB
)

add_library(kernel_options INTERFACE)
target_compile_options(kernel_options INTERFACE -fno-exceptions
		$<$<COMPILE_LANGUAGE:CXX>: -fno-rtti>)

endif()

function(list_dir parent_dir dir_list)

file(GLOB app_dirs DIRECTORY ${parent_dir}/*)

set(local_dir_list "")
foreach(dir_path ${app_dirs})
	get_filename_component(dir_name "${dir_path}" NAME)
	list(APPEND local_dir_list ${dir_name})
endforeach()
set(${dir_list} ${local_dir_list} PARENT_SCOPE)

endfunction()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CTEST_ARGUMENTS "--verbose")
