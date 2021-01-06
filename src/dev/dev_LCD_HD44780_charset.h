// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#pragma once

#ifndef __DEV_LCD_HD44780_CHARSET_H__
#define __DEV_LCD_HD44780_CHARSET_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Map of charset of LCD.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    06/01/2020 v0.0
 *
 ****************************************************************************/

namespace dev{

struct HD44780_charset_A00{

static constexpr const char end_code[] = "\x00\x00";
static constexpr const char first_code = 0xE0;

// Para escribir con 'vim' los símbolos usar: CTRL-V u 221a = escribe √
// Observar que los códigos utf8 que se van a usar son los que estoy
// escribiendo aquí.
static constexpr const char* codes[] = { 
"α", // = "\xce\xb1" 
"ä",
"β",
"ε",
"μ",
"σ",
"ρ",
"g", // 'g' larga
"\x22\x1a", // "√" u221A
"-1",// TODO: ¿cuál es el utf8 de ^-1?
"j", // 'j' larga
"ˣ",
"¢", 
"£", // \uC2A3
"ñ",
"ö", 

"p", // 'p' larga
"q", // 'q' larga
"θ",
"\x22\x1e", // "∞" u221E (No puedo escribirlo en símbolo, lo confunde con √. ¿por qué?)
"Ω", //'\u03A9'
"ü",
"Σ",
"π", // \u03C0
"\x53\x43", // x bar (average(x)) 
"y", // 'y' larga
"于", // \u4E8E. 
"万", //'\u4E07'
"円", //'\u5186'
"÷",
"■", //'\u25A0'
"█", //'\u2588': 0xFF,  # FULL BLOCK
end_code
};


static constexpr char _of(const char* str, int i) noexcept
{
    return ((codes[i][0] == str[0] and codes[i][1] == str[1])
        or (codes[i][0] == end_code[0] and codes[i][1] == end_code[1]))
             ? first_code
             : 1 + _of(str, ++i);
}

static constexpr char code_symbol_not_found()
{
    return first_code - 1 + sizeof(codes) / sizeof(const char*);
}

static constexpr char of(const char* str) noexcept
{
    return (_of(str, 0) == code_symbol_not_found()
                ? '?'
                : _of(str, 0));
}
};



}// namespace



#endif


