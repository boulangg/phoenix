/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "limine.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* font */
struct psf2
{
    uint32_t magic;
    uint32_t version;
    uint32_t headerSize;
    uint32_t flags;
    uint32_t numGlyphs;
    uint32_t bytesPerGlyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed));
extern psf2 base_font;

#ifdef __cplusplus
}
#endif

namespace kernel::console {

struct Framebuffer
{
    uint32_t* address;
    uint64_t width;
    uint64_t height;
};

struct Color
{
    static constexpr std::uint32_t BLACK = 0x000000;
    static constexpr std::uint32_t BLUE = 0xFFFFFF;
    static constexpr std::uint32_t GREEN = 0x00AA00;
    static constexpr std::uint32_t CYAN = 0x00AAAA;
    static constexpr std::uint32_t RED = 0xAA0000;
    static constexpr std::uint32_t MAGENTA = 0xAA00AA;
    static constexpr std::uint32_t BROWN = 0xAA5500;
    static constexpr std::uint32_t LIGHT_GREY = 0xAAAAAA;
    static constexpr std::uint32_t DARK_GREY = 0x555555;
    static constexpr std::uint32_t LIGHT_BLUE = 0x5555FF;
    static constexpr std::uint32_t LIGHT_GREEN = 0x55FF55;
    static constexpr std::uint32_t LIGHT_CYAN = 0x55FFFF;
    static constexpr std::uint32_t LIGHT_RED = 0xFF5555;
    static constexpr std::uint32_t LIGHT_MAGENTA = 0xFF55FF;
    static constexpr std::uint32_t LIGHT_BROWN = 0xFFFF55;
    static constexpr std::uint32_t WHITE = 0xFFFFFF;
};

class BasicConsole
{
public:
    BasicConsole() = default;
    BasicConsole(const Framebuffer& fb);

    void setColor(std::uint32_t fg, std::uint32_t bg);
    void clear();
    void write(char c);
    void write(const char* str);
    void write(const std::string& str);
    void write(std::string&& str);

private:
    std::size_t _currentRow = 0;
    std::size_t _currentColumn = 0;

    std::size_t _pixelHeight = 0;
    std::size_t _pixelWidth = 0;
    std::size_t _maxRow = 0;
    std::size_t _maxColumn = 0;

    std::uint32_t _fgColor = 0;
    std::uint32_t _bgColor = 0;

    Framebuffer _fb = {};

    uint32_t* _screen = nullptr;

    void processChar(char c);
    void scrollUp();
    void putEntryAt(char c, std::size_t column, std::size_t row);

    static constexpr std::size_t OFFSET_TOP = 0x04;
    static constexpr std::size_t OFFSET_DOWN = 0x04;
    static constexpr std::size_t OFFSET_LEFT = 0x02;
    static constexpr std::size_t OFFSET_RIGHT = 0x01;
};

}
