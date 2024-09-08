/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Acpica.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <acpi.h>

#ifdef __cplusplus
}
#endif

#include "KernelGlobals.h"
#include "proc/Semaphore.h"
#include <climits>

// Environmental and ACPI Tables
ACPI_STATUS AcpiOsInitialize()
{
    return AE_OK;
}

ACPI_STATUS AcpiOsTerminate()
{
    return AE_OK;
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer()
{
    // TODO
    return AE_ERROR;
}

ACPI_STATUS AcpiOsPredefinedOverride(const ACPI_PREDEFINED_NAMES* PredefinedObject, ACPI_STRING* NewValue)
{
    (void)PredefinedObject;
    *NewValue = nullptr;
    return AE_OK;
}

ACPI_STATUS AcpiOsTableOverride(ACPI_TABLE_HEADER* ExistingTable, ACPI_TABLE_HEADER** NewTable)
{
    (void)ExistingTable;
    *NewTable = nullptr;
    return AE_OK;
}

ACPI_STATUS AcpiOsPhysicalTableOverride(ACPI_TABLE_HEADER* ExistingTable, ACPI_PHYSICAL_ADDRESS* NewAddress,
                                        UINT32* NewTableLength)
{
    (void)ExistingTable;
    (void)NewTableLength;
    *NewAddress = 0;
    return AE_OK;
}

// Memory Management
void* AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS PhysicalAddress, ACPI_SIZE Length)
{
    // TODO
    return nullptr;
}

void AcpiOsUnmapMemory(void* Where, ACPI_SIZE Length)
{
    (void)Where;
    (void)Length;
    return;
}

void* AcpiOsAllocate(ACPI_SIZE Size)
{
    return malloc(Size);
}

void AcpiOsFree(void* Memory)
{
    free(Memory);
}

// Multithreading and Scheduling Services
ACPI_THREAD_ID AcpiOsGetThreadId()
{
    // TODO
    return 0;
}

ACPI_STATUS AcpiOsExecute(ACPI_EXECUTE_TYPE Type, ACPI_OSD_EXEC_CALLBACK Function, void* Context)
{
    // TODO
    return AE_ERROR;
}

void AcpiOsSleep(UINT64 Milliseconds)
{
    // TODO
    return;
}

void AcpiOsStall(UINT32 Microseconds)
{
    // TODO
    return;
}

void AcpiOsWaitEventsComplete()
{
    // TODO
    return;
}

// Mutual Exclusion and Synchronization
ACPI_STATUS AcpiOsCreateSemaphore(UINT32 MaxUnits, UINT32 InitialUnits, ACPI_SEMAPHORE* OutHandle)
{
    kernel::proc::Semaphore* sem = new kernel::proc::Semaphore(MaxUnits, InitialUnits);
    *OutHandle = reinterpret_cast<void*>(sem);
    return AE_OK;
}

ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE Handle)
{
    kernel::proc::Semaphore* sem = reinterpret_cast<kernel::proc::Semaphore*>(Handle);
    delete sem;
    return AE_OK;
}

ACPI_STATUS AcpiOsWaitSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units, UINT16 Timeout)
{
    kernel::proc::Semaphore* sem = reinterpret_cast<kernel::proc::Semaphore*>(Handle);
    sem->wait(Units);
    return AE_OK;
}

ACPI_STATUS AcpiOsSignalSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units)
{
    kernel::proc::Semaphore* sem = reinterpret_cast<kernel::proc::Semaphore*>(Handle);
    sem->signal(Units);
    return AE_OK;
}

ACPI_STATUS AcpiOsCreateLock(ACPI_SPINLOCK* OutHandle)
{
    kernel::spinlock* lock = new kernel::spinlock();
    *OutHandle = reinterpret_cast<void*>(lock);
    return AE_ERROR;
}

void AcpiOsDeleteLock(ACPI_HANDLE Handle)
{
    kernel::spinlock* lock = reinterpret_cast<kernel::spinlock*>(Handle);
    delete lock;
    return;
}

ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK Handle)
{
    kernel::spinlock* lock = reinterpret_cast<kernel::spinlock*>(Handle);
    lock->lock();
    return 0;
}

void AcpiOsReleaseLock(ACPI_SPINLOCK Handle, ACPI_CPU_FLAGS Flags)
{
    kernel::spinlock* lock = reinterpret_cast<kernel::spinlock*>(Handle);
    lock->unlock();
    return;
}

// Interrupt Handling
ACPI_STATUS AcpiOsInstallInterruptHandler(UINT32 InterruptLevel, ACPI_OSD_HANDLER Handler, void* Context)
{
    // TODO
    return AE_ERROR;
}

ACPI_STATUS AcpiOsRemoveInterruptHandler(UINT32 InterruptNumber, ACPI_OSD_HANDLER Handler)
{
    // TODO
    return AE_ERROR;
}

// Memory Access and Memory Mapped I/O
ACPI_STATUS AcpiOsReadMemory(ACPI_PHYSICAL_ADDRESS Address, UINT64* Value, UINT32 Width)
{
    // TODO
    return AE_ERROR;
}

ACPI_STATUS AcpiOsWriteMemory(ACPI_PHYSICAL_ADDRESS Address, UINT64 Value, UINT32 Width)
{
    // TODO
    return AE_ERROR;
}

// Port Input/Output
ACPI_STATUS AcpiOsReadPort(ACPI_IO_ADDRESS Address, UINT32* Value, UINT32 Width)
{
    // TODO
    return AE_ERROR;
}

ACPI_STATUS AcpiOsWritePort(ACPI_IO_ADDRESS Address, UINT32 Value, UINT32 Width)
{
    // TODO
    return AE_ERROR;
}

// PCI Configuration Space Access
ACPI_STATUS AcpiOsReadPciConfiguration(ACPI_PCI_ID* PciId, UINT32 Reg, UINT64* Value, UINT32 Width)
{
    // TODO
    return AE_ERROR;
}

ACPI_STATUS AcpiOsWritePciConfiguration(ACPI_PCI_ID* PciId, UINT32 Reg, UINT64 Value, UINT32 Width)
{
    // TODO
    return AE_ERROR;
}

// Formatted Output
void AcpiOsPrintf(const char* Format, ...)
{
    va_list vl;
    va_start(vl, Format);
    kernel::printk(Format, vl);
    va_end(vl);
}

void AcpiOsVprintf(const char* Format, va_list Args)
{
    kernel::printk(Format, Args);
}

// Miscellaneous
UINT64 AcpiOsGetTimer(void)
{
    // TODO
    return 0;
}

ACPI_STATUS AcpiOsSignal(UINT32 Function, void* Info)
{
    // TODO
    return AE_ERROR;
}

// Other

namespace kernel::dev {

// ACPI_STATUS DisplayOneDevice(ACPI_HANDLE ObjHandle, UINT32 Level, void* Context, void** ReturnResult)
//{
//    ACPI_STATUS Status;
//    ACPI_DEVICE_INFO Info;
//    ACPI_BUFFER Path;
//    char Buffer[256];
//    Path.Length = sizeof(Buffer);
//    Path.Pointer = Buffer;
//    /* Get the full path of this device and print it */
//    Status = AcpiHandleToPathname(ObjHandle, &Path);
//    if (ACPI_SUCCESS(Status)) {
//        printk("%s\n", Path.Pointer));
//    }
//    /* Get the device info for this device and print it */
//    Status = AcpiGetDeviceInfo(ObjHandle, &Info);
//    if (ACPI_SUCCESS(Status)) {
//        printk(" HID: %.8X, ADR: %.8X, Status: %x\n", Info.HardwareId, Info.Address, Info.CurrentStatus));
//    }
//    ReturnResult = NULL;
//    return AE_OK;
//}

void Acpica::initAcpi()
{
    ACPI_STATUS status;
    status = AcpiInitializeSubsystem();
    status = AcpiInitializeTables(nullptr, 16, false);

    // ACPI_HANDLE SysBusHandle;
    // AcpiNameToHandle(0, NS_SYSTEM_BUS, &SysBusHandle);
    // printk("Display of all devices in the namespace:\n");
    // AcpiWalkNamespace(ACPI_TYPE_DEVICE, SysBusHandle, INT_MAX, DisplayOneDevice, NULL, NULL);

    status = AcpiLoadTables();
    status = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
    status = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
    (void)status;
}

}
