#pragma once

#include <driver/PCI/IDE/IDEChannel.hpp>
#include <cstdint>

#include <driver/PCI/PCIManager.hpp>

class IDEDevice
{
public:
	IDEDevice(PCIDevice* device, int deviceID);

	bool getMaster();

	IDEChannel* primary;
	IDEChannel* secondary;
private:

	void initDrives(PCIDevice* device);

	std::uint8_t readReg(std::uint8_t reg);
	std::uint8_t writeReg(std::uint8_t reg, std::uint8_t data);
	int deviceID;
	PCIDevice* device;
};
