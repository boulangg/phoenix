/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "InterruptHandlers.h"

#include <list>
#include <vector>

#include "GlobalDescTable.h"
#include "InterruptDescTable.h"
#include "InterruptDispatcher.h"

namespace kernel::core {

namespace InterruptHandlers {

static void Exception_00_DIVZero(std::uint32_t)
{
    // cout << "INT: Divide-by-zero Error\n";
    while (1) {
    }
}

static void Exception_01_Debug(std::uint32_t)
{
    // cout << "INT: Debug\n";
    while (1) {
    }
}

static void Exception_02_NMI(std::uint32_t)
{
    // cout << "INT: NMI\n";
    while (1) {
    }
}
static void Exception_03_Breakpoint(std::uint32_t)
{
    // cout << "INT: Breakpoint\n";
    while (1) {
    }
}

static void Exception_04_Overflow(std::uint32_t)
{
    // cout << "INT: Overflow\n";
    while (1) {
    }
}

static void Exception_05_BoundRange(std::uint32_t)
{
    // cout << "INT: Bound Range\n";
    while (1) {
    }
}

static void Exception_06_InvalidOpcode(std::uint32_t)
{
    // cout << "INT: Invalid Opcode\n";
    while (1) {
    }
}

static void Exception_07_DeviceNotAvailable(std::uint32_t)
{
    // cout << "INT: Device Not Available\n";
    while (1) {
    }
}

static void Exception_08_DoubleFault(std::uint32_t errorCode)
{
    (void)errorCode;
    // cout << "INT: Double Fault\n";
    while (1) {
    }
}

static void Exception_10_InvalidTSS(std::uint32_t errorCode)
{
    (void)errorCode;
    // cout << "INT: Invalid TSS\n";
    while (1) {
    }
}

static void Exception_11_SegmentNotPresent(std::uint32_t errorCode)
{
    (void)errorCode;
    // cout << "INT: Segment not Present\n";
    while (1) {
    }
}

static void Exception_12_StackSegmentFault(std::uint32_t errorCode)
{
    (void)errorCode;
    // cout << "INT: Stack Segment Fault\n";
    while (1) {
    }
}

static void Exception_13_GPFault(std::uint32_t errorCode)
{
    (void)errorCode;
    // cout << "INT: Debug General Protection Fault\n";
    while (1) {
    }
}

static void Exception_14_PageFault(std::uint32_t errorCode)
{
    std::uint64_t pfa = readCR2();
    (void)pfa;
    (void)errorCode;
    // ProcessScheduler::pageFault(errorCode, (void*)pfa);
    // cout << "INT: Page Fault\n";
    while (1) {
    }
}

static void Exception_16_x87FP(std::uint32_t)
{
    // cout << "INT: x87 Floating-Point Excpetion\n";
    while (1) {
    }
}

static void Exception_17_AlignmentCheck(std::uint32_t errorCode)
{
    (void)errorCode;
    // cout << "INT: Alignment Check\n";
    while (1) {
    }
}

static void Exception_18_MachineCheck(std::uint32_t)
{
    // cout << "INT: Machine Check\n";
    while (1) {
    }
}

static void Exception_19_SIMDFP(std::uint32_t)
{
    // cout << "INT: SIMDFP\n";
    while (1) {
    }
}

static void Exception_20_Virtualization(std::uint32_t)
{
    // cout << "INT: Virtualization\n";
    while (1) {
    }
}

static void Exception_30_Security(std::uint32_t errorCode)
{
    (void)errorCode;
    // cout << "INT: Security\n";
    while (1) {
    }
}

static void Exception_Reserved(std::uint32_t)
{
    // cout << "INT: Rserved\n";
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
    fill_idt_descriptor_64(IDT::idt, 0, (uint64_t)EXCEPTION_HANDLER_NAME(00), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 1, (uint64_t)EXCEPTION_HANDLER_NAME(00), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 2, (uint64_t)EXCEPTION_HANDLER_NAME(02), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 3, (uint64_t)EXCEPTION_HANDLER_NAME(03), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 4, (uint64_t)EXCEPTION_HANDLER_NAME(04), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 5, (uint64_t)EXCEPTION_HANDLER_NAME(05), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 6, (uint64_t)EXCEPTION_HANDLER_NAME(06), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 7, (uint64_t)EXCEPTION_HANDLER_NAME(07), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 8, (uint64_t)EXCEPTION_HANDLER_NAME(08), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 9, (uint64_t)EXCEPTION_HANDLER_NAME(09), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 10, (uint64_t)EXCEPTION_HANDLER_NAME(0A), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 11, (uint64_t)EXCEPTION_HANDLER_NAME(0B), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 12, (uint64_t)EXCEPTION_HANDLER_NAME(0C), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 13, (uint64_t)EXCEPTION_HANDLER_NAME(0D), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 14, (uint64_t)EXCEPTION_HANDLER_NAME(0E), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 15, (uint64_t)EXCEPTION_HANDLER_NAME(0F), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 16, (uint64_t)EXCEPTION_HANDLER_NAME(10), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 17, (uint64_t)EXCEPTION_HANDLER_NAME(10), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 18, (uint64_t)EXCEPTION_HANDLER_NAME(12), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 19, (uint64_t)EXCEPTION_HANDLER_NAME(13), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 20, (uint64_t)EXCEPTION_HANDLER_NAME(14), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 21, (uint64_t)EXCEPTION_HANDLER_NAME(15), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 22, (uint64_t)EXCEPTION_HANDLER_NAME(16), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 23, (uint64_t)EXCEPTION_HANDLER_NAME(17), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 24, (uint64_t)EXCEPTION_HANDLER_NAME(18), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 25, (uint64_t)EXCEPTION_HANDLER_NAME(19), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 26, (uint64_t)EXCEPTION_HANDLER_NAME(1A), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 27, (uint64_t)EXCEPTION_HANDLER_NAME(1B), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 28, (uint64_t)EXCEPTION_HANDLER_NAME(1C), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 29, (uint64_t)EXCEPTION_HANDLER_NAME(1D), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 30, (uint64_t)EXCEPTION_HANDLER_NAME(1E), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 31, (uint64_t)EXCEPTION_HANDLER_NAME(1F), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);

    // PIC interrupt handlers
    fill_idt_descriptor_64(IDT::idt, 32, (uint64_t)INTERRUPT_HANDLER_NAME(00), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 33, (uint64_t)INTERRUPT_HANDLER_NAME(00), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 34, (uint64_t)INTERRUPT_HANDLER_NAME(02), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 35, (uint64_t)INTERRUPT_HANDLER_NAME(03), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 36, (uint64_t)INTERRUPT_HANDLER_NAME(04), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 37, (uint64_t)INTERRUPT_HANDLER_NAME(05), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 38, (uint64_t)INTERRUPT_HANDLER_NAME(06), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 39, (uint64_t)INTERRUPT_HANDLER_NAME(07), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 40, (uint64_t)INTERRUPT_HANDLER_NAME(08), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 41, (uint64_t)INTERRUPT_HANDLER_NAME(09), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 42, (uint64_t)INTERRUPT_HANDLER_NAME(0A), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 43, (uint64_t)INTERRUPT_HANDLER_NAME(0B), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 44, (uint64_t)INTERRUPT_HANDLER_NAME(0C), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 45, (uint64_t)INTERRUPT_HANDLER_NAME(0D), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 46, (uint64_t)INTERRUPT_HANDLER_NAME(0E), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
    fill_idt_descriptor_64(IDT::idt, 47, (uint64_t)INTERRUPT_HANDLER_NAME(0F), GDT::USER_CS_64_OFFSET, IDT_FLAGS, 0);
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
                                                     Exception_10_InvalidTSS,
                                                     Exception_11_SegmentNotPresent,
                                                     Exception_12_StackSegmentFault,
                                                     Exception_13_GPFault,
                                                     Exception_14_PageFault,
                                                     Exception_Reserved,
                                                     Exception_16_x87FP,
                                                     Exception_17_AlignmentCheck,
                                                     Exception_18_MachineCheck,
                                                     Exception_19_SIMDFP,
                                                     Exception_20_Virtualization,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_Reserved,
                                                     Exception_30_Security,
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
    kernel::core::InterruptHandlers::exception_handlers[irq](errocode);
}
void generic_interrupt_handler(std::uint32_t irq)
{
    kernel::core::InterruptDispatcher::handleIRQ(irq);
}

#ifdef __cplusplus
}
#endif
