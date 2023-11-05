/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <limine.h>

#include "stddef.h"

#include "Kernel.h"
#include "MemoryInit.h"
#include "console/BasicConsole.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Limine requests
static volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0, .response = nullptr};
static limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 0, .response = nullptr};
static volatile limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST, .revision = 0, .response = nullptr};
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
    console.write("Hello World !!!\nLooks like it's working nicely :)");
    
    for (std::size_t i = 0; i < 0x1000; ++i) {
        if (i % 16 == 0) {
            console.write('\n');
        }
        char c = 32 + (i % 96);
        console.write(c);
    }
    
    auto pageArray = kernel::mem::initPageArray(memmap_request);
    kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING = hhdm_request.response->offset;
    kernel::Kernel::kernel.start(pageArray.first, pageArray.second);

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // We're done, just hang...
    hcf();
}

#ifdef __cplusplus
}
#endif
