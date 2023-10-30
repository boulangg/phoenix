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
#include "asm/cpu.h"

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
    enable_SSE();
    limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

    auto pageArray = kernel::boot::initPageArray(memmap_request);
    uint64_t kernelFileAddr = reinterpret_cast<uint64_t>(kernel_file_request.response->kernel_file->address);

    kernel::KernelInfo info = {.pageArray = pageArray.first + hhdm_request.response->offset,
                               .pageCount = pageArray.second,
                               .hhdm = hhdm_request.response->offset,
                               .kernelFileAddr = kernelFileAddr,
                               .kernelFileSize = kernel_file_request.response->kernel_file->size,
                               .kernelPhysBase = kernel_address_request.response->physical_base,
                               .kernelVirtBase = kernel_address_request.response->virtual_base,
                               .framebuffer = {
                                   .address = static_cast<uint32_t*>(framebuffer->address),
                                   .width = framebuffer->width,
                                   .height = framebuffer->height,
                               }};

    kernel::Kernel::init(info);

    // We're done, just hang...
    hcf();
}

#ifdef __cplusplus
}
#endif
