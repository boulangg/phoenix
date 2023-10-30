cmake_minimum_required(VERSION 3.22)

project(phoenix)
enable_language(C CXX ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/compiler_flags.cmake)

add_subdirectory(kernel)
add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools/UnitTestFramework)
add_subdirectory(user)

set(IMAGE_NAME disk)

# Download limine-bootloader
add_custom_command(
    OUTPUT limine/limine
    COMMAND git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1
    COMMAND make -C limine
)

# Create iso file
add_custom_target(${IMAGE_NAME}.iso
                    COMMAND rm -rf iso_root
                    COMMAND mkdir -p iso_root
                    COMMAND cp -v kernel/kernel ${CMAKE_SOURCE_DIR}/kernel/boot/limine/limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/
                    COMMAND mkdir -p mkdir -p iso_root/EFI/BOOT
                    COMMAND cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
                    COMMAND cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/
                    COMMAND xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o ${IMAGE_NAME}.iso
                    COMMAND ./limine/limine bios-install ${IMAGE_NAME}.iso
                    COMMAND rm -rf iso_root
                    DEPENDS limine/limine
)
add_dependencies(${IMAGE_NAME}.iso kernel)

# QEMU setup
set(qemu qemu-system-x86_64)
message(${qemu})
set(qemu_opts "-M q35 -m 2G -cdrom ${IMAGE_NAME}.iso -boot d")
set(qemu_opts_debug "${qemu_opts} -s -S")

set(terminal ${CMAKE_CURRENT_SOURCE_DIR}/scripts/terminal.sh)

# QEMU run
## Start QEMU
add_custom_target(run
                    COMMAND ${terminal} -e "${qemu} ${qemu_opts}" &
)
add_dependencies(run ${IMAGE_NAME}.iso)

# QEMU debug
## Copy binary into CMAKE_CURRENT_BINARY_DIR
set(kernel_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${kernel_debug_dir})
set(kernel_app_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/kernel)
file(MAKE_DIRECTORY ${kernel_app_debug_dir})
set(user_app_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/user)
file(MAKE_DIRECTORY ${user_app_debug_dir})

add_custom_target( 	debug_bin
					COMMAND ${CMAKE_COMMAND} -E copy
                			${CMAKE_CURRENT_BINARY_DIR}/kernel/kernel
                			${kernel_debug_dir}
					COMMAND ${CMAKE_COMMAND} -E copy
                			${CMAKE_CURRENT_BINARY_DIR}/kernel/kernel_apps/*
                			${kernel_app_debug_dir}
					COMMAND ${CMAKE_COMMAND} -E copy
                			${CMAKE_CURRENT_BINARY_DIR}/kernel/user_apps/*
                			${user_app_debug_dir}
                	DEPENDS kernel
)

## Start QEMU with GDB
add_custom_target(	debug
					COMMAND ${terminal} -e "${qemu} ${qemu_opts_debug}" &
					COMMAND sleep 1
					COMMAND ${terminal} -e "gdb kernel/kernel" &
					DEPENDS ${IMAGE_NAME}.iso .gdbinit
)

add_custom_command(
        OUTPUT .gdbinit
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/.gdbinit
                .gdbinit)
