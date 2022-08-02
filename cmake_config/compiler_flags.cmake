if(NOT DEFINED COMPILER_FLAGS)

set(COMPILER_FLAGS True)

# C generic compiler/linker flags
set(C_STANDARD 11)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -nostdinc -g -fno-exceptions -mcmodel=kernel -Wno-packed-bitfield-compat -fno-stack-protector -fno-builtin-fprintf -fno-pie")
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")

# C++ generic compiler/linker flags
set(CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -nostdinc -nostdinc++ -g -fno-exceptions -fno-rtti -mcmodel=kernel -Wno-packed-bitfield-compat -fno-stack-protector -fno-builtin-fprintf -fno-use-cxa-atexit -fno-pie")
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")

# ASM generic compiler/linker flags
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} "-Wall -Wextra -nostdinc -nostdinc++ -g -fno-exceptions -fno-rtti -mcmodel=kernel -Wno-packed-bitfield-compat -fno-stack-protector -fno-builtin-fprintf -fno-use-cxa-atexit -fno-pie")

# Linker flags
SET(CMAKE_EXE_LINKER_FLAGS  "-nodefaultlibs -g -no-pie")

#-specs=${CMAKE_CURRENT_LIST_FILE}/specs

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
