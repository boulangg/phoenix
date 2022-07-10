#pragma once

#include <cstdio>
#include <core/Console.hpp>
#include <driver/PCI/IDE/IDEDevice.hpp>
#include <driver/PCI/IDE/IDEDrive.hpp>

#include "../PCIDriver.hpp"

class IDEDriver : public PCIDriver {
public:
	IDEDriver();

	virtual const std::vector<PCIDeviceID> getPCIDeviceID();

	virtual int probe(PCIDevice* device);

	virtual void remove(PCIDevice* device);

	IDEDrive* getDrive() {
		return _IDEDevices.front()->primary->disks[0];
	}

	static std::string getNameForDrive(int channelNo, bool slave) {
		std::string name = "hd";
		int id = 2 * channelNo + (int)slave;
		if (id < 26) {
			name += std::string(1, 'a' + (char)id);
		} else {
			name += std::string(1, 'A' + (char)(id / 26));
			name += std::string(1, 'a' + (char)(id % 26));
		}
		return name;
	}

private:
	std::list<PCIDevice*> _devices;
	std::list<IDEDevice*> _IDEDevices;

	static std::vector<PCIDeviceID> _deviceIds;
};
