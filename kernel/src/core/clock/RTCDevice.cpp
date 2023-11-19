/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT
 * license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "RTCDevice.h"

#include "Kernel.h"
#include "asm/cpu.h"

namespace kernel::core::clock {

static constexpr std::uint32_t CURRENT_YEAR = 2023;

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
    outb(CMOS_ADDR, reg);
    return inb(CMOS_DATA);
}
static void setCRTReg(std::uint16_t reg, std::uint8_t val)
{
    outb(CMOS_ADDR, reg);
    outb(CMOS_DATA, val);
}

RTCDevice* RTCDevice::initRTC()
{
    RTCDevice* device = new RTCDevice();

    device->_handler =
        new InterruptHandlerClass<RTCDevice, &RTCDevice::IRQHandler>("RTC", {true, false}, nullptr, device);

    Kernel::interrupt.registerHandler(8, device->_handler);

    // Enable IRQ8 for RTC at update-ended (bit 4 of register B)
    cli();
    std::uint8_t prev = getCRTReg(CMOS_REG_B | CMOS_DISABLE_NMI_FLAG); // Read register B and turn off NMI
    setCRTReg(CMOS_REG_B | CMOS_DISABLE_NMI_FLAG, prev | 0x10);        // Set bit 4 of register B
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
    unsigned char second = getCRTReg(CMOS_REG_SEC);
    unsigned char minute = getCRTReg(CMOS_REG_MIN);
    unsigned char hour = getCRTReg(CMOS_REG_HOUR);
    unsigned char day = getCRTReg(CMOS_REG_DAY);
    unsigned char month = getCRTReg(CMOS_REG_MONTH);
    unsigned char year = getCRTReg(CMOS_REG_YEAR);

    std::uint8_t status = getCRTReg(0x0B);

    // Convert BCD to binary values if necessary
    if (!(status & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
    }

    // Convert 12 hour clock to 24 hour clock if necessary
    if (!(status & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }

    // Get the number of days between 2000 and beginning of this year (coutning leap days).
    unsigned fullyeardays = year * 365 + ((year - 1) / 4) + 1;

    // Get the number of (full) days for this year, using the lookup table for months.
    static constexpr unsigned daysbeforemonth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    unsigned thisyeardays = daysbeforemonth[month - 1] + day - (((year % 4) == 0 && month > 2) ? 0 : 1);

    // Add days between EPOCH (1970-01-01 00:00) and 2000
    static constexpr unsigned DAYS_BETWEEN_1970_AND_2000 = 10957;
    unsigned date = fullyeardays + thisyeardays + DAYS_BETWEEN_1970_AND_2000;

    // Compute current system time
    unsigned time = second + 60 * (minute + 60 * (hour + 24 * date));
    BaseTime baseTime = {.time = time, .tsc = rdtsc()};
    Kernel::clock->setCurrentTime(baseTime);
}

}
