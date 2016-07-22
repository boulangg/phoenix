#include "Keyboard.hpp"
#include <asm/io.h>
#include <core/Console.hpp>
#include <stdio.h>

void KEYBOARD_handler() {
	outb(0x20, 0x20);
    int scancode = inb(0x60);
	Keyboard::processScancode(scancode);
}


struct keybind {
	char map[256];
};
/*
static struct keybind default_keybind {
	{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19}
};
static struct keybind extend_keybind;*/

static struct keybind normal_keybind {
	'\0',
	0x1b,  '&',  'e',  '"',  '\'',  '(',  '-',  'e',  '_',  'c',  'a',  ')',  '=', '\b',
	0x09,  'a',  'z',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '^',  '$', '\n', 0x00,
	 'q',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  'm', 'u', 0x60, 0x00, '\\',
	 'w',  'x',  'c',  'v',  'b',  'n',  ',',  ';',  ':',  '!', 0x00,  '*', 0x00, 0x00,
};

static struct keybind shift_keybind {
	'\0',
	0x1b,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  'o',  '+', '\b',
	0x09,  'A',  'Z',  'E',  'R',  'T',  'Y',  'U',  'I',  'O',  'P',  '"',  0x00, 0x0d, 0x00,
	 'Q',  'S',  'D',  'F',  'G',  'H',  'J',  'K',  'L',  'M', '%', 0x60, 0x00, '\\',
	 'W',  'X',  'C',  'V',  'B',  'N',  '?',  '.',  '/',  0x00, 0x00,  0x00, 0x00, 0x00,
};

Keyboard::KEYBOARD_MODE Keyboard::mode = Keyboard::KEYBOARD_MODE::ASCII;
bool Keyboard::keystate[256];
bool Keyboard::extended = false;
uint8_t Keyboard::specialKeys = 0;

void Keyboard::processScancode(uint8_t scancode) {
	if (mode == RAW) {
		Console::keyboardInput((char)scancode);
	    char str[16];
	    sprintf(str, "%x, ", scancode);
		Console::write(str);
	} else if (mode == ASCII) {
		processSpecialKeys(scancode);
		if (scancode > 0xe0) {
			extended = true;
			return;
		}
		if (!extended) {
			if (scancode < 0x80) {
				processKeycode(scancode);
			}
		}
	} else if (mode == UNICODE) {
		/*if (scancode != 0xe0) {
			uint8_t keycode = default_keybind[s
		}*/
	}
}

#define SHIFT 0x1
#define CAPSLOCK 0x2

bool Keyboard::isShift() {
	return ((specialKeys & SHIFT) && !(specialKeys & CAPSLOCK)) ||
		   (!(specialKeys & SHIFT) && (specialKeys & CAPSLOCK));
}

void Keyboard::processSpecialKeys(uint8_t scancode) {
	(void)scancode;
	switch(scancode) {
	case 0x2a:
	case 0x36:
		specialKeys |= SHIFT;
		break;
	case 0xaa:
	case 0xb6:
		specialKeys &= ~SHIFT;
		break;
	case 0x3a:
		specialKeys ^= CAPSLOCK;
		break;

	}

}

void Keyboard::processKeycode(uint8_t keycode) {
	if(isShift()) {
		Console::keyboardInput(shift_keybind.map[keycode]);
	} else {
		Console::keyboardInput(normal_keybind.map[keycode]);
	}
}
