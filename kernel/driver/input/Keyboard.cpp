#include "Keyboard.hpp"
#include <asm/io.h>
#include <core/Console.hpp>
#include <stdio.h>

void KEYBOARD_handler() {
	outb(0x20, 0x20);
    int scancode = inb(0x60);
	Keyboard::processScancode(scancode);
}


/*struct keybind {
	char map[256];
};

static struct keybind default_keybind {
	{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19}
};
static struct keybind extend_keybind;

struct keymap {

};*/

Keyboard::KEYBOARD_MODE Keyboard::mode = Keyboard::KEYBOARD_MODE::RAW;
bool Keyboard::keystate[256];

void Keyboard::processScancode(uint8_t scancode) {
	if (mode == RAW) {
		Console::keyboardInput((char)scancode);
	} else if (mode == UNICODE) {
		/*if (scancode != 0xe0) {
			uint8_t keycode = default_keybind[s
		}*/
	}
    char str[16];
    sprintf(str, "%x, ", scancode);
	Console::write(str);
}
