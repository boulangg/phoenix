/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <limine.h>

#include "stddef.h"
#include <cstdio>

#include "Kernel.h"
#include "MemoryInit.h"
#include "console/BasicConsole.h"

#include "utils/Elf64File.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Limine requests
static volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0, .response = nullptr};
static limine_memmap_request memmap_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_kernel_file_request kernel_file_request = {
    .id = LIMINE_KERNEL_FILE_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_smbios_request smbios_request = {
    .id = LIMINE_SMBIOS_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_rsdp_request rsbp_request = {.id = LIMINE_RSDP_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0, .response = nullptr};

// Halt and catch fire function.
static void hcf(void)
{
    asm("cli");
    for (;;) {
        asm("hlt");
    }
}

// Kernel entry point
void _start(void)
{
    kernel::console::BasicConsole console =
        kernel::console::BasicConsole(framebuffer_request.response->framebuffers[0]);

    char buffer[512 + 1];
    buffer[512] = '\0';
    if (bootloader_info_request.response != nullptr) {
        snprintf(buffer, 512, "Bootloader Info: %s - %s\n", bootloader_info_request.response->name,
                 bootloader_info_request.response->version);
        console.write(buffer);
    }

    console.write("Memory Mapping: \n");
    for (std::size_t i = 0; i < memmap_request.response->entry_count; ++i) {
        auto& entry = memmap_request.response->entries[i];
        snprintf(buffer, 512, "%.2lu: 0x%.8lx - 0x%.8lx - %lu \n", i, entry->base, entry->length, entry->type);
        console.write(buffer);
    }

    auto pageArray = kernel::boot::initPageArray(memmap_request);
    uint64_t kernelFileAddr = reinterpret_cast<uint64_t>(kernel_file_request.response->kernel_file->address);
    kernel::KernelInfo info = {
        .pageArray = pageArray.first,
        .pageCount = pageArray.second,
        .hhdm = hhdm_request.response->offset,
        .hhdmSize = pageArray.second * kernel::mem::PAGE_SIZE,
        .kernelFileAddr = kernelFileAddr,
        .kernelPhysBase = kernel_address_request.response->physical_base,
        .kernelVirtBase = kernel_address_request.response->virtual_base,

    };
    kernel::Kernel::start(info);

    // We're done, just hang...
    hcf();
}

#ifdef __cplusplus
}
#endif
