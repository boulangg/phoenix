
#pragma once

#include <cstdint>
#include <list>

class PCIDriver;

#define PCI_ANY_ID (uint32_t)(~(uint32_t)0)

struct PCIDeviceID
{
	uint32_t vendorID;
	uint32_t deviceID;
	uint32_t subVendorID;
	uint32_t subDeviceID;
	uint32_t classCode;
	uint32_t classCodeMask;
};

struct PCIConfigSpace
{
	union
	{
		uint32_t PCIConfigSpace0;
		struct
		{
			uint16_t deviceID;
			uint16_t vendorID;
		};
	};
	union
	{
		uint32_t PCIConfigSpace1;
		struct
		{
			uint16_t command;
			uint16_t status;
		};
	};
	union
	{
		uint32_t PCIConfigSpace2;
		struct
		{
			uint8_t revisionID;
			uint8_t progIF;
			uint8_t subclassCode;
			uint8_t classCode;
		};
	};
	union
	{
		uint32_t PCIConfigSpace3;
		struct
		{
			uint8_t BIST;
			uint8_t headerType;
			uint8_t latencyTimer;
			uint8_t cacheLineSize;
		};
	};
	union
	{
		uint32_t PCIConfigSpace4;
		uint32_t BAR0;
	};
	union
	{
		uint32_t PCIConfigSpace5;
		uint32_t BAR1;
	};
	union
	{
		uint32_t PCIConfigSpace6;
		uint32_t BAR2;
		struct
		{
			uint8_t secondaryLatencyTimer;
			uint8_t subordinateBusNumber;
			uint8_t secondaryBusNumber;
			uint8_t primaryBusNumber;
		};
	};
	union
	{
		uint32_t PCIConfigSpace7;
		uint32_t BAR3;
		struct
		{
			uint16_t secondaryStatus;
			uint8_t IOLimit;
			uint8_t IOBase;
		};
	};
	union
	{
		uint32_t PCIConfigSpace8;
		uint32_t BAR4;
		struct
		{
			uint16_t memoryLimit;
			uint16_t memoryBase;
		};
	};
	union
	{
		uint32_t PCIConfigSpace9;
		uint32_t BAR5;
	};
	union
	{
		uint32_t PCIConfigSpace10;
		uint32_t cardBusCISpointer;
	};
	union
	{
		uint32_t PCIConfigSpace11;
		struct
		{
			uint16_t subDeviceID;
			uint16_t subVendorID;
		};
	};
	union
	{
		uint32_t PCIConfigSpace12;
		uint32_t expansionROMBaseAdress;
	};
	union
	{
		uint32_t PCIConfigSpace13;
		union
		{
			char reserved_0[3];
			uint8_t capabilitiesPointer;
		};
	};
	union
	{
		uint32_t PCIConfigSpace14;
		char reserved_1[4];
	};
	union
	{
		uint32_t PCIConfigSpace15;
		struct
		{
			uint8_t maxLatency;
			uint8_t minGrant;
			uint8_t interruptPIN;
			uint8_t interruptLine;
		};
	};
};

struct PCIDevice
{

	PCIDevice(uint8_t bus, uint8_t slot, uint8_t function);

	uint8_t bus;
	uint8_t slot;
	uint8_t function;
	PCIConfigSpace configSpace;

	inline uint16_t getVendorID() const
	{
		return configSpace.vendorID;
	}

	inline uint16_t getDeviceID() const
	{
		return configSpace.deviceID;
	}

	inline uint16_t getSubVendorID() const
	{
		return configSpace.subVendorID;
	}

	inline uint16_t getSubDeviceID() const
	{
		return configSpace.subDeviceID;
	}

	inline uint16_t getClassCode() const
	{
		return configSpace.classCode;
	}

	inline uint32_t getDeviceType() const
	{
		return ((uint32_t)configSpace.classCode << 24) |
			((uint32_t)configSpace.subclassCode << 16) |
			((uint32_t)configSpace.progIF << 8) |
			((uint32_t)configSpace.revisionID);
	}

	inline uint16_t getBAR(uint8_t index)
	{
		switch (index) {
		case 0:
			return configSpace.BAR0;
		case 1:
			return configSpace.BAR1;
		case 2:
			return configSpace.BAR2;
		case 3:
			return configSpace.BAR3;
		case 4:
			return configSpace.BAR4;
		case 5:
			return configSpace.BAR5;
		default:
			return 0x00;
		}
	}

	uint16_t readPCIConfigLine(uint8_t offset);
	void writePCIConfigLine(uint8_t offset, uint32_t data);
	bool getMaster();
	void setMaster(bool enable);
};

class PCIManager
{
public:

	static void initPCI();

	// Register driver
	static void registerDriver(PCIDriver* driver);

	// Unregister driver
	static void unregisterDriver(PCIDriver* driver);

	static void printDeviceInfo(PCIDevice* dev);

	static void readPCIConfigSpace(uint8_t bus, uint8_t slot, uint8_t func, PCIConfigSpace& configSpace);

	static uint32_t readPCIConfigLine(uint8_t bus, uint8_t slot, uint8_t func, uint8_t off);
	static void writePCIConfigLine(uint8_t bus, uint8_t slot, uint8_t func, uint8_t off, uint32_t data);
private:

	static void checkDevice(uint8_t bus, uint8_t slot);

	static int matchDeviceDriver(PCIDevice* device, PCIDriver* driver);

	static void findDrivers(PCIDevice* device);
	static uint32_t getAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t off);

	static std::list<PCIDevice*> _devices;
	static std::list<PCIDevice*> _unkonwDevices;
	static std::list<PCIDriver*> _drivers;
};
