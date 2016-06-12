#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

extern "C" void KEYBOARD_handler();

#include <cstdint>

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
		UNICODE,
	};

	struct keyboardCommand {
		uint8_t command;
		uint8_t dataSize;
		uint8_t data[8];
	};

	static void initKeyboard();

	static void processScancode(uint8_t scancode);

	static void sendCommand(struct keyboardCommand cmd);
private:
	static KEYBOARD_MODE mode;
	static bool keystate[256];
};

#endif // __KEYBOARD_H__
