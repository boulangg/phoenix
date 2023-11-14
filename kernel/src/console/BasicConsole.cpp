/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "BasicConsole.h"

#include <algorithm>
#include <cstring>

namespace kernel::console {

BasicConsole::BasicConsole(const Framebuffer& fb) : BasicConsole()
{
    _fb = fb;
    setColor(Color::LIGHT_GREY, Color::BLACK);
    _screen = static_cast<uint32_t*>(_fb.address);
    _pixelHeight = base_font.height;
    _pixelWidth = base_font.width + 1ul;
    _maxColumn = (_fb.width - OFFSET_LEFT - OFFSET_RIGHT) / _pixelWidth;
    _maxRow = (_fb.height - OFFSET_TOP - OFFSET_DOWN) / _pixelHeight;
}

void BasicConsole::setColor(std::uint32_t fg, std::uint32_t bg)
{
    _fgColor = fg;
    _bgColor = bg;
}

void BasicConsole::clear()
{
    std::fill_n(_screen, _fb.width * _fb.height, _bgColor);
    _currentColumn = 0;
    _currentRow = 0;
}

void BasicConsole::write(char c)
{
    processChar(c);
}

void BasicConsole::write(const char* str)
{
    std::size_t index = 0;
    while (str[index] != '\0') {
        write(str[index]);
        index++;
    }
}

void BasicConsole::write(const std::string& str)
{
    write(str.c_str());
}

void BasicConsole::write(std::string&& str)
{
    write(str.c_str());
}

void BasicConsole::processChar(char c)
{
    if (c == '\b') {
        if (_currentColumn > 0) {
            --_currentColumn;
            putEntryAt(' ', _currentColumn, _currentRow);
        }
    } else if (c == '\t') {
        _currentColumn = (_currentColumn & 0xF8) + 0x8;
    } else if (c == '\n') {
        _currentColumn = 0;
        ++_currentRow;
    } else if (c == '\f') {
        clear();
    } else if (c == '\r') {
        _currentColumn = 0;
    } else if (c >= 32 && c <= 126) {
        putEntryAt(c, _currentColumn, _currentRow);
        ++_currentColumn;
    }

    if (_currentColumn >= _maxColumn) {
        _currentColumn = 0;
        ++_currentRow;
    }
    if (_currentRow >= _maxRow) {
        scrollUp();
    }
}

void BasicConsole::scrollUp()
{
    std::size_t size = _pixelHeight * _fb.width;
    for (size_t y = 0; y < _maxRow - 1; ++y) {
        std::uint32_t* base = _screen + (OFFSET_TOP + y * _pixelHeight) * _fb.width;
        std::memcpy(base, base + size, size * sizeof(uint32_t));
    }

    std::uint32_t* base = _screen + (OFFSET_TOP + (_maxRow - 1) * _pixelHeight) * _fb.width;
    std::fill_n(base, _pixelHeight * _fb.width, 0ul);

    --_currentRow;
}

void BasicConsole::putEntryAt(char c, std::size_t column, std::size_t row)
{
    std::uint8_t* glyphBase = (std::uint8_t*)&base_font + base_font.headerSize;

    std::uint8_t offset = 0;
    if ((std::uint32_t)(c) < base_font.numGlyphs) {
        offset = (std::uint8_t)c;
    }
    std::size_t font_bpl = (base_font.width + 7) / 8;
    std::uint8_t* glyph = glyphBase + offset * base_font.bytesPerGlyph;

    std::size_t x0 = OFFSET_LEFT + column * _pixelWidth;
    std::size_t y0 = (OFFSET_TOP + row * _pixelHeight) * _fb.width;

    for (std::size_t y = y0; y < y0 + base_font.height * _fb.width; y += _fb.width) {
        std::uint8_t mask = 0;
        std::uint8_t glyphPart = 0;
        std::uint8_t counter = 0;
        for (std::size_t x = x0; x < x0 + base_font.width; ++x) {
            if (mask == 0) {
                mask = 1 << 7;
                glyphPart = *(glyph + counter);
                ++counter;
            }
            std::size_t color = (glyphPart & mask) ? _fgColor : _bgColor;
            *(_screen + x + y) = color;
            mask = mask >> 1;
        }
        glyph += font_bpl;
    }
}

}
