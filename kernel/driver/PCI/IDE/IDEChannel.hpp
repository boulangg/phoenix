#pragma once

#include <cstdint>

#include <core/Console.hpp>
#include <driver/PCI/IDE/IDEDrive.hpp>
#include <driver/PCI/IDE/IDEStruct.hpp>

#include "IDEDrive.hpp"

#include <driver/BlockIO.hpp>



class IDEDevice;

class IDEChannel {

	static const std::uint8_t DISK_NUM = 2;

public:
	IDEChannel(IDEDevice* device, IDEChannelRegisters regs, int channelNo);

	virtual ~IDEChannel();

	virtual void processBlockIO(BlockIO bio, bool slave);

	/*void executeRequest() {
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
	}*/

	IDEDevice* device;    // Parent device

	std::uint16_t base;   // Data
	std::uint16_t ctrl;   // Control
	std::uint16_t bmIDE;  // BusMaster (DMA)
	std::uint8_t nIEN;    // Interrupt

	IDEDrive* disks[2];   // master and slave disk

	int channelNo;

private:
	void initDrives();
	IDEIdentifyData getIdentificationSpace();
	std::uint8_t readReg(std::uint8_t reg);
	void readBuffer(std::uint8_t reg, void* buffer, uint32_t size);
	void writeReg(std::uint8_t reg, std::uint8_t data);
	std::uint8_t ATAAccess(bool slave, bool write, std::uint64_t lba, char* buffer, std::uint32_t numSec);
	void poll();
	std::uint8_t checkErrors();

	/*std::uint8_t ATARead(std::uint64_t lba, char* buffer, size_t numSec) {
		return ATAAccess(false, lba, buffer, numSec);
	}

	std::uint8_t ATAWrite(std::uint64_t lba, char* buffer, size_t numSec) {
		return ATAAccess(true, lba, buffer, numSec);
	}*/
};
