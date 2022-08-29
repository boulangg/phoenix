#pragma once

#include <cstdint>

// Source ATA 3, ATA 8 and SATA 3 specifications
struct __attribute__((packed)) IDEIdentifyData
{
// Word 0 - 63
	std::uint16_t generalConfiguration;
	std::uint16_t numLogicalCylinders;
	std::uint16_t specificConfiguration;
	std::uint16_t numLogicalHeads;
	std::uint16_t reserved_4_5[2];
	std::uint16_t numLogicalSectorsPerLogicalTrack;
	std::uint16_t reserved_7_9[3];
	char serialNumber[20];
	std::uint16_t reserved_20_21[2];
	std::uint16_t numberOfVendorSpecificBytesRWLonCMD;
	char firmwareVersion[8];
	char modelNumber[40];
	std::uint16_t multipleCount;
	std::uint16_t reserved_48;
	std::uint32_t capabilities;
	std::uint16_t reserved_51;
	std::uint16_t PIODataTransferCycleTimingMode;
	std::uint16_t fieldValidity;
	std::uint16_t numCurrLogicalCylinders;
	std::uint16_t numCurrLogicalHeads;
	std::uint16_t numCurrLogicalSectorsPerTrack;
	std::uint32_t currCapacityInSectors;
	std::uint16_t multiSectorSetting;
	std::uint32_t numAdressableSector;
	std::uint16_t DMAModeSupport;
	std::uint16_t multiwordDMATranfer;

	// Word 64-79
	std::uint16_t PIOModeTranfer;
	std::uint16_t minMultiwordDMASpeed; // in cycle time per word
	std::uint16_t recommendedMultiwordDMASpeed;
	std::uint16_t minPIOSpeedNoIORDY;
	std::uint16_t minPIOSpeedIORDY;
	std::uint16_t reserved_69_74[6];
	std::uint16_t queueDepth;
	std::uint16_t SATACapabilities;
	std::uint16_t SATAAdditionalCapabilities;
	std::uint16_t SATAFeaturesSupported;
	std::uint16_t SATAFeaturesEnabled;

	// Word 80 - 127
	std::uint16_t majorVersion;
	std::uint16_t minorVersion;
	std::uint16_t commandSetSupportedEnabled[6];
	std::uint16_t ultraDMAMode;
	std::uint16_t securityEraseUnitTime;
	std::uint16_t enhancedSecurityEraseUnitTime;
	std::uint16_t reserved_91;
	std::uint16_t masterPasswordRevisionCode;
	std::uint16_t hardwareResetResults;
	std::uint16_t automaticAcousticManagementValue;

	std::uint16_t streamMinimumRequestSize;
	std::uint16_t streamingTranferTimeDMA;
	std::uint16_t streamingTranferTimeDMAPIO;
	std::uint16_t streamingPerfGranularity[2];
	std::uint64_t max48LBA;
	std::uint16_t streamingTransferTimePIO;
	std::uint16_t reserved_105;
	std::uint16_t physicalSectorSizePerLogicalSectorSize;
	std::uint16_t interSeekDelay;

	std::uint16_t worldWideName[4];
	std::uint16_t worldWideNameExt[4];

	std::uint16_t reserved_116;
	std::uint32_t logicalSectorSize;
	std::uint16_t supportedSettings;
	std::uint16_t commandSetSupportedEnabled_2;
	std::uint16_t reserved_121_124[4];
	std::uint16_t ATAPIByteCountZero;
	std::uint16_t reserved_126;
	std::uint16_t removableMediaNotificationSupport; // obsolete in version 8

	// Word 128 - 159
	std::uint16_t securityStatus;
	std::uint16_t vendorSpecific_129_159[31];

	// Word 160 - 255
	std::uint16_t CFAPowerMode1;
	std::uint16_t reservedForCompactFlashAssociation[15];

	std::uint16_t mediaSerialNumber[30];
	std::uint16_t SCTCommandTransport;
	std::uint16_t reserved207_208[2];
	std::uint16_t logicalBlockAlignment;
	std::uint32_t WRVSectorCountMode3Only;
	std::uint32_t VSectorCountMode2Only;
	std::uint16_t NVCacheCapabilities;
	std::uint16_t NVCacheSizeInLogicalBlock_1;
	std::uint16_t NVCacheSizeInLogicalBlock_2;
	std::uint16_t NominalMediaRotationRate;
	std::uint16_t reserved_218;
	std::uint16_t NVCacheOptions;
	std::uint16_t WRVCurrMode;
	std::uint16_t reserved_221;

	std::uint16_t transportMajorRevisionnumber;
	std::uint16_t transportMinorRevisionnumber;

	std::uint16_t reserved_224_233[10];
	std::uint16_t min512ByteUnitforDWNMicroCode;
	std::uint16_t max512ByteUnitforDWNMicroCode;
	std::uint16_t reserved_236_254[19];
	std::uint16_t integrity;
};

static_assert (sizeof(IDEIdentifyData) == 512, "Size of IDEIdentifyData struct is not correct");

//static_assert (offsetof(logicalSectorSize) == 117, "incorrect offset for logicalSectorSize");
