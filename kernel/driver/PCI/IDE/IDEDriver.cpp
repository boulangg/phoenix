#include <driver/PCI/IDE/IDEDriver.hpp>

std::vector<PCIDeviceID> IDEDriver::_deviceIds;

IDEDriver::IDEDriver() {
	PCIDeviceID id1 = {.vendorID = PCI_ANY_ID, .deviceID = PCI_ANY_ID,
			.subVendorID = PCI_ANY_ID, .subDeviceID = PCI_ANY_ID,
			.classCode = 0x01, .classCodeMask = PCI_ANY_ID};
	_deviceIds.push_back(id1);
}

const std::vector<PCIDeviceID> IDEDriver::getPCIDeviceID() {
	return _deviceIds;
}

int IDEDriver::probe(PCIDevice* device) {
	char tmp[1024];
	sprintf(tmp, "IDE device found\n");
	Console::write(tmp);

	IDEDevice* IDEdevice = new IDEDevice();
	IDEdevice->initDrives(device);
	_IDEDevices.push_back(IDEdevice);

	return 0;
}

void IDEDriver::remove(PCIDevice* device) {
	(void)device;
}
