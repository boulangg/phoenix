
#include "PCIManager.hpp"

#include <cstdio>

#include <asm/io.h>
#include <core/Console.hpp>

#include "PCIDriver.hpp"

#include "../Disk.hpp"

PCIDevice::PCIDevice(uint8_t bus, uint8_t slot, uint8_t function) {
	this->bus = bus;
	this->slot = slot;
	this->function = function;

	PCIManager::readPCIConfigSpace(bus, slot, function, this->configSpace);
}

uint16_t PCIDevice::readPCIConfigLine(uint8_t offset){
	return PCIManager::readPCIConfigLine(this->bus, this->slot, this->function, offset);
}

void PCIDevice::writePCIConfigLine(uint8_t offset, uint32_t data){
	PCIManager::writePCIConfigLine(this->bus, this->slot, this->function, offset, data);
}

bool PCIDevice::getMaster(){
	return this->configSpace.command & 0x4;
}

void PCIDevice::setMaster(bool enable){
	uint32_t data;
	if(enable){
		data = this->configSpace.command | 0x4;
	} else {
		data = this->configSpace.command & (~(0x4));
	}
	this->writePCIConfigLine(0x4, data);
	this->configSpace.PCIConfigSpace1 = this->readPCIConfigLine(0x4);
}

std::list<PCIDevice*> PCIManager::_devices;
std::list<PCIDevice*> PCIManager::_unkonwDevices;
std::list<PCIDriver*> PCIManager::_drivers;

void PCIManager::initPCI() {
	uint8_t bus;
	uint8_t slot;

	for(bus = 0; bus < 32; bus++) {
		for(slot = 0; slot < 32; slot++) {
			checkDevice(bus, slot);
		}
	}
}

void PCIManager::registerDriver(PCIDriver* driver) {
	_drivers.push_back(driver);
	for (auto device : _unkonwDevices) {
		if (matchDeviceDriver(device, driver) != 0) {
			continue;
		} else {
			break;
		}
	}
}

void PCIManager::unregisterDriver(PCIDriver* driver) {
	// TODO implement it
	(void)driver;
}

void PCIManager::printDeviceInfo(PCIDevice* dev) {
	uint32_t bus = dev->bus;
	uint32_t slot = dev->slot;
	uint32_t func = dev->function;
	uint32_t deviceType = dev->getDeviceType();
	char tmp[1024];
	switch(deviceType >> 8) {
	case 0x10180:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: IDE Controller\n", bus, slot, func, deviceType);
		break;
	case 0x20000:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: Ethernet Controller\n", bus, slot, func, deviceType);
		break;
	case 0x30000:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: VGA-Compatible Controller\n", bus, slot, func, deviceType);
		break;
	case 0x40100:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: Multimedia Audio Controller\n", bus, slot, func, deviceType);
		break;
	case 0x40300:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: Audio Device\n", bus, slot, func, deviceType);
		break;
	case 0x60000:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: Host Bridge\n", bus, slot, func, deviceType);
		break;
	case 0x60100:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: ISA Bridge\n", bus, slot, func, deviceType);
		break;
	case 0x68000:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x: Other bridge device\n", bus, slot, func, deviceType);
		break;
	default:
		sprintf(tmp, "Bus: %i, slot: %i, fn: %i, class: 0x%x\n", bus, slot, func, deviceType);
		break;
	}
	Console::write(tmp);
}

uint32_t PCIManager::readPCIConfigLine(uint8_t bus, uint8_t slot, uint8_t func, uint8_t off) {
	uint32_t address = PCIManager::getAddress(bus, slot, func, off);

	/* write out the address */
	outl(0xCF8, address);
	/* read in the data */
	uint32_t tmp = inl(0xCFC);
	return tmp;
}

void PCIManager::writePCIConfigLine(uint8_t bus, uint8_t slot, uint8_t func, uint8_t off, uint32_t data) {
	uint32_t address = PCIManager::getAddress(bus, slot, func, off);

	/* write out the address */
	outl(0xCF8, address);
	/* write the data */
	outl(0xCFC, data);
}

uint32_t PCIManager::getAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t off) {
	uint32_t address;
	uint32_t lbus  = (uint32_t)(bus);
	uint32_t lslot = (uint32_t)(slot);
	uint32_t lfunc = (uint32_t)(func);
	uint32_t loff  = (uint32_t)(off & 0xFC);

	/* create configuration address as per Figure 1 */
	address = ((uint32_t)0x80000000) | (lbus << 16) |
			(lslot << 11) | (lfunc << 8) | (loff);

	return address;
}

void PCIManager::readPCIConfigSpace(uint8_t bus, uint8_t slot, uint8_t func, PCIConfigSpace& configSpace) {
	configSpace.PCIConfigSpace0  = PCIManager::readPCIConfigLine(bus, slot, func, 0x00);
	configSpace.PCIConfigSpace1  = PCIManager::readPCIConfigLine(bus, slot, func, 0x04);
	configSpace.PCIConfigSpace2  = PCIManager::readPCIConfigLine(bus, slot, func, 0x08);
	configSpace.PCIConfigSpace3  = PCIManager::readPCIConfigLine(bus, slot, func, 0x0C);
	configSpace.PCIConfigSpace4  = PCIManager::readPCIConfigLine(bus, slot, func, 0x10);
	configSpace.PCIConfigSpace5  = PCIManager::readPCIConfigLine(bus, slot, func, 0x14);
	configSpace.PCIConfigSpace6  = PCIManager::readPCIConfigLine(bus, slot, func, 0x18);
	configSpace.PCIConfigSpace7  = PCIManager::readPCIConfigLine(bus, slot, func, 0x1C);
	configSpace.PCIConfigSpace8  = PCIManager::readPCIConfigLine(bus, slot, func, 0x20);
	configSpace.PCIConfigSpace9  = PCIManager::readPCIConfigLine(bus, slot, func, 0x24);
	configSpace.PCIConfigSpace10 = PCIManager::readPCIConfigLine(bus, slot, func, 0x28);
	configSpace.PCIConfigSpace11 = PCIManager::readPCIConfigLine(bus, slot, func, 0x2C);
	configSpace.PCIConfigSpace12 = PCIManager::readPCIConfigLine(bus, slot, func, 0x30);
	configSpace.PCIConfigSpace13 = PCIManager::readPCIConfigLine(bus, slot, func, 0x34);
	configSpace.PCIConfigSpace14 = PCIManager::readPCIConfigLine(bus, slot, func, 0x38);
	configSpace.PCIConfigSpace15 = PCIManager::readPCIConfigLine(bus, slot, func, 0x3C);
}


void PCIManager::checkDevice(uint8_t bus, uint8_t slot) {
	uint32_t deviceID;
	deviceID = readPCIConfigLine(bus,slot,0,0) & 0xFFFF;
	if (deviceID == 0xFFFF) {
		return;  // No device
	}
	PCIDevice* dev = new PCIDevice(bus, slot, 0);
	findDrivers(dev);
	printDeviceInfo(dev);
	for (uint8_t function = 1; function < 8; function++) {
		deviceID = readPCIConfigLine(bus,slot,function,0) & 0xFFFF;
		if ((deviceID & 0xFFFF) == 0xFFFF) {
			continue;  // No device
		}
		PCIDevice* dev = new PCIDevice(bus, slot, function);
		printDeviceInfo(dev);
		findDrivers(dev);
	}
}

int PCIManager::matchDeviceDriver(PCIDevice* device, PCIDriver* driver) {
	const std::vector<PCIDeviceID> deviceIDs = driver->getPCIDeviceID();
	for (auto deviceID : deviceIDs) {
		if ((deviceID.vendorID != (uint32_t)PCI_ANY_ID) &&
					(deviceID.vendorID != device->getVendorID())) {
				continue;
			}
			if ((deviceID.deviceID != (uint32_t)PCI_ANY_ID) &&
					(deviceID.deviceID != device->getDeviceID())) {
				return -1;
			}
			if ((deviceID.subVendorID != (uint32_t)PCI_ANY_ID) &&
					(deviceID.subVendorID != device->getSubVendorID())) {
				return -1;
			}
			if ((deviceID.subDeviceID != (uint32_t)PCI_ANY_ID) &&
					(deviceID.subDeviceID != device->getSubDeviceID())) {
				return -1;
			}
			if ((deviceID.classCode != (uint32_t)PCI_ANY_ID) &&
					(deviceID.classCode != (device->getClassCode() & deviceID.classCodeMask))) {
				return -1;
			}
			return driver->probe(device);
	}
	return -1;

}

void PCIManager::findDrivers(PCIDevice* device) {
	_devices.push_back(device);
	for (auto driver : _drivers) {
		if (matchDeviceDriver(device, driver) != 0) {
			continue;
		} else {
			return;
		}
	}
	_unkonwDevices.push_back(device);
}
