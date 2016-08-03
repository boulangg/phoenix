#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

extern "C" void KEYBOARD_handler();

#include <cstdint>
#include <fs/TTY.hpp>

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

#endif // __KEYBOARD_H__
