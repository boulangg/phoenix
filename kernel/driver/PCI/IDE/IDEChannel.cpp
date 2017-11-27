#include "IDEChannel.hpp"

#include <driver/DeviceManager.hpp>

IDEChannel::IDEChannel(IDEDevice* device, IDEChannelRegisters regs, int channelNo) :
		device(device), base(regs.base), ctrl(regs.ctrl), bmIDE(regs.bmIDE),
		nIEN(regs.nIEN), channelNo(channelNo)
{
	initDrives();
}


IDEChannel::~IDEChannel() {
	for (std::uint8_t i = 0; i < DISK_NUM; i++) {
		delete disks[i];
	}
}

void IDEChannel::processBlockIO(BlockIO bio, bool slave) {
	std::uint64_t start_sector = bio.start_sector;
	for (auto segment: bio.segments) {
		ATAAccess(slave, bio.write, start_sector, (char*)segment.page->kernelMappAddr, 1);
		start_sector += 1;
	}
}

void IDEChannel::initDrives() {
	for (int i = 0; i < DISK_NUM; i++) {
		// Detect is device exist;
		writeReg(ATA_REG_HDDEVSEL, 0xA0 | i << 4);
		//sleep(1); TODO implement microsleep
		writeReg(ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
		//sleep(1); TODO implement microsleep

		if (readReg(ATA_REG_STATUS) == 0) {
			// No device
			continue;
		}

		// Detect device type
		uint8_t status;
		uint8_t type = IDE_UNKNOWN;
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
					type = IDE_UNKNOWN;
				}
				break;
			}
			if ((!(status & ATA_SR_BSY)) && (status & ATA_SR_DRQ))  {
				type = IDE_ATA;
				break;
			}
		}

		if (type == IDE_UNKNOWN) {
			continue;
		}

		IDEDrive* drive = nullptr;
		if (type == IDE_ATA) {
			drive = new IDEDrive(this, i, type, channelNo);
			drive->identifyData = getIdentificationSpace();

			if (drive->identifyData.commandSetSupportedEnabled[1] & (1 << 10)) {
				// Device support 48 LBA
				drive->nbSectors = drive->identifyData.max48LBA;
			} else {
				// Device uses CHS or 28-bit LBA
				drive->nbSectors = drive->identifyData.numAdressableSector;
			}

			// Read sector size (default = DEFAULT_SECTOR_SIZE)
			if ((drive->identifyData.physicalSectorSizePerLogicalSectorSize & 0xC000) == 0x4000) {
				if (drive->identifyData.physicalSectorSizePerLogicalSectorSize & 0x100) {
					drive->sectorSize = drive->identifyData.logicalSectorSize << 1;
				} else {
					drive->sectorSize = IDEDrive::DEFAULT_SECTOR_SIZE;
				}
			}

		} else if (type == IDE_ATAPI) {

		}

		disks[i] = drive;
		if (drive != nullptr) {
			DeviceManager::registerIDEDevice(drive);
		}
	}


}

IDEIdentifyData IDEChannel::getIdentificationSpace() {
	IDEIdentifyData buffer;

	writeReg(ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
	sleep(1);
	readBuffer(ATA_REG_DATA, (void*)&buffer, 128);

	return buffer;
}

std::uint8_t IDEChannel::readReg(std::uint8_t reg) {
	unsigned char result;
	if (reg < 0x08)
		result = inb(base  + reg - 0x00);
	else if (reg < 0x10)
		result = inb(ctrl  + reg - 0x08);
	else if (reg < 0x18)
		result = inb(bmIDE + reg - 0x10);
	return result;
}

void IDEChannel::readBuffer(std::uint8_t reg, void* buffer, uint32_t size) {
	if (reg < 0x08)
		insl(base  + reg - 0x00, buffer, size);
	else if (reg < 0x10)
		insl(ctrl  + reg - 0x08, buffer, size);
	else if (reg < 0x18)
		insl(bmIDE + reg - 0x10, buffer, size);
}

void IDEChannel::writeReg(std::uint8_t reg, std::uint8_t data) {
	if (reg < 0x08)
		outb(base  + reg - 0x00, data);
	else if (reg < 0x10)
		outb(ctrl  + reg - 0x08, data);
	else if (reg < 0x18)
		outb(bmIDE + reg - 0x10, data);
}

std::uint8_t IDEChannel::ATAAccess(bool slave, bool write, std::uint64_t lba, char* buffer, std::uint32_t numSec) {
	//char tmp[1024];
	//sprintf(tmp, "Access sector %i, R/W %i, numSec %i, sectorSize %i, diskSize %i bytes\n", lba, write, numSec, disks[slave]->sectorSize, disks[slave]->getSize());
	//Console::write(tmp);
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
	} else if (disks[slave]->identifyData.capabilities & 0x200)  { // Drive supports LBA?
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
		writeReg(ATA_REG_HDDEVSEL, 0xA0 | (slave << 4) | head); // Drive & CHS.
	else
		writeReg(ATA_REG_HDDEVSEL, 0xE0 | (slave << 4) | head); // Drive & LBA

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
	if (lba_mode == 0 && dma == 0 && write == 0) cmd = ATA_CMD_READ_PIO;
	if (lba_mode == 1 && dma == 0 && write == 0) cmd = ATA_CMD_READ_PIO;
	if (lba_mode == 2 && dma == 0 && write == 0) cmd = ATA_CMD_READ_PIO_EXT;
	if (lba_mode == 0 && dma == 1 && write == 0) cmd = ATA_CMD_READ_DMA;
	if (lba_mode == 1 && dma == 1 && write == 0) cmd = ATA_CMD_READ_DMA;
	if (lba_mode == 2 && dma == 1 && write == 0) cmd = ATA_CMD_READ_DMA_EXT;
	if (lba_mode == 0 && dma == 0 && write == 1) cmd = ATA_CMD_WRITE_PIO;
	if (lba_mode == 1 && dma == 0 && write == 1) cmd = ATA_CMD_WRITE_PIO;
	if (lba_mode == 2 && dma == 0 && write == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
	if (lba_mode == 0 && dma == 1 && write == 1) cmd = ATA_CMD_WRITE_DMA;
	if (lba_mode == 1 && dma == 1 && write == 1) cmd = ATA_CMD_WRITE_DMA;
	if (lba_mode == 2 && dma == 1 && write == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
	// Send the Command.
	writeReg(ATA_REG_COMMAND, cmd);

	if (dma) {
		if (write == 0) {
			// DMA Read.
		} else {
			// DMA Write.
		}
	} else {
		if (write == 0) {
			// PIO Read.
			//Console::write("PIO read\n");
			for (i = 0; i < numSec; i++) {
				poll();
				err = checkErrors();
				if (err) {
					//Console::write("Error\n");
					return err; // Polling, set error and exit if there is.
				}
				insw(base, buffer, disks[slave]->sectorSize/2);
				buffer += disks[slave]->sectorSize;
			}
		} else {
			// PIO Write.
			//Console::write("PIO write\n");
			for (i = 0; i < numSec; i++) {
				poll(); // Polling.
				outsw(base, buffer, disks[slave]->sectorSize/2);
				buffer += disks[slave]->sectorSize;
			}
			writeReg(ATA_REG_COMMAND, cache_cmd[lba_mode]); // Cache flush
			poll(); // Polling.
		}
	}
	//Console::write("ATAAccess successful\n");
	return 0;
}

void IDEChannel::poll() {
	for(int i = 0; i < 4; i++)
		readReg(ATA_REG_ALTSTATUS);

	while (readReg(ATA_REG_STATUS) & ATA_SR_BSY);
}

std::uint8_t IDEChannel::checkErrors() {
	unsigned char state = readReg(ATA_REG_STATUS);
	// TODO Log error messages
	if (state & ATA_SR_ERR)
		return 1; // Error.

	if (state & ATA_SR_DF)
		return 2; // Device Fault.

	if ((state & ATA_SR_DRQ) == 0)
		return 3; // DRQ should be set

	return 0;
}
