/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Console.hpp"

#include <asm/io.h>
#include <include/constant.h>
#include <proc/ProcessScheduler.hpp>
#include <new>
#include <type_traits>
#include <stdio.h>


using namespace std;

/* Public members */
size_t Console::row = 0;
size_t Console::column = 0;
uint8_t Console::color = 0;
uint16_t* Console::screen = (uint16_t*)(0xB8000 | KERNEL_MAPPING_START);
bool Console::cursor_enabled = true;
char Console::kbdBuf[KBD_BUF_SIZE] = {};
std::size_t Console::kbdBufStart = 0;
std::size_t Console::kbdBufSize = 0;

static unsigned char stream_buf[sizeof(Console)] alignas(alignof(Console));
//static typename std::<sizeof(Console), alignof (Console)>::type
//stream_buf;

Console& cout = reinterpret_cast<Console&> (stream_buf);

void Console::initConsole()
{
	new (&cout) Console(); //not usefull since we only use static methods, but just in case
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
			const size_t index = y * VGA_WIDTH + x;
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

void Console::write(const char* str)
{
	size_t index = 0;
	while (str[index] != '\0') {
		write(str[index]);
		index++;
	}
	updateCursor();
}

void Console::write(const std::string& str)
{
	Console::write(str.c_str());
}

void Console::write(std::string& str)
{
	Console::write(str.c_str());
}

void Console::write(std::string&& str)
{
	Console::write(str.c_str());
}

size_t Console::read(char* buf, size_t count)
{
	size_t curr = 0;
	if (kbdBufSize == 0) {
		Event ev(Event::EventType::FileEvent, 0);
		ProcessScheduler::wait(ev);
	}
	while ((kbdBufSize > 0) && (curr < count)) {
		buf[curr] = kbdBuf[kbdBufStart];
		kbdBufStart = (kbdBufStart + 1) % KBD_BUF_SIZE;
		curr++;
	}
	return curr;
}

void Console::keyboardInput(char character)
{
	if ((character == '\b') && (kbdBufSize > 0)) {
		kbdBufSize--;
	}
	if ((character != '\b') && (kbdBufSize < 1024)) {
		kbdBuf[(kbdBufStart + kbdBufSize) % KBD_BUF_SIZE] = character;
		kbdBufSize++;
	}
	Console::write(character);
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
			const size_t index_src = y * VGA_WIDTH + x;
			const size_t index_dest = (y - 1) * VGA_WIDTH + x;
			screen[index_dest] = screen[index_src];
		}
	}
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		putEntryAt(' ', x, VGA_HEIGHT - 1);
	}
	if (row != 0) {
		row--;
	}
}

void Console::putEntryAt(char c, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	screen[index] = makeVgaEntry(c, color);
}

void Console::updateCursor()
{
	if (cursor_enabled) {
		const size_t index = row * VGA_WIDTH + column;

		outb(0x3D4, 0x0F);
		outb(0x3D5, (uint8_t)(index & 0xFF));
		outb(0x3D4, 0x0E);
		outb(0x3D5, (uint8_t)((index >> 8) & 0xFF));
	}
}

void Console::processChar(char c)
{
	if (c == '\b') {
		if (column > 0) {
			column--;
			putEntryAt(' ', column, row);
		}
	} else if (c == '\t') {
		column = (column & 0xF8) + 0x8;
	} else if (c == '\n') {
		column = 0;
		row++;
	} else if (c == '\f') {
		clear();
	} else if (c == '\r') {
		column = 0;
	} else if (c >= 32 && c <= 126) {
		putEntryAt(c, column, row);
		column++;
	}

	if (column >= VGA_WIDTH) {
		row++;
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

Console& Console::operator<<(const char* str)
{
	Console::write(str);
	return *this;
}

Console& Console::operator<<(char c)
{
	Console::write(c);
	return *this;
}

Console& Console::operator<<(int i)
{
	char tmp[1024];
	sprintf(tmp, "%i", i);
	Console::write(tmp);
	return *this;
}

Console& Console::operator<<(long i)
{
	char tmp[1024];
	sprintf(tmp, "%li", i);
	Console::write(tmp);
	return *this;
}

Console& Console::operator<<(uint64_t i)
{
	char tmp[1024];
	sprintf(tmp, "%lu", i);
	Console::write(tmp);
	return *this;
}

Console& Console::operator<<(uint32_t i)
{
	char tmp[1024];
	sprintf(tmp, "%u", i);
	Console::write(tmp);
	return *this;
}
