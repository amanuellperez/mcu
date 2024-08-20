// Copyright (C) 2024 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the MCU++ Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


// This file has been generated automatically by `ttf_txt2bin.py`


#pragma once
#ifndef __ROM_FONT_DOGICA_NUMBER_5X7_CR_H__
#define __ROM_FONT_DOGICA_NUMBER_5X7_CR_H__

#include <atd_rom.h>
// #include <avr_memory.h> <-- hay que incluirlo antes de este archivo

namespace rom{
namespace font_dogica_number_5x7_cr{

using ROM_read = MCU::ROM_read;

struct Font{
static constexpr bool by_columns = true;
static constexpr uint8_t rows           = 7; // número de filas que tiene cada font
static constexpr uint8_t cols           = 5; // número de columnas que tiene cada font
static constexpr uint8_t col_in_bytes   = 1; // número de bytes que tiene cada columna
static constexpr uint8_t nchars         = 10; // número de caracteres

static constexpr
atd::ROM_biarray<uint8_t, nchars, cols*1, ROM_read> glyph
	PROGMEM = {
0b00111110, 0b01000001, 0b01011101, 0b01000001, 0b00111110, // 0
0b01000010, 0b01000010, 0b01111111, 0b01000000, 0b01000000, // 1
0b01000010, 0b01100001, 0b01010001, 0b01001001, 0b01000110, // 2
0b01000001, 0b01001001, 0b01001001, 0b01001001, 0b00110110, // 3
0b00011000, 0b00010100, 0b00010010, 0b01111111, 0b00010000, // 4
0b00100111, 0b01000101, 0b01000101, 0b01000101, 0b00111001, // 5
0b00111110, 0b01001001, 0b01001001, 0b01001001, 0b00110010, // 6
0b00000011, 0b01100001, 0b00010001, 0b00001001, 0b00000111, // 7
0b00110110, 0b01001001, 0b01001001, 0b01001001, 0b00110110, // 8
0b00000110, 0b01001001, 0b01001001, 0b00101001, 0b00011110// 9
}; // glyphs
}; // struct Font


} // namespace font
} // namespace rom

#endif
