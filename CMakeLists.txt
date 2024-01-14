cmake_minimum_required(VERSION 3.22)

project(phoenix)
enable_language(C CXX ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/compiler_flags.cmake)

add_subdirectory(kernel)
add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools/UnitTestFramework)
add_subdirectory(user)

# Download limine-bootloader
add_custom_command(
    OUTPUT limine/limine
    COMMAND rm -rf limine
    COMMAND git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1
    COMMAND make -C limine
)

# Create iso file
set(IMAGE_NAME disk.iso)

add_custom_target(
    ${IMAGE_NAME}
    COMMAND rm -rf iso_root
    COMMAND mkdir -p iso_root
    COMMAND cp -v kernel/kernel ${CMAKE_SOURCE_DIR}/kernel/boot/limine/limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/
    COMMAND mkdir -p mkdir -p iso_root/EFI/BOOT
    COMMAND cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
    COMMAND cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/
    COMMAND xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o ${IMAGE_NAME}
    COMMAND ./limine/limine bios-install ${IMAGE_NAME}
    COMMAND rm -rf iso_root
    DEPENDS kernel/kernel limine/limine ${CMAKE_SOURCE_DIR}/kernel/boot/limine/limine.cfg
)
add_dependencies(${IMAGE_NAME} kernel)

# Create disk
set(DISK_NAME drive.img)
set(BLOCK_SIZE 512)
math(EXPR MEGS "1 << 20")

## Partition table
math(EXPR PARTTABLE_SIZE "1 << 20")

## Partition 1
set(PART1_DIR drive_part1)
set(PART1_SIZE_MEGS 32)

math(EXPR PART1_SIZE "${PART1_SIZE_MEGS} * ${MEGS}")
math(EXPR PART1_BLOCK_COUNT "${PART1_SIZE} / ${BLOCK_SIZE}")
math(EXPR PART1_OFFSET "${PARTTABLE_SIZE}")

add_custom_target(
    ${PART1_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PART1_DIR}
)

add_custom_target(
    ${PART1_DIR}_bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PART1_DIR}/bin
    DEPENDS ${PART1_DIR}
)

add_custom_target(
    partition_1
    DEPENDS ${PART1_DIR}
)

list_dir(${CMAKE_CURRENT_SOURCE_DIR}/user/apps user_app_names)

foreach(app_name ${user_app_names})
    add_custom_target(
        ${PART1_DIR}_bin_${app_name}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/user/${app_name}
                ${PART1_DIR}/bin
        DEPENDS ${PART1_DIR}_bin ${CMAKE_CURRENT_BINARY_DIR}/user/${app_name}
    )
    add_dependencies(partition_1 ${PART1_DIR}_bin_${app_name})
endforeach()

## Create Disk
math(EXPR DISK_SIZE "${PARTTABLE_SIZE} + ${PART1_SIZE}")
math(EXPR DISK_BLOCK_COUNT "${DISK_SIZE} / ${BLOCK_SIZE}")

set(PART_CONFIG "")
string(APPEND PART_CONFIG "type=83, size=${PART1_BLOCK_COUNT}\\n")
#message(${PART_CONFIG})

add_custom_target(
    ${DISK_NAME}
    COMMAND rm -rf ${DISK_NAME}
    COMMAND dd if=/dev/zero of="${DISK_NAME}" bs=${BLOCK_SIZE} count=${DISK_BLOCK_COUNT}
    DEPENDS partition_1
)

add_custom_target(
    partition_drive
    COMMAND bash -c "printf ${PART_CONFIG} | sfdisk ${DISK_NAME}"
    VERBATIM
    DEPENDS ${DISK_NAME}
)

add_custom_target(
    partition_copy
    COMMAND mke2fs -L '' -N 0 -O ^64bit -d ${PART1_DIR} -m 5 -r 1 -t ext2 -E offset=${PART1_OFFSET} "${DISK_NAME}" "${PART1_SIZE_MEGS}M"
    DEPENDS partition_drive
)

# Other drive
set(drive2_name ${CMAKE_CURRENT_BINARY_DIR}/drive2.img)
set(drive2_size 16M)
set(drive2_create_script ${CMAKE_CURRENT_SOURCE_DIR}/scripts/disk.sh)
add_custom_command(	OUTPUT ${drive2_name}
					COMMAND ${drive2_create_script} CREATE ${drive2_name} ${drive2_size})

# QEMU setup
set(qemu qemu-system-x86_64)
set(qemu_opts "-M q35 -m 2G -cdrom ${IMAGE_NAME} -device piix3-ide,id=ide -drive id=disk,file=${DISK_NAME},format=raw,if=none -device ide-hd,drive=disk,bus=ide.0,unit=0 -boot d -display gtk,zoom-to-fit=off")
set(qemu_opts_debug "${qemu_opts} -s -S -d int -no-shutdown -no-reboot")

set(terminal ${CMAKE_CURRENT_SOURCE_DIR}/scripts/terminal.sh)

# QEMU run
## Start QEMU
add_custom_target(
    run
    COMMAND ${terminal} -e "${qemu} ${qemu_opts} &"
    DEPENDS ${IMAGE_NAME} partition_copy ${drive2_name}
)

# QEMU debug
## Copy binary into CMAKE_CURRENT_BINARY_DIR
set(kernel_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${kernel_debug_dir})
set(kernel_app_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/kernel)
file(MAKE_DIRECTORY ${kernel_app_debug_dir})
set(user_app_debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/user)
file(MAKE_DIRECTORY ${user_app_debug_dir})

add_custom_target(
    debug_bin
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
add_custom_target(
    debug
	COMMAND ${terminal} -e "${qemu} ${qemu_opts_debug} &"
	COMMAND sleep 1
	COMMAND ${terminal} -e "gdb kernel/kernel &"
	DEPENDS ${IMAGE_NAME} partition_copy .gdbinit
)

add_custom_command(
    OUTPUT .gdbinit
    COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/.gdbinit 
        .gdbinit
)
