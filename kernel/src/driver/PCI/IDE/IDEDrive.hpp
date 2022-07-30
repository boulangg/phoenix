#pragma once


#include <cstdint>
#include <cstdlib>
#include <cstdio>

#include <asm/io.h>
#include <driver/PCI/IDE/IDEStruct.hpp>

#include <driver/BlockIO.hpp>
#include <driver/BlockStorageDevice.hpp>


struct IDEChannelRegisters {
	std::uint16_t base;
	std::uint16_t ctrl;
	std::uint16_t bmIDE;
	std::uint8_t nIEN;
};

// Status
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Inlex
#define ATA_SR_ERR     0x01    // Error

// Error
#define ATA_ER_BBK      0x80    // Bad sector
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // No media
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // No media
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark

// Commands
#define ATA_CMD_READ_PIO          (char)0x20
#define ATA_CMD_READ_PIO_EXT      (char)0x24
#define ATA_CMD_READ_DMA          (char)0xC8
#define ATA_CMD_READ_DMA_EXT      (char)0x25
#define ATA_CMD_WRITE_PIO         (char)0x30
#define ATA_CMD_WRITE_PIO_EXT     (char)0x34
#define ATA_CMD_WRITE_DMA         (char)0xCA
#define ATA_CMD_WRITE_DMA_EXT     (char)0x35
#define ATA_CMD_CACHE_FLUSH       (char)0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   (char)0xEA
#define ATA_CMD_PACKET            (char)0xA0
#define ATA_CMD_IDENTIFY_PACKET   (char)0xA1
#define ATA_CMD_IDENTIFY          (char)0xEC
// ATAPI only commands
#define      ATAPI_CMD_READ       0xA8
#define      ATAPI_CMD_EJECT      0x1B

// Drive type
#define IDE_ATA          0x00
#define IDE_ATAPI        0x01
#define IDE_UNKNOWN      0x02

// Drive master/slave
#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

// Registers offset
#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_SECCOUNT1  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_LBA3       0x03
#define ATA_REG_LBA4       0x04
#define ATA_REG_LBA5       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_CONTROL    0x02
#define ATA_REG_ALTSTATUS  0x02
#define ATA_REG_DEVADDRESS 0x03

// Channels
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

// Directions
#define      ATA_READ      0x00
#define      ATA_WRITE     0x01

typedef std::uint8_t IDEDiskType;

class IDEChannel;

class IDEDrive : public BlockStorageDevice {

public:
	virtual std::uint64_t getStorageSize() override ;
	virtual std::uint32_t getSectorSize();
	virtual std::uint64_t getSectorNumber() {
		return nbSectors;
	}


	void getDriveModel(char* buffer, size_t size) {
		uint64_t k;
		for (k = 0; (k < 40) && (k < size - 2); k += 2) {
			buffer[k] = identifyData.modelNumber[k+1];
			buffer[k+1] = identifyData.modelNumber[k];
		}
		buffer[k] = '\0';
	}

private:
	static const uint32_t DEFAULT_SECTOR_SIZE = 512;

	IDEDrive(IDEChannel* channel, bool slave, IDEDiskType type, int channelNo);

	virtual ~IDEDrive();

	virtual void processBlockIO(BlockIO bio);

	IDEChannel* channel;
	bool slave;
	IDEDiskType type; // ATA or ATAPI

	IDEIdentifyData identifyData;
	std::uint64_t nbSectors;
	std::uint32_t sectorSize;

	friend class IDEChannel;
};
