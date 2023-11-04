
#pragma once

#include <vector>

#include "PCIManager.hpp"

class PCIDriver
{
public:
	PCIDriver()
	{}
	virtual ~PCIDriver()
	{}

/* List all device that might be compatible with this driver */
	virtual const std::vector<PCIDeviceID> getPCIDeviceID() = 0;

	/* Check if the PCI device can be handled by this driver */
	virtual int probe(PCIDevice* device) = 0;

	virtual void remove(PCIDevice* device) = 0;
	//virtual int suspend(PCIDevice* device, uint32_t state);
	//virtual int resume(PCIDevice* device);
};
