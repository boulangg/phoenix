#pragma once

class IDEDriver : public PCIDriver {
public:
	virtual PCIDeviceID* getPCIDeviceID();
	virtual int probe(PCIDevice* device);
	virtual void remove(PCIDevice* device);

private:
	std::list<PCIDevice*> _devices;

	static PCIDeviceID* _deviceIds;
};
