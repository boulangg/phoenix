#pragma once

#include <driver/PCI/IDE/IDEChannel.hpp>
#include <driver/PCI/IDE/IDEDrive.hpp>
#include <driver/PCI/IDE/IDEStruct.hpp>
#include <cstdint>

#include "../PCIManager.hpp"


class IDEDevice {
public:
	void initDrives(PCIDevice* device) {
		// Primary Channel
		IDEChannelRegisters regs;
		regs.base = device->getBAR(0);
		if (regs.base == 0x00 || regs.base == 0x01) {
			regs.base = 0x1F0;
		}
		regs.ctrl = device->getBAR(1);
		if (regs.ctrl == 0x00 || regs.ctrl == 0x01) {
			regs.ctrl = 0x3F6;
		}
		regs.bmIDE = device->getBAR(4);

		primary = new IDEChannel(this, regs);

		// Secondary Channel
		regs.base = device->getBAR(2);
		if (regs.base == 0x00 || regs.base == 0x01) {
			regs.base = 0x170;
		}
		regs.ctrl = device->getBAR(3);
		if (regs.ctrl == 0x00 || regs.ctrl == 0x01) {
			regs.ctrl = 0x376;
		}
		regs.bmIDE = device->getBAR(4) + 8;

		secondary = new IDEChannel(this, regs);
	}

	IDEChannel* primary;
	IDEChannel* secondary;
private:

	std::uint8_t readReg(std::uint8_t reg);
	std::uint8_t writeReg(std::uint8_t reg, std::uint8_t data);


};
