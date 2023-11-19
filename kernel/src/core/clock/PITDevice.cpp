/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "PITDevice.h"

#include "Kernel.h"
#include "asm/cpu.h"

#include "Clock.h"

// Test
#include "KernelGlobals.h"

namespace kernel::core::clock {

enum PITAccess
{
    Latch = 0b00,
    lobyte = 0b01,
    hibyte = 0b10,
    lohibyte = 0b11,
};

static constexpr std::uint8_t PIT_CHANNEL_BASE = 0x40;
static constexpr std::uint8_t PIT_COMMAND = 0x43;

static constexpr std::uint32_t BASE_FREQUENCY = 0x1234DD;

PITDevice* PITDevice::initPIT()
{
    PITDevice* device = new PITDevice();

    device->changeConfig(CHANNEL_0, MODE_2, 0);
    device->_handler =
        new InterruptHandlerClass<PITDevice, &PITDevice::measureCpuSpeed>("PIT", {true, false}, nullptr, device);
    Kernel::interrupt.registerHandler(0, device->_handler);

    return device;
}

void PITDevice::changeConfig(PITChannel channel, PITMode mode, std::uint16_t divider)
{
    std::uint8_t command = (channel << 6) | (lohibyte << 4) | (mode << 1);
    outb(PIT_COMMAND, command);
    outb(PIT_CHANNEL_BASE + channel, divider & 0xFF);
    outb(PIT_CHANNEL_BASE + channel, divider >> 8);
}

void PITDevice::switchHandler()
{
    sti();
    Kernel::interrupt.unregisterHandler(0, _handler);
    delete _handler;

    changeConfig(CHANNEL_0, MODE_2, 1 << 14); // 72 Hz
    _handler = new InterruptHandlerClass<PITDevice, &PITDevice::schedule>("PIT", {true, false}, nullptr, this);
    Kernel::interrupt.registerHandler(0, _handler);
    cli();
}

int PITDevice::measureCpuSpeed(std::uint8_t, void*)
{
    static constexpr std::size_t PRECISION = 4;
    static std::uint64_t TSC[PRECISION] = {};
    static std::size_t i = 0;

    if (i >= PRECISION) {
        return 0;
    }

    TSC[i] = rdtsc();
    ++i;
    if (i < PRECISION) {
        return 0;
    }

    constexpr std::uint64_t frame_nsec = NSEC_PER_SEC * (PRECISION - 1) * 0x10000 / BASE_FREQUENCY;
    std::uint64_t cpu_speed = (TSC[PRECISION - 1] - TSC[0]) * BASE_FREQUENCY / ((PRECISION - 1) * 0x10000);
    printk("[0.%0.6us] Estimated frequency: %u.%0.6u GHz\n", frame_nsec / 1000, cpu_speed / 1000000000,
           cpu_speed / 1000);
    Kernel::clock->setCpuSpeed(cpu_speed);
    return 0;
}

int PITDevice::schedule(std::uint8_t, void*)
{
    return 0;
}

}
