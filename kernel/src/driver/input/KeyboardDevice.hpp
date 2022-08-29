/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <asm/io.h>

#include "InputDevice.hpp"
#include "InputManager.hpp"

class KeyboardDevice : public InputDevice
{
public:

	static void initKeyboard();

	KeyboardDevice() : InputDevice("Keyboard")
	{}
	virtual ~KeyboardDevice()
	{}

	int IRQHandler()
	{
		uint8_t scancode = inb(0x60);
		processScancode(scancode);
		sync();
		return 0;
	}

	virtual int handleEvent(unsigned int, unsigned int, int) override
	{
		return 0;
	}

	/*const int scanCodeSet1_QWERTY[256] = {
		KEY_RESERVED, KEY_ESC, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_TAB,
		KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFTBRACE, KEY_RIGHTBRACE, KEY_ENTER, KEY_LEFTCTRL, KEY_A, KEY_S,
		KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE, KEY_GRAVE, KEY_LEFTSHIFT, KEY_BACKSLASH,
	};*/

protected:
	void processScancode(int scancode)
	{
// Scan code set 1 QWERTY hardcoded only for regular key (up to keycode 88) pressed & released
		if (scancode <= 88) {
			reportKey(scancode, 1);
		} else if (scancode >= 128 && scancode <= 216) {
			reportKey(scancode - 128, 0);
		}
		sync();
	}
};

/*
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

extern "C" void KEYBOARD_handler();

#include <cstdint>
#include <fs/TTY.hpp>


struct keybind {
	char map[256];
};

extern struct keybind normal_keybind;

class Keyboard {
public:

	enum keyboardCommandType {
		setLED = 0xED,
		echo = 0xEE,
		currentScanCode = 0xF0,
		identify = 0xF2,
		rateAndDelay = 0xF3,
		resend = 0xFE,
		resetAndSelfTest = 0xFF
	};

	enum KEYBOARD_MODE {
		RAW,
		ASCII,
		UNICODE,
	};

	struct keyboardCommand {
		uint8_t command;
		uint8_t dataSize;
		uint8_t data[8];
	};

	static void initKeyboard();

	static int handleInterrupt();

	static void processScancode(uint8_t scancode);
	static void processSpecialKeys(uint8_t scancode);
	static void processKeycode(uint8_t keycode);

	static void sendCommand(struct keyboardCommand cmd);

	static bool isShift();
	static void setTTY(TTY* tty) {
		Keyboard::tty = tty;
	}
private:
	static KEYBOARD_MODE mode;
	static uint8_t specialKeys;
	static bool keystate[256];
	static bool extended;
	static TTY* tty;
};

#endif // __KEYBOARD_H__*/
