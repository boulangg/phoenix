/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "KeyboardDevice.h"

#include "Kernel.h"
#include "core/InterruptDispatcher.h"

// Tests
#include "asm/cpu.h"

namespace kernel::dev::input {

void KeyboardDevice::processScancode()
{
    std::uint8_t scancode = inb(0x60);
    if (_previousScancode == char(0xE0)) {
        _previousScancode = 0;
        bool keyPressed = !(scancode & 0x80);
        scancode |= 0x80;
        switch (scancode & 0x7F) {
        case 0x1C:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_KPENTER, keyPressed);
            break;
        case 0x1D:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_RIGHTCTRL, keyPressed);
            break;
        case 0x35:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_KPSLASH, keyPressed);
            break;
        case 0x38:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_RIGHTALT, keyPressed);
            break;
        case 0x47:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_HOME, keyPressed);
            break;
        case 0x48:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_UP, keyPressed);
            break;
        case 0x4B:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_LEFT, keyPressed);
            break;
        case 0x49:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_PAGEUP, keyPressed);
            break;
        case 0x4D:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_RIGHT, keyPressed);
            break;
        case 0x4F:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_END, keyPressed);
            break;
        case 0x50:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_DOWN, keyPressed);
            break;
        case 0x51:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_PAGEDOWN, keyPressed);
            break;
        case 0x52:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_INSERT, keyPressed);
            break;
        case 0x53:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_DELETE, keyPressed);
            break;
        case 0x5D:
            reportMsc(MSC_SCAN, scancode);
            reportKey(KEY_COMPOSE, keyPressed);
            break;
        default:
            // Not supported
            reportMsc(MSC_SCAN, scancode);
            reportKey(scancode, 3);
            sync();
            return;
        }
    } else if (scancode == 0xE0) {
        // Multi-code scancode
        _previousScancode = scancode;
        return;
    } else if ((scancode & 0x7F) <= 88) {
        // Direct scancode
        bool keyPressed = !(scancode & 0x80);
        scancode = scancode & 0x7F;
        reportMsc(MSC_SCAN, scancode);
        reportKey(scancode, keyPressed);
    } else {
        // Not supported
        reportMsc(MSC_SCAN, scancode);
        reportKey(scancode, 4);
        sync();
        return;
    }

    sync();
}


void KeyboardDevice::initKeyboard()
{
    KeyboardDevice* device = new KeyboardDevice();
    core::InterruptHandler* handler = new core::InterruptHandlerClass<KeyboardDevice, &KeyboardDevice::IRQHandler>(
        "Keyboard", {true, false}, nullptr, device);
    Kernel::interrupt.registerHandler(1, handler);
    InputManager::registerDevice(device);
}

}
