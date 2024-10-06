if(NOT DEFINED COMPILER_FLAGS)

set(COMPILER_FLAGS True)

enable_testing()
enable_language( C CXX ASM )

# C generic compiler/linker flags
# set(C_STANDARD 11)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -fno-stack-protector -fno-builtin -mno-red-zone -std=c2x -U__linux__ -D__phoenix__ -g")
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")

# C++ generic compiler/linker flags
# set(CXX_STANDARD 17)
# -Wold-style-cast -Wconversion -Wsign-conversion -Wshadow
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -fno-stack-protector -fno-builtin -Wno-literal-suffix -mno-red-zone -Wno-packed-bitfield-compat -std=c++2b -U__linux__ -D__phoenix__ -g")
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")

# ASM generic compiler/linker flags
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} "-Wall -Wextra -fno-stack-protector -fno-builtin -x assembler-with-cpp -g")

# Linker flags
SET(CMAKE_EXE_LINKER_FLAGS "-g")

add_library(nostd_options INTERFACE)
target_link_options(nostd_options INTERFACE -nostdlib -no-pie)
target_compile_options(nostd_options INTERFACE 
		$<$<COMPILE_LANGUAGE:C>:-nostdinc -ffreestanding -DHAVE_MMAP=0>
		$<$<COMPILE_LANGUAGE:CXX>:-nostdinc -ffreestanding -nostdinc++>
)
		
add_library(nostd_shared_options INTERFACE)
target_link_options(nostd_shared_options INTERFACE -nostdlib -fPIC)
target_compile_options(nostd_shared_options INTERFACE 
		$<$<COMPILE_LANGUAGE:C>:-nostdinc -ffreestanding>
		$<$<COMPILE_LANGUAGE:CXX>:-nostdinc -nostdinc++ -ffreestanding>
)

add_library(kernel_options INTERFACE)
target_link_options(kernel_options INTERFACE -nostdlib -static -pie -Wl,--no-dynamic-linker -g)
#target_compile_options(kernel_options INTERFACE -fno-exceptions
#		$<$<COMPILE_LANGUAGE:CXX>: -fno-rtti>)

endif()

if (NOT DEFINED VS_TARGET_FILTER)
	set(VS_TARGET_FILTER "") 
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
