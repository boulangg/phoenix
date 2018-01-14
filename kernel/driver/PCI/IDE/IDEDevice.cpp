#include "IDEDevice.hpp"

IDEDevice::IDEDevice(PCIDevice* device, int deviceID): deviceID(deviceID), device(device) {
    initDrives(device);
}

void IDEDevice::initDrives(PCIDevice* device){
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

		if(regs.bmIDE){
			device->setMaster(true);
		}

		primary = new IDEChannel(this, regs, deviceID*2);

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

		secondary = new IDEChannel(this, regs, deviceID*2+1);
}

bool IDEDevice::getMaster() {
    return this->device->getMaster();
}