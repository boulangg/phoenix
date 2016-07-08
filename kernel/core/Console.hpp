/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef KERNEL_CONSOLE_HPP_
#define KERNEL_CONSOLE_HPP_

#include <cstddef>
#include <cstdint>
#include <string>

#define KBD_BUF_SIZE 1024

namespace VGA {
enum class Color {
	BLACK = 0,
			BLUE = 1,
			GREEN = 2,
			CYAN = 3,
			RED = 4,
			MAGENTA = 5,
			BROWN = 6,
			LIGHT_GREY = 7,
			DARK_GREY = 8,
			LIGHT_BLUE = 9,
			LIGHT_GREEN = 10,
			LIGHT_CYAN = 11,
			LIGHT_RED = 12,
			LIGHT_MAGENTA = 13,
			LIGHT_BROWN = 14,
			WHITE = 15,
};
}

class Console {
public:
	static void initConsole();
	static void setColor(VGA::Color fg, VGA::Color bg);
	static void clear();
	static void write(char c);
	static void write(const char *str);
	static void write(std::string& str);
	static void write(std::string&& str);
	static void write(const std::string& str);
	static void toggleCursor(bool enabled);
	static std::size_t read(char* buf, std::size_t count);
	static void keyboardInput(char character);

private:
	Console();
	~Console() {}

	static const std::size_t VGA_WIDTH;
	static const std::size_t VGA_HEIGHT;

	static char kbdBuf[KBD_BUF_SIZE];
	static std::size_t kbdBufStart;
	static std::size_t kbdBufSize;

	static std::size_t row;
	static std::size_t column;
	static std::uint8_t color;
	static std::uint16_t* screen;
	static bool cursor_enabled;

	static void scrollUp();
	static void putEntryAt(char c, std::size_t x, std::size_t y);
	static void updateCursor();
	static void processChar(char c);

	static std::uint8_t makeColor(VGA::Color fg, VGA::Color bg);
	static std::uint16_t makeVgaEntry(char c, std::uint8_t color);
};



#endif /* KERNEL_CONSOLE_HPP_ */
