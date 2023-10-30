/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "IDEChannel.h"

#include "Kernel.h"
#include "asm/cpu.h"

#include "ATADisk.h"

namespace kernel::dev::pci::ide {

namespace ATA {

struct Status
{
    static constexpr std::uint8_t BSY = 0x80;  // Busy
    static constexpr std::uint8_t DRDY = 0x40; // Drive ready
    static constexpr std::uint8_t DF = 0x20;   // Drive write fault
    static constexpr std::uint8_t DSC = 0x10;  // Drive seek complete
    static constexpr std::uint8_t DRQ = 0x08;  // Data request ready
    static constexpr std::uint8_t CORR = 0x04; // Corrected data
    static constexpr std::uint8_t IDX = 0x02;  // Index
    static constexpr std::uint8_t ERR = 0x01;  // Error
};

struct Error
{
    static constexpr std::uint8_t BBK = 0x80;   // Bad sector
    static constexpr std::uint8_t UNC = 0x40;   // Uncorrectable data
    static constexpr std::uint8_t MC = 0x20;    // No media
    static constexpr std::uint8_t IDNF = 0x10;  // ID mark not found
    static constexpr std::uint8_t MCR = 0x08;   // No media
    static constexpr std::uint8_t ABRT = 0x04;  // Command aborted
    static constexpr std::uint8_t TK0NF = 0x02; // Track 0 not found
    static constexpr std::uint8_t AMNF = 0x01;  // No address mark
};

struct Command
{
    static constexpr std::uint8_t READ_PIO = 0x20;
    static constexpr std::uint8_t READ_PIO_EXT = 0x24;
    static constexpr std::uint8_t READ_DMA = 0xC8;
    static constexpr std::uint8_t READ_DMA_EXT = 0x25;
    static constexpr std::uint8_t WRITE_PIO = 0x30;
    static constexpr std::uint8_t WRITE_PIO_EXT = 0x34;
    static constexpr std::uint8_t WRITE_DMA = 0xCA;
    static constexpr std::uint8_t WRITE_DMA_EXT = 0x35;
    static constexpr std::uint8_t CACHE_FLUSH = 0xE7;
    static constexpr std::uint8_t CACHE_FLUSH_EXT = 0xEA;
    static constexpr std::uint8_t PACKET = 0xA0;
    static constexpr std::uint8_t IDENTIFY_PACKET = 0xA1;
    static constexpr std::uint8_t IDENTIFY = 0xEC;
};

struct DriveType
{
    static constexpr std::uint8_t IDE_ATA = 0x00;
    static constexpr std::uint8_t IDE_ATAPI = 0x01;
    static constexpr std::uint8_t IDE_UNKNOWN = 0x02;
};

struct RegisterOffset
{
    static constexpr std::uint8_t BASE_DATA = 0x00;      // Read-Write
    static constexpr std::uint8_t BASE_ERROR = 0x01;     // Read only
    static constexpr std::uint8_t BASE_FEATURES = 0x01;  // Read only
    static constexpr std::uint8_t BASE_SECCOUNT0 = 0x02; // Read-Write
    static constexpr std::uint8_t BASE_LBA0 = 0x03;      // Read-Write
    static constexpr std::uint8_t BASE_LBA1 = 0x04;      // Read-Write
    static constexpr std::uint8_t BASE_LBA2 = 0x05;      // Read-Write
    static constexpr std::uint8_t BASE_DEVSEL = 0x06;    // Read-Write
    static constexpr std::uint8_t BASE_COMMAND = 0x07;   // Write only
    static constexpr std::uint8_t BASE_STATUS = 0x07;    // Read only
    static constexpr std::uint8_t BASE_SECCOUNT1 = 0x02; // Read-Write
    static constexpr std::uint8_t BASE_LBA3 = 0x03;      // Read-Write
    static constexpr std::uint8_t BASE_LBA4 = 0x04;      // Read-Write
    static constexpr std::uint8_t BASE_LBA5 = 0x05;      // Read-Write

    static constexpr std::uint8_t CTRL_DEVCTRL = 0x08 + 0x04;   // Write only
    static constexpr std::uint8_t CTRL_ALTSTATUS = 0x08 + 0x04; // Read only

    static constexpr std::uint8_t BMIDE_COMMAND = 0x10 + 0x00;
    static constexpr std::uint8_t BMIDE_STATUS = 0x10 + 0x02;
    static constexpr std::uint8_t BMIDE_PRDT = 0x10 + 0x04;
};

}

namespace ATAPI {

struct Command
{
    // ATAPI only commands
    static constexpr std::uint8_t READ = 0xA8;
    static constexpr std::uint8_t EJECT = 0x1B;
};

}

constexpr std::uint32_t IDE_DISK_MAJOR = 8;
constexpr std::uint32_t IDE_DISK_MINOR_PER_DISK = 16;
constexpr const char* IDE_DISK_NAME_PREFIX = "sd";

static std::string getIdeDiskName(bool isPrimaryChannel, bool isSlave)
{
    std::string name = std::string(IDE_DISK_NAME_PREFIX);
    name.push_back(char('a' + int(!isPrimaryChannel) * 2 + isSlave));
    return name;
}

static std::uint32_t getIdeDiskMajor(bool isPrimaryChannel, bool isSlave)
{
    (void)isPrimaryChannel;
    (void)isSlave;
    return IDE_DISK_MAJOR;
}

static std::uint32_t getIdeDiskMinor(bool isPrimaryChannel, bool isSlave)
{
    return IDE_DISK_MINOR_PER_DISK * (int(!isPrimaryChannel) * 2 + int(isSlave));
}

IDEChannel::IDEChannel(IDEDevice* parent, bool isPrimary, IDEChannelRegisters regs, std::uint8_t irqNum) :
    _parent(parent), _isPrimary(isPrimary), _regs(regs), _irqNum(irqNum)
{}

IDEChannel::~IDEChannel()
{
    for (std::uint8_t i = 0; i < IDE_DISK_PAR_CHANNEL; i++) {
        if (_disks[i] != nullptr) {
            delete _disks[i];
        }
    }
}

void IDEChannel::initDrives()
{
    // TODO: fix disk detection
    for (int i = 0; i < IDE_DISK_PAR_CHANNEL; i++) {
        // (II) Select Disk:
        selectDrive(i);

        // (II) Identify Disk:
        writeReg(ATA::RegisterOffset::BASE_COMMAND, ATA::Command::IDENTIFY);
        nanosleep(1000);

        // (III) Polling:
        if (readReg(ATA::RegisterOffset::BASE_STATUS) == 0)
            continue; // If Status = 0, No Device.

        std::uint8_t devSel = readReg(ATA::RegisterOffset::BASE_DEVSEL);
        (void)devSel;
        bool err = false;
        while (1) {
            std::uint8_t status = readReg(ATA::RegisterOffset::BASE_STATUS);
            if ((status & ATA::Status::ERR) || (status & ATA::Status::DF)) {
                err = true;
                break;
            } // If Err, Device is not ATA.

            if (!(status & ATA::Status::BSY) && (status & ATA::Status::DRQ))
                break; // Everything is right.
        }
        std::uint8_t type = ATA::DriveType::IDE_ATA;

        // (IV) Probe for ATAPI Devices:
        if (err) {
            std::uint8_t cl = readReg(ATA::RegisterOffset::BASE_LBA1);
            std::uint8_t ch = readReg(ATA::RegisterOffset::BASE_LBA2);

            if (cl == 0x14 && ch == 0xEB)
                type = ATA::DriveType::IDE_ATAPI;
            else if (cl == 0x69 && ch == 0x96)
                type = ATA::DriveType::IDE_ATAPI;
            else
                continue; // Unknown Type (may not be a device).

            writeReg(ATA::RegisterOffset::BASE_COMMAND, ATA::Command::IDENTIFY_PACKET);
            nanosleep(1000);
        }

        IDEDisk* drive = nullptr;
        switch (type) {
        case ATA::DriveType::IDE_ATA:
        {
            IDEIdentifyData data;
            readBuffer(ATA::RegisterOffset::BASE_DATA, (void*)&data, 128);
            std::uint8_t status = readReg(ATA::RegisterOffset::BASE_STATUS);
            if (status & ATA::Status::DRQ)
                break; // TODO: Something is wrong: fix disk detection
            drive = new ATADisk(this, getIdeDiskMajor(_isPrimary, i), getIdeDiskMinor(_isPrimary, i),
                                getIdeDiskName(_isPrimary, i), _irqNum, i, data);
            break;
        }
        default:
            continue;
        }

        _disks[i] = drive;
        if (drive != nullptr) {
            Kernel::deviceExplorer.addDisk(drive);
        }
    }
}

void IDEChannel::selectDrive(bool slave)
{
    writeReg(ATA::RegisterOffset::BASE_DEVSEL, 0xA0 | (slave << 4));
    nanosleep(400);
}

void IDEChannel::nanosleep(std::uint64_t nsec)
{
    // TODO Make the process sleep properly instead ?
    auto limit = Kernel::getDateTime();
    limit.tv_nsec += nsec;
    if (limit.tv_nsec >= core::clock::NSEC_PER_SEC) {
        limit.tv_nsec -= core::clock::NSEC_PER_SEC;
        limit.tv_sec += 1;
    }
    struct timespec current;
    do {
        readReg(ATA::RegisterOffset::CTRL_ALTSTATUS);
        current = Kernel::getDateTime();
    } while ((limit.tv_sec > current.tv_sec) ||
             ((limit.tv_sec == current.tv_sec) && (limit.tv_nsec > current.tv_nsec)));
}

void IDEChannel::executeDMARequest(bool slave, mem::Page* prdt, std::uint64_t lba, std::uint32_t numSec, bool write)
{
    // set command to 0
    outb(_regs.bmIDE + 0x00, 0);

    // clear IRQ and error flags
    outb(_regs.bmIDE + 0x02, 0x04 | 0x02); // 4 = IRQ, 2 = ERR

    // set PRDT address
    outl(_regs.bmIDE + 0x04, prdt->getPhysicalAddr());

    // set transfer direction
    if (write) {
        outb(_regs.bmIDE + 0x00, 1 << 3);
    } else {
        outb(_regs.bmIDE + 0x00, 0);
    }

    sendRWCommand(slave, write, lba, numSec, true /*isDma*/);

    // start DMA
    outb(_regs.bmIDE + 0x00, 1);
}

void IDEChannel::executePIORequest(const BlockIORequest& request, std::uint8_t blockShift, bool isSlave)
{
    std::uint32_t numSec = 0;
    for (auto& entry : request.entries) {
        numSec += entry.len >> blockShift;
    }

    sendRWCommand(isSlave, request.write, request.start, numSec, false /*isDMA*/);

    if (request.write) {
        // PIO Write
        for (auto& entry : request.entries) {
            std::uint64_t buffer = entry.page->getKernelAddr() + entry.offset;
            poll();
            outsw(_regs.base, (std::uint64_t*)(buffer), entry.len / 2);
        }
        // Cache flush
        if (request.start >= 0x10000000) {
            writeReg(ATA::RegisterOffset::BASE_COMMAND, ATA::Command::CACHE_FLUSH_EXT);
        } else {
            writeReg(ATA::RegisterOffset::BASE_COMMAND, ATA::Command::CACHE_FLUSH);
        }
    } else {
        // PIO Read
        for (auto& entry : request.entries) {
            std::uint64_t buffer = entry.page->getKernelAddr() + entry.offset;
            poll();
            insw(_regs.base, (std::uint64_t*)(buffer), entry.len / 2);
        }
    }
}

void IDEChannel::sendRWCommand(bool isSlave, bool write, std::uint64_t lba, std::uint32_t numSec, bool isDma)
{
    bool lba_48_mode = false;
    unsigned char cmd;
    unsigned char lba_io[6], head;

    // (I) Prepare lba registers
    if (lba >= 0x10000000) {
        // LBA48:
        lba_48_mode = true;
        lba_io[0] = (lba >> 0) & 0xFF;
        lba_io[1] = (lba >> 8) & 0xFF;
        lba_io[2] = (lba >> 16) & 0xFF;
        lba_io[3] = (lba >> 24) & 0xFF;
        lba_io[4] = (lba >> 32) & 0xFF;
        lba_io[5] = (lba >> 40) & 0xFF;
        head = 0x00; // Lower 4-bits of HDDEVSEL are not used here.
    } else {
        // LBA28:
        lba_48_mode = false;
        lba_io[0] = (lba >> 0) & 0xFF;
        lba_io[1] = (lba >> 8) & 0xFF;
        lba_io[2] = (lba >> 16) & 0xFF;
        lba_io[3] = 0; // These Registers are not used here.
        lba_io[4] = 0; // These Registers are not used here.
        lba_io[5] = 0; // These Registers are not used here.
        head = ((lba >> 24) & 0x0F);
    }

    // (II) Wait if the drive is busy;
    while (readReg(ATA::RegisterOffset::BASE_STATUS) & ATA::Status::BSY)
        ; // Wait if busy.

    // (III) Select drive + (IV) Write Parameters
    writeReg(ATA::RegisterOffset::BASE_DEVSEL, 0x40 | (0x20 * isSlave) | head); // LBA & drive & highest bits for lba
    if (lba_48_mode) {
        // Write higher bytes of LBA first
        writeReg(ATA::RegisterOffset::BASE_SECCOUNT1, 0);
        writeReg(ATA::RegisterOffset::BASE_LBA3, lba_io[3]);
        writeReg(ATA::RegisterOffset::BASE_LBA4, lba_io[4]);
        writeReg(ATA::RegisterOffset::BASE_LBA5, lba_io[5]);
    }
    // Write lower bytes of LBA
    writeReg(ATA::RegisterOffset::BASE_SECCOUNT0, numSec);
    writeReg(ATA::RegisterOffset::BASE_LBA0, lba_io[0]);
    writeReg(ATA::RegisterOffset::BASE_LBA1, lba_io[1]);
    writeReg(ATA::RegisterOffset::BASE_LBA2, lba_io[2]);

    // (V) Select the command
    if (isDma) {
        if (lba_48_mode) {
            if (write) {
                cmd = ATA::Command::WRITE_DMA_EXT;
            } else {
                cmd = ATA::Command::READ_DMA_EXT;
            }
        } else {
            if (write) {
                cmd = ATA::Command::WRITE_DMA;
            } else {
                cmd = ATA::Command::READ_DMA;
            }
        }
    } else {
        if (lba_48_mode) {
            if (write) {
                cmd = ATA::Command::WRITE_PIO_EXT;
            } else {
                cmd = ATA::Command::READ_PIO_EXT;
            }
        } else {
            if (write) {
                cmd = ATA::Command::WRITE_PIO;
            } else {
                cmd = ATA::Command::READ_PIO;
            }
        }
    }

    // (VI) Send the Command.
    writeReg(ATA::RegisterOffset::BASE_COMMAND, cmd);
}

void IDEChannel::poll()
{
    for (int i = 0; i < 4; i++) {
        readReg(ATA::RegisterOffset::CTRL_ALTSTATUS);
    }

    while (readReg(ATA::RegisterOffset::BASE_STATUS) & ATA::Status::BSY)
        ;
}

bool IDEChannel::validateDMATransfer()
{
    printk("validateDMATransfer\n");
    std::uint8_t status = inb(_regs.bmIDE + 0x02);
    auto status2 = readReg(ATA::RegisterOffset::BASE_STATUS);
    printk("status %x - %x\n", status, status2);

    // Clear IRQ bit
    outb(_regs.bmIDE + 0x02, status);

    if ((status & 0x04) == 0) {
        printk("other drive\n");
        // IRQ not from this drive
        return false;
    }

    // if ((status & 0x01)) {
    //      Pending request
    //     printk("DMA Pending work\n");
    //     return false;
    // }

    while (readReg(ATA::RegisterOffset::BASE_STATUS) & (ATA::Status::BSY | ATA::Status::DRQ))
        ;

    // stop DMA
    // outb(_regs.bmIDE + 0x00, 0);

    if ((status & 0x02) == 0x02) {
        // Error occurs
        printk("DMA transfer error\n");
        // CLear error
        outb(_regs.bmIDE + 0x02, 0x02);
        return true;
    }

    status = inb(_regs.bmIDE + 0x02);
    status2 = readReg(ATA::RegisterOffset::BASE_STATUS);
    printk("status %x, %x\n", status, status2);

    return true;
}

std::uint8_t IDEChannel::readReg(std::uint8_t reg)
{
    unsigned char result = 0;
    if (reg < 0x08)
        result = inb(_regs.base + reg - 0x00);
    else if (reg < 0x10)
        result = inb(_regs.ctrl + reg - 0x08);
    else if (reg < 0x18)
        result = inb(_regs.bmIDE + reg - 0x10);
    return result;
}

void IDEChannel::readBuffer(std::uint8_t reg, void* buffer, uint32_t size)
{
    if (reg < 0x08)
        insl(_regs.base + reg - 0x00, buffer, size);
    else if (reg < 0x10)
        insl(_regs.ctrl + reg - 0x08, buffer, size);
    else if (reg < 0x18)
        insl(_regs.bmIDE + reg - 0x10, buffer, size);
}

void IDEChannel::writeReg(std::uint8_t reg, std::uint8_t data)
{
    if (reg < 0x08)
        outb(_regs.base + reg - 0x00, data);
    else if (reg < 0x10)
        outb(_regs.ctrl + reg - 0x08, data);
    else if (reg < 0x18)
        outb(_regs.bmIDE + reg - 0x10, data);
}

}
