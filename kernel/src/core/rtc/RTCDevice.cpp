/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "RTCDevice.h"


#include "core/CpuInstr.h"
#include "core/InterruptDispatcher.h"
#include "Kernel.h"

namespace kernel::core::rtc {

static constexpr std::uint16_t CMOS_ADDR = 0x70;
static constexpr std::uint16_t CMOS_DATA = 0x71;

static constexpr std::uint16_t CMOS_REG_SEC = 0x00;
static constexpr std::uint16_t CMOS_REG_MIN = 0x02;
static constexpr std::uint16_t CMOS_REG_HOUR = 0x04;
static constexpr std::uint16_t CMOS_REG_DAY = 0x07;
static constexpr std::uint16_t CMOS_REG_MONTH = 0x08;
static constexpr std::uint16_t CMOS_REG_YEAR = 0x09;
static constexpr std::uint16_t CMOS_REG_A = 0x0A;
static constexpr std::uint16_t CMOS_REG_B = 0x0B;
static constexpr std::uint16_t CMOS_REG_C = 0x0C;
static constexpr std::uint16_t CMOS_REG_D = 0x0D;
static constexpr std::uint16_t CMOS_DISABLE_NMI_FLAG = 0x80;

static std::uint8_t getCRTReg(std::uint16_t reg)
{
    core::outb(CMOS_ADDR, reg);
    return core::inb(CMOS_DATA);
}
static void setCRTReg(std::uint16_t reg, std::uint8_t val)
{
    core::outb(CMOS_ADDR, reg);
    core::outb(CMOS_DATA, val);
}

RTCDevice* RTCDevice::initRTC()
{
    RTCDevice* device = new RTCDevice();

    core::InterruptHandler* handler =
        new core::InterruptHandlerClass<RTCDevice, &RTCDevice::IRQHandler>(
        "RTC", {true, false}, nullptr, device);
    Kernel::interrupt.registerHandler(8, handler);

    // Enable IRQ8 for RTC at update-ended (bit 4 of register B)
    cli();
    std::uint8_t prev = getCRTReg(CMOS_REG_B | CMOS_DISABLE_NMI_FLAG); // Read register B and turn off NMI
    setCRTReg(CMOS_REG_B | CMOS_DISABLE_NMI_FLAG, prev | 0x10); // Set bit 4 of register B
    getCRTReg(CMOS_REG_C); // Reset RTC interrupt status (register C) and reenable NMI
    sti();

    return device;
}

int RTCDevice::IRQHandler(std::uint8_t, void*)
{
    getCRTReg(CMOS_REG_C); // Reset RTC interrupt status (register C)

    readDateAndTime();
    return 0;
}

void RTCDevice::readDateAndTime()
{
    DateTime dateTime
    {
        .second = getCRTReg(CMOS_REG_SEC),
        .minute = getCRTReg(CMOS_REG_MIN),
        .hour = getCRTReg(CMOS_REG_HOUR),
        .day = getCRTReg(CMOS_REG_DAY),
        .month = getCRTReg(CMOS_REG_MONTH),
        .year = getCRTReg(CMOS_REG_YEAR)
    };

    std::uint8_t status = getCRTReg(0x0B);

    // Convert BCD to binary values if necessary
    if (!(status & 0x04)) {
        dateTime.second = (dateTime.second & 0x0F) + ((dateTime.second / 16) * 10);
        dateTime.minute = (dateTime.minute & 0x0F) + ((dateTime.minute / 16) * 10);
        dateTime.hour = ((dateTime.hour & 0x0F) + (((dateTime.hour & 0x70) / 16) * 10)) | (dateTime.hour & 0x80);
        dateTime.day = (dateTime.day & 0x0F) + ((dateTime.day / 16) * 10);
        dateTime.month = (dateTime.month & 0x0F) + ((dateTime.month / 16) * 10);
        dateTime.year = (dateTime.year & 0x0F) + ((dateTime.year / 16) * 10);
    }

    // Convert 12 hour clock to 24 hour clock if necessary
    if (!(status & 0x02) && (dateTime.hour & 0x80)) {
        dateTime.hour = ((dateTime.hour & 0x7F) + 12) % 24;
    }

    // Calculate the full (4-digit) year
    dateTime.year += (CURRENT_YEAR / 100) * 100;
    if (dateTime.year < CURRENT_YEAR) {
        dateTime.year += 100;
    }

    _currentDateTime = dateTime;
}

}

