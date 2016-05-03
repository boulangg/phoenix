/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Console.hpp"

#include "cpu.h"

using namespace std;

/* Public members */
std::size_t Console::row = 0;
std::size_t Console::column = 0;
std::uint8_t Console::color = 0;
std::uint16_t* Console::screen = (uint16_t*) 0xB8000;
bool Console::cursor_enabled = true;

void Console::initConsole() {
	setColor(VGA::Color::WHITE, VGA::Color::BLACK);
	clear();
}

void Console::setColor(VGA::Color fg, VGA::Color bg)
{
	color = makeColor(fg, bg);
}

void Console::clear()
{
	row = 0;
	column = 0;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y*VGA_WIDTH + x;
			screen[index] = makeVgaEntry(' ', color);
		}
	}
	updateCursor();
}

void Console::write(char c)
{
	processChar(c);
	updateCursor();
}

void Console::write(const char *str)
{
	size_t index = 0;
	while (str[index] != '\0') {
		write(str[index]);
		index++;
	}
	updateCursor();
}

void Console::toggleCursor(bool enabled)
{
	cursor_enabled = enabled;
	outb(0x3D4, 0x0A);
	uint8_t cursor_start_reg = inb(0x3D5);
	cursor_start_reg ^= !enabled << 4;
	outb(0x3D4, 0x0A);
	outb(0x3D5, cursor_start_reg);
}


/* Private members */
void Console::scrollUp()
{
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index_src = y*VGA_HEIGHT + x;
			const size_t index_dest = (y-1)*VGA_HEIGHT + x;
			screen[index_dest] = screen[index_src];
		}
	}
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		putEntryAt(' ', x, 0);
	}
	if (row != 0) {
		row --;
	}
}

void Console::putEntryAt(char c, size_t x, size_t y)
{
	const size_t index = y*VGA_WIDTH + x;
	screen[index] = makeVgaEntry(c, color);
}

void Console::updateCursor()
{
	if (cursor_enabled) {
		const size_t index = row*VGA_WIDTH + column;

		outb(0x3D4, 0x0F);
		outb(0x3D5, (uint8_t) (index & 0xFF));
		outb(0x3D4, 0x0E);
		outb(0x3D5, (uint8_t) ((index >> 8) & 0xFF));
	}
}

void Console::processChar(char c)
{
	if (c == '\b') {
		if (column > 0) {
			column --;
		}
	} else if (c == '\t') {
		column  = (column & 0xF8) + 0x8;
	} else if (c == '\n') {
		column = 0;
		row ++;
	} else if (c == '\f') {
		clear();
	} else if (c == '\r') {
		column = 0;
	} else if (c >= 32 && c<=126) {
		putEntryAt(c, column, row);
		column ++;
	}

	if (column >= VGA_WIDTH) {
		row ++;
		column = 0;
	}
	if (row >= VGA_HEIGHT) {
		scrollUp();
	}
}

/* Private static const */
const size_t Console::VGA_WIDTH = 80;
const size_t Console::VGA_HEIGHT = 25;

/* Private static functions */
uint16_t Console::makeVgaEntry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

uint8_t Console::makeColor(VGA::Color fg, VGA::Color bg)
{
	return static_cast<uint8_t>(fg) | static_cast<uint8_t>(bg) << 4;
}




