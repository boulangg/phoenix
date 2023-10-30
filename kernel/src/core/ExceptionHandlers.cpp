/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "ExceptionHandlers.h"

#include <list>
#include <vector>

#include "GlobalDescTable.h"
#include "InterruptDescTable.h"
#include "InterruptDispatcher.h"
#include "Kernel.h"
#include "KernelGlobals.h"

namespace kernel::core {

namespace ExceptionHandlers {

static void Exception_00_DIVZero(std::uint32_t)
{
    printk("INT: Divide-by-zero Error\n");
    while (1) {
    }
}

static void Exception_01_Debug(std::uint32_t)
{
    printk("INT: Debug\n");
    while (1) {
    }
}

static void Exception_02_NMI(std::uint32_t)
{
    printk("INT: NMI\n");
    while (1) {
    }
}
static void Exception_03_Breakpoint(std::uint32_t)
{
    printk("INT: Breakpoint\n");
    while (1) {
    }
}

static void Exception_04_Overflow(std::uint32_t)
{
    printk("INT: Overflow\n");
    while (1) {
    }
}

static void Exception_05_BoundRange(std::uint32_t)
{
    printk("INT: Bound Range\n");
    while (1) {
    }
}

static void Exception_06_InvalidOpcode(std::uint32_t)
{
    printk("INT: Invalid Opcode\n");
    while (1) {
    }
}

static void Exception_07_DeviceNotAvailable(std::uint32_t)
{
    printk("INT: Device Not Available\n");
    while (1) {
    }
}

static void Exception_08_DoubleFault(std::uint32_t errorCode)
{
    (void)errorCode;
    printk("INT (%u): Double Fault\n", errorCode);
    while (1) {
    }
}

static void Exception_0A_InvalidTSS(std::uint32_t errorCode)
{
    (void)errorCode;
    printk("INT (%u): Invalid TSS\n", errorCode);
    while (1) {
    }
}

static void Exception_0B_SegmentNotPresent(std::uint32_t errorCode)
{
    (void)errorCode;
    printk("INT (%u): Segment not Present\n", errorCode);
    while (1) {
    }
}

static void Exception_0C_StackSegmentFault(std::uint32_t errorCode)
{
    (void)errorCode;
    printk("INT (%u): Stack Segment Fault\n", errorCode);
    while (1) {
    }
}

static void Exception_0D_GPFault(std::uint32_t errorCode)
{
    (void)errorCode;
    printk("INT (%u): Debug General Protection Fault\n", errorCode);
    while (1) {
    }
}

static void Exception_0E_PageFault(std::uint32_t errorCode)
{
    std::uint64_t pfa = read_CR2();
    (void)pfa;
    (void)errorCode;
    printk("INT (%u): Page Fault on 0x%0.16x\n", errorCode, pfa);
    // ProcessScheduler::pageFault(errorCode, (void*)pfa);
    while (1) {
    }
}

static void Exception_10_x87FP(std::uint32_t)
{
    printk("INT: x87 Floating-Point Exception\n");
    while (1) {
    }
}

static void Exception_11_AlignmentCheck(std::uint32_t errorCode)
{
    (void)errorCode;
    printk("INT (%u): Alignment Check\n", errorCode);
    while (1) {
    }
}

static void Exception_12_MachineCheck(std::uint32_t)
{
    printk("INT: Machine Check\n");
    while (1) {
    }
}

static void Exception_13_SIMDFP(std::uint32_t)
{
    printk("INT (%u): SIMDFP\n");
    while (1) {
    }
}

static void Exception_14_Virtualization(std::uint32_t)
{
    printk("INT: Virtualization\n");
    while (1) {
    }
}

static void Exception_1E_Security(std::uint32_t errorCode)
{
    (void)errorCode;
    printk("INT (%u): Security\n", errorCode);
    while (1) {
    }
}

static void Exception_Reserved(std::uint32_t)
{
    printk("INT: Rserved\n");
    while (1) {
    }
}

static void fill_idt_descriptor_64(IDT::idt_desc idt[], uint8_t index, uint64_t offset, uint16_t selector,
                                   uint8_t flags, uint8_t ist)
{
    IDT::idt_desc* gate = &(idt[index]);

    gate->offset_1 = offset & 0xFFFF;
    gate->selector = selector & 0xFFFF;
    gate->ist = ist & 0x03;
    gate->reserved_1 = 0;
    gate->flags = flags & 0b11101111;
    gate->offset_2 = (offset >> 16) & 0xFFFF;
    gate->offset_3 = (offset >> 32) & 0xFFFFFFFF;
    gate->reserved_2 = 0;
}

void setupHandlers()
{
    static constexpr std::uint8_t IDT_FLAGS = IDT::ACCESS_P | IDT::ACCESS_DPL3 | IDT::GATE_TYPE_INTERRUPT;
    // Hardaware Exception handlers
    fill_idt_descriptor_64(IDT::idt, 0, EXCEPTION_HANDLER_NAME_UINT64(00), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 1, EXCEPTION_HANDLER_NAME_UINT64(01), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 2, EXCEPTION_HANDLER_NAME_UINT64(02), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 3, EXCEPTION_HANDLER_NAME_UINT64(03), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 4, EXCEPTION_HANDLER_NAME_UINT64(04), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 5, EXCEPTION_HANDLER_NAME_UINT64(05), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 6, EXCEPTION_HANDLER_NAME_UINT64(06), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 7, EXCEPTION_HANDLER_NAME_UINT64(07), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 8, EXCEPTION_HANDLER_NAME_UINT64(08), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 9, EXCEPTION_HANDLER_NAME_UINT64(09), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 10, EXCEPTION_HANDLER_NAME_UINT64(0A), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 11, EXCEPTION_HANDLER_NAME_UINT64(0B), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 12, EXCEPTION_HANDLER_NAME_UINT64(0C), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 13, EXCEPTION_HANDLER_NAME_UINT64(0D), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 14, EXCEPTION_HANDLER_NAME_UINT64(0E), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 15, EXCEPTION_HANDLER_NAME_UINT64(0F), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 16, EXCEPTION_HANDLER_NAME_UINT64(10), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 17, EXCEPTION_HANDLER_NAME_UINT64(10), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 18, EXCEPTION_HANDLER_NAME_UINT64(12), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 19, EXCEPTION_HANDLER_NAME_UINT64(13), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 20, EXCEPTION_HANDLER_NAME_UINT64(14), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 21, EXCEPTION_HANDLER_NAME_UINT64(15), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 22, EXCEPTION_HANDLER_NAME_UINT64(16), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 23, EXCEPTION_HANDLER_NAME_UINT64(17), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 24, EXCEPTION_HANDLER_NAME_UINT64(18), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 25, EXCEPTION_HANDLER_NAME_UINT64(19), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 26, EXCEPTION_HANDLER_NAME_UINT64(1A), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 27, EXCEPTION_HANDLER_NAME_UINT64(1B), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 28, EXCEPTION_HANDLER_NAME_UINT64(1C), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 29, EXCEPTION_HANDLER_NAME_UINT64(1D), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 30, EXCEPTION_HANDLER_NAME_UINT64(1E), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 31, EXCEPTION_HANDLER_NAME_UINT64(1F), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);

    // PIC interrupt handlers
    fill_idt_descriptor_64(IDT::idt, 32, INTERRUPT_HANDLER_NAME_UINT64(00), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 33, INTERRUPT_HANDLER_NAME_UINT64(01), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 34, INTERRUPT_HANDLER_NAME_UINT64(02), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 35, INTERRUPT_HANDLER_NAME_UINT64(03), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 36, INTERRUPT_HANDLER_NAME_UINT64(04), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 37, INTERRUPT_HANDLER_NAME_UINT64(05), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 38, INTERRUPT_HANDLER_NAME_UINT64(06), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 39, INTERRUPT_HANDLER_NAME_UINT64(07), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 40, INTERRUPT_HANDLER_NAME_UINT64(08), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 41, INTERRUPT_HANDLER_NAME_UINT64(09), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 42, INTERRUPT_HANDLER_NAME_UINT64(0A), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 43, INTERRUPT_HANDLER_NAME_UINT64(0B), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 44, INTERRUPT_HANDLER_NAME_UINT64(0C), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 45, INTERRUPT_HANDLER_NAME_UINT64(0D), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 46, INTERRUPT_HANDLER_NAME_UINT64(0E), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 47, INTERRUPT_HANDLER_NAME_UINT64(0F), GDT::KERNEL_CS_64_OFFSET, IDT_FLAGS, 0);
}

using exception_handler_t = void (*)(std::uint32_t);

static exception_handler_t exception_handlers[32] = {Exception_00_DIVZero,
                                                     Exception_01_Debug,
                                                     Exception_02_NMI,
                                                     Exception_03_Breakpoint,
                                                     Exception_04_Overflow,
                                                     Exception_05_BoundRange,
                                                     Exception_06_InvalidOpcode,
                                                     Exception_07_DeviceNotAvailable,
                                                     Exception_08_DoubleFault,
                                                     Exception_Reserved,
                                                     Exception_0A_InvalidTSS,
                                                     Exception_0B_SegmentNotPresent,
                                                     Exception_0C_StackSegmentFault,
                                                     Exception_0D_GPFault,
                                                     Exception_0E_PageFault,
                                                     Exception_Reserved,
                                                     Exception_10_x87FP,
                                                     Exception_11_AlignmentCheck,
                                                     Exception_12_MachineCheck,
                                                     Exception_13_SIMDFP,
                                                     Exception_14_Virtualization,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_1E_Security,
                                                     Exception_Reserved};
}

}

#ifdef __cplusplus
extern "C"
{
#endif

// C functions
void generic_exception_handler(std::uint32_t irq, std::uint32_t errocode)
{
    kernel::core::ExceptionHandlers::exception_handlers[irq](errocode);
}
void generic_interrupt_handler(std::uint32_t irq)
{
    kernel::Kernel::interrupt.handleIRQ(static_cast<uint8_t>(irq));
}

#ifdef __cplusplus
}
#endif
