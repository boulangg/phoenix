
#include <cstdint>

#include <asm/io.h>

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
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC
// ATAPI only commands
#define      ATAPI_CMD_READ       0xA8
#define      ATAPI_CMD_EJECT      0x1B

// Drive type
#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

// Drive master/slave
#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

// Registers offset
#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

// Directions
#define      ATA_READ      0x00
#define      ATA_WRITE     0x01

typedef struct {
    unsigned char  boot_indicator;
    unsigned char  start_head;
    unsigned short start_sec_cyl;
    unsigned char  system_ID;
    unsigned char  end_head;
    unsigned short end_sec_cyl;
    unsigned long  starting_LBA;
    unsigned long  nb_sectors;
} partitionTableEntry;

typedef struct {
	partitionTableEntry partitions[4];
} partitionTable;

typedef struct {
    char bootstrap[436];
    char disk_ID[10];
    partitionTable partTable;
    char padding[2];
} MBR;

class BlockCache {
public:
	BlockCache(std::uint64_t blockSize, std::uint64_t blockCount);
	~BlockCache();

	char* getBlocCache();

	std::uint64_t blockSize;
	std::uint64_t blockCount;
};

class Partition : public BlockDevice {
public:
	Partition();
	~Partition();

	virtual int addRequest();

private:
	std::uint64_t _startSector;
	std::uint64_t _nbSects;
	int partno;
};

class IDEDrive : public BlockDevice {
public:
	IDEDrive();

	void init(IDEChannelRegisters regs, std::uint8_t slave) {
		_slave = slave;
		// Set I/O ports
		_channel = regs;

		// Disable IRQs
		writeReg(ATA_REG_CONTROL, 2);

		// Detect ATA-ATAPI devices
		uint8_t err = 0, type = IDE_ATA, status;
		writeReg(ATA_REG_HDDEVSEL, 0xA0 | _slave << 4);
		sleep(1);

		writeReg(ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
		sleep(1);

		if (readReg(ATA_REG_STATUS) == 0) {
			// No device
			return;
		}

		// Polling
		while (1) {
			status = readReg(ATA_REG_STATUS);
			if ((status & ATA_SR_ERR)) {
				// Not an ATA device
				// Probe for ATAPI Devices
				uint8_t cl = readReg(ATA_REG_LBA1);
				uint8_t ch = readReg(ATA_REG_LBA1);

				if (cl == 0x14 && ch == 0xEB) {
					type = IDE_ATAPI;
				} else if (cl == 0x69 && ch == 0x96) {
					type = IDE_ATAPI;
				} else {
					return;
				}
				break;
			}
			if ((!status & ATA_SR_BSY) && (status & ATA_SR_DRQ))  {
				break;
			}
		}

		// Read identification space of the device
		_identifyData = getIdentificationSpace();

		if (_identifyData.commandSetSupported[1] & (1 << 10)) {
			// Device support 48 LBA
			_nbSectors = _identifyData.max48LBA;
		} else {
			// Device uses CHS or 28-bit LBA
			_nbSectors = _identifyData.adressableSectorNumber;
		}

	}

	std::uint64_t getSize() {
		return _nbSectors*_identifyData.logicalSectorSize;
	}

	std::uint64_t getSectorSize() {
		return _identifyData.logicalSectorSize;
	}

	void executeRequest() {
		Request* current;
		while ((current = getNextRequest()) != nullptr) {
			if (current->type == 0) {
				// Read
				ATARead(current->lba, current->buffer, current->nb_sector);
			} else {
				// Write
				ATAWrite(current->lba, current->buffer, current->nb_sector);
			}
		}
	}

	void getDriveModel(char* buffer, size_t size) {
		uint64_t k;
		for (k = 0; (k < 40) && (k < size - 2); k += 2) {
			buffer[k] = _identifyData.modelNumber[k+1];
			buffer[k+1] = _identifyData.modelNumber[k];
		}
		buffer[k] = '\0';
	}

	IDEIdentifyData getIdentificationSpace() {
		IDEIdentifyData buffer;
		readBuffer(ATA_REG_COMMAND, (void*)buffer, 128);

		return buffer;
	}

	void poll() {
		for(int i = 0; i < 4; i++)
			readReg(ATA_REG_ALTSTATUS);

		while (readReg(ATA_REG_STATUS) & ATA_SR_BSY);
	}

	std::uint8_t checkErrors() {
		unsigned char state = readReg(ATA_REG_STATUS);

		if (state & ATA_SR_ERR)
			return 1; // Error.

		if (state & ATA_SR_DF)
			return 2; // Device Fault.

		if ((state & ATA_SR_DRQ) == 0)
			return 3; // DRQ should be set

		return 0;
	}

	std::uint8_t readReg(std::uint8_t reg) {
		unsigned char result;
		if (reg > 0x07 && reg < 0x0C)
			writeReg(ATA_REG_CONTROL, 0x80 | _channel.nIEN);
		if (reg < 0x08)
			result = inb(_channel.base  + reg - 0x00);
		else if (reg < 0x0C)
			result = inb(_channel.base  + reg - 0x06);
		else if (reg < 0x0E)
			result = inb(_channel.ctrl  + reg - 0x0A);
		else if (reg < 0x16)
			result = inb(_channel.bmIDE + reg - 0x0E);
		if (reg > 0x07 && reg < 0x0C)
			writeReg(ATA_REG_CONTROL, _channel.nIEN);
		return result;
	}

	void readBuffer(std::uint8_t reg, void* buffer, uint32_t size) {
		if (reg > 0x07 && reg < 0x0C)
			writeReg(ATA_REG_CONTROL, 0x80 | _channel.nIEN);
		//asm("pushw %es; movw %ds, %ax; movw %ax, %es");
		if (reg < 0x08)
			insl(_channel.base  + reg - 0x00, buffer, size);
		else if (reg < 0x0C)
			insl(_channel.base  + reg - 0x06, buffer, size);
		else if (reg < 0x0E)
			insl(_channel.ctrl  + reg - 0x0A, buffer, size);
		else if (reg < 0x16)
			insl(_channel.bmIDE + reg - 0x0E, buffer, size);
		//asm("popw %es;");
		if (reg > 0x07 && reg < 0x0C)
			writeReg(ATA_REG_CONTROL, _channel.nIEN);
	}

	void writeReg(std::uint8_t reg, std::uint8_t data) {
		if (reg > 0x07 && reg < 0x0C)
			writeReg(ATA_REG_CONTROL, 0x80 | _channel.nIEN);
		if (reg < 0x08)
			outb(_channel.base  + reg - 0x00, data);
		else if (reg < 0x0C)
			outb(_channel.base  + reg - 0x06, data);
		else if (reg < 0x0E)
			outb(_channel.ctrl  + reg - 0x0A, data);
		else if (reg < 0x16)
			outb(_channel.bmIDE + reg - 0x0E, data);
		if (reg > 0x07 && reg < 0x0C)
			writeReg(ATA_REG_CONTROL, _channel.nIEN);
	}

	std::uint8_t ATARead(std::uint64_t lba, char* buffer, size_t numSec);
	std::uint8_t ATAWrite(std::uint64_t lba, char* buffer, size_t numSec);

	std::uint8_t ATAAccess(std::uint8_t dir, std::uint64_t lba, char* buffer, size_t numSec) {
		unsigned char lba_mode, dma, cmd;
		unsigned char lba_io[6];
		unsigned short cyl, i;
		unsigned char head, sect, err;
		char cache_cmd[] = {ATA_CMD_CACHE_FLUSH,
							ATA_CMD_CACHE_FLUSH,
							ATA_CMD_CACHE_FLUSH_EXT};

		// Disable IRQ ??
		//writeReg(ATA_REG_CONTROL, _channel.nIEN....)

		// Select mode
		if (lba >= 0x10000000) {
			// LBA48:
			lba_mode  = 2;
			lba_io[0] = (lba & 0x000000FF) >> 0;
			lba_io[1] = (lba & 0x0000FF00) >> 8;
			lba_io[2] = (lba & 0x00FF0000) >> 16;
			lba_io[3] = (lba & 0xFF000000) >> 24;
			lba_io[4] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
			lba_io[5] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
			head      = 0; // Lower 4-bits of HDDEVSEL are not used here.
		} else if (_identifyData.capabilities & 0x200)  { // Drive supports LBA?
			// LBA28:
			lba_mode  = 1;
			lba_io[0] = (lba & 0x00000FF) >> 0;
			lba_io[1] = (lba & 0x000FF00) >> 8;
			lba_io[2] = (lba & 0x0FF0000) >> 16;
			lba_io[3] = 0; // These Registers are not used here.
			lba_io[4] = 0; // These Registers are not used here.
			lba_io[5] = 0; // These Registers are not used here.
			head      = (lba & 0xF000000) >> 24;
		} else {
			// CHS:
			lba_mode  = 0;
			sect      = (lba % 63) + 1;
			cyl       = (lba + 1  - sect) / (16 * 63);
			lba_io[0] = sect;
			lba_io[1] = (cyl >> 0) & 0xFF;
			lba_io[2] = (cyl >> 8) & 0xFF;
			lba_io[3] = 0;
			lba_io[4] = 0;
			lba_io[5] = 0;
			head      = (lba + 1  - sect) % (16 * 63) / (63); // Head number is written to HDDEVSEL lower 4-bits.
		}

		 // (II) See if drive supports DMA or not;
		dma = 0; // We don't support DMA

		// (III) Wait if the drive is busy;
		while (readReg(ATA_REG_STATUS) & ATA_SR_BSY); // Wait if busy.

		if (lba_mode == 0)
			writeReg(ATA_REG_HDDEVSEL, 0xA0 | (_slave << 4) | head); // Drive & CHS.
		else
			writeReg(ATA_REG_HDDEVSEL, 0xE0 | (_slave << 4) | head); // Drive & LBA

		// (V) Write Parameters;
		if (lba_mode == 2) {
			writeReg(ATA_REG_SECCOUNT1,   0);
			writeReg(ATA_REG_LBA3,   lba_io[3]);
			writeReg(ATA_REG_LBA4,   lba_io[4]);
			writeReg(ATA_REG_LBA5,   lba_io[5]);
		}
		writeReg(ATA_REG_SECCOUNT0,   numSec);
		writeReg(ATA_REG_LBA0,   lba_io[0]);
		writeReg(ATA_REG_LBA1,   lba_io[1]);
		writeReg(ATA_REG_LBA2,   lba_io[2]);

		// (VI) Select the command
		if (lba_mode == 0 && dma == 0 && dir == 0) cmd = ATA_CMD_READ_PIO;
		if (lba_mode == 1 && dma == 0 && dir == 0) cmd = ATA_CMD_READ_PIO;
		if (lba_mode == 2 && dma == 0 && dir == 0) cmd = ATA_CMD_READ_PIO_EXT;
		if (lba_mode == 0 && dma == 1 && dir == 0) cmd = ATA_CMD_READ_DMA;
		if (lba_mode == 1 && dma == 1 && dir == 0) cmd = ATA_CMD_READ_DMA;
		if (lba_mode == 2 && dma == 1 && dir == 0) cmd = ATA_CMD_READ_DMA_EXT;
		if (lba_mode == 0 && dma == 0 && dir == 1) cmd = ATA_CMD_WRITE_PIO;
		if (lba_mode == 1 && dma == 0 && dir == 1) cmd = ATA_CMD_WRITE_PIO;
		if (lba_mode == 2 && dma == 0 && dir == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
		if (lba_mode == 0 && dma == 1 && dir == 1) cmd = ATA_CMD_WRITE_DMA;
		if (lba_mode == 1 && dma == 1 && dir == 1) cmd = ATA_CMD_WRITE_DMA;
		if (lba_mode == 2 && dma == 1 && dir == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
		// Send the Command.
		writeReg(ATA_REG_COMMAND, cmd);

		if (dma) {
			if (dir == 0) {
				// DMA Read.
			} else {
				// DMA Write.
			}
		} else {
			if (dir == 0) {
				// PIO Read.
				for (i = 0; i < numSec; i++) {
					poll();
					err = checkErrors()
					if (err) {
						return err; // Polling, set error and exit if there is.
					}
					insw(_channel.base, buffer, getSectorSize());
					buffer += getSectorSize();
				}
			} else {
				// PIO Write.
				for (i = 0; i < numSec; i++) {
					poll(); // Polling.
					outsw(_channel.base, buffer, getSectorSize());
					buffer += getSectorSize();
				}
				writeReg(ATA_REG_COMMAND, cache_cmd[lba_mode]);
				poll(); // Polling.
			}
		}
		return 0;
	}

	int readMBR() {
		Page* p = PhysicalAllocator::allocPage();
	}

	bool _exists;					// 0 (Empty) or 1 (Drive exists)
	IDEChannelRegisters _channel;
	std::uint8_t _slave; 			// 0 (Master Drive) or 1 (Slave Drive)
	std::uint8_t _type;				// 0: ATA, 1:ATAPI.
	IDEIdentifyData _identifyData;
	std::uint64_t _nbSectors;		// number of logical sector on the drive
};
