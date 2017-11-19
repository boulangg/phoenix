
#include <cstdint>

#include "IDEDrive.hpp"

struct IDEIdentifyData {
	std::uint16_t generalConfiguration;
	std::uint16_t gap_0;
	std::uint16_t specificConfiguration;
	std::uint16_t gap_1[7];
	char serialNumber[20];
	std::uint16_t gap_2[3];
	char firmwareVersion[8];
	char modelNumber[40];
	std::uint16_t multiReadWriteSupport;
	std::uint16_t gap_3;
	std::uint32_t capabilities;
	std::uint16_t gap_4[2];
	std::uint16_t fieldValidity;
	std::uint16_t gap_5[5];
	std::uint16_t multiSectorSetting;
	std::uint32_t adressableSectorNumber;
	std::uint16_t gap_6;
	std::uint16_t multiwordDMATranfer;
	std::uint16_t PIOModeTranfer;
	std::uint16_t minMultiwordDMASpeed; // in cycle time per word
	std::uint16_t recommendedMultiwordDMASpeed;
	std::uint16_t minPIOSpeedNoIORDY;
	std::uint16_t minPIOSpeedIORDY;
	std::uint16_t gap_7[6];
	std::uint16_t queueDepth;
	std::uint16_t gap_8[4];
	std::uint16_t majorVersion;
	std::uint16_t minorVersion;
	std::uint16_t commandSetSupported[3];
	std::uint16_t commandSetEnabled[3];
	std::uint16_t ultraDMAMode;
	std::uint16_t securityEraseUnitTime;
	std::uint16_t enhancedSecurityEraseUnitTime;
	std::uint16_t advancedPowerManagementLevel;
	std::uint16_t masterPasswordRevisionCode;
	std::uint16_t hardwareConfigurationTestResults;
	std::uint16_t automaticAcousticManagementValue;
	std::uint16_t streamMinimumRequestSize;
	std::uint16_t streamingTranferTimeDMA;
	std::uint16_t streamingTranferTimeDMAPIO;
	std::uint16_t streamingPerfGranularity[2];
	std::uint64_t max48LBA;
	std::uint16_t stremingTransferTimePIO;
	std::uint16_t gap_9;
	std::uint16_t physicalSectorSize;
	std::uint16_t interSeekDelay;
	std::uint16_t worldWideName[4];
	std::uint16_t worldWideNameExt[4];
	std::uint16_t gap_10;
	std::uint32_t logicalSectorSize;
	std::uint16_t removableMediaNotificationSupport;
	std::uint16_t securityStatus;
	std::uint16_t vendorSpceific[31];
	std::uint64_t CFAPowerMode;
	std::uint16_t gap_11[15];
	std::uint16_t mediaSerialNumber[30];
	std::uint16_t gap_12[49];
	std::uint16_t integrity;
};
static_assert (sizeof(IDEIdentifyData) == 512, "Size is not correct");


class IDEDevice : public BlockDevice {
public:

private:

	std::uint8_t readReg(std::uint8_t reg);
	std::uint8_t writeReg(std::uint8_t reg, std::uint8_t data);

	IDEDrive _drives[4];
};
