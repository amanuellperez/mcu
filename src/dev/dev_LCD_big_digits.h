// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#pragma once

#ifndef __LCD_BIG_DIGITS_H__
#define __LCD_BIG_DIGITS_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Escribamos cifras grandes en el LCD
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    05/02/2022 v0.0
 *
 ****************************************************************************/

#include "dev_LCD_HD44780_charset.h"

namespace dev{

namespace big_digits{
constexpr const uint8_t char_full = 255;
constexpr const uint8_t char_space = 254;

// TODO: meterlo en progmem
// Para el display de 4 filas (cada digit es de 3 x 3)
constexpr uint8_t bricks3x3_v1_size = 8;
constexpr const char bricks3x3_v1[8][bricks3x3_v1_size] /* PROGMEM */ = {
    { 0b00000011,
      0b00000111,
      0b00001111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00011000,
      0b00011100,
      0b00011110,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },
    
    { 0b00011111,
      0b00001111,
      0b00000111,
      0b00000011,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00011111,
      0b00011110,
      0b00011100,
      0b00011000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00000011,
      0b00000111,
      0b00001111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111 },

    { 0b00011000,
      0b00011100,
      0b00011110,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111 },

    { 0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00001110,  // decimal point
      0b00001110,
      0b00001110,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },
};

constexpr const uint8_t digits3x3_v1[10][9] /* PROGMEM */ = {
    // 0
    { 4, 6, 5,	
      char_full, char_space, char_full, 
      2, 6, 3 },

    // 1
    { 0, char_full, char_space,
      char_space, char_full, char_space,
      6, 6, 6 },

    // 2
    { 0, 6, 5,
      4, 6, 3, 
      6, 6, 6},

    // 3
    { 6, 6, 5,
      char_space, 6, char_full,
      6 , 6, 3},

    // 4
    { char_full, char_space, char_full,
      2, 6, char_full, 
      char_space, char_space, 6},

    // 5
    { char_full, 6, 6,
      2, 6, 5,
      2, 6, 3 },

    // 6
    { 4, 6, 1, 
      char_full, 6, 5,
      2, 6, 3 },

    // 7
    { 6, 6, char_full,
      char_space, 4, 3,
      char_space, 6, char_space},

    // 8
    { 4, 6, 5,
      char_full, 6, char_full,
      2, 6, 3},

    // 9
    { 4, 6, 5,
      2, 6, char_full,
      2, 6, 3}
};


} // namespace big_digits


template <typename LCD_screen>
class Big_digit_3x3{
public:
    using Screen = LCD_screen;

    static void init(Screen& lcd);

    /// Imprime el digit i (de 0 a 9). 
    // CUIDADO: no imprime números, sino digits!!!
    static void print(Screen& lcd, uint8_t i);

private:
    // Imprime los dígitos [j0, j0 + 3) del digit i
    static void print_3bricks(Screen& lcd, uint8_t j0, uint8_t i);
};

template <typename S>
void Big_digit_3x3<S>::init(Screen& lcd)
{
    for (uint8_t i = 0; i < big_digits::bricks3x3_v1_size; ++i)
	lcd.new_extended_char(i, big_digits::bricks3x3_v1[i]);
}


// (RRR) ¿Por qué usar symbol y no el código correspondiente? 
//       Lo bueno de symbol es que esta función no hay que tocarla aunque
//       cambiemos de LCD. symbol se encarga de saber qué código corresponde
//       al símbolo correspondiente. De esta forma es código genérico.
template <typename S>
void Big_digit_3x3<S>::print_3bricks(Screen& lcd, uint8_t j0, uint8_t i)
{
    using symbol = dev::HD44780_charset_A00;

    for (uint8_t j = j0; j < j0 + 3; ++j)
    {
	if ( big_digits::digits3x3_v1[i][j] == big_digits::char_full)
	    lcd.print(symbol::of("█"));
	else if ( big_digits::digits3x3_v1[i][j] == big_digits::char_space)
	    lcd.print(' ');
	else
	    lcd.print_extended(big_digits::digits3x3_v1[i][j]);
    }
}

template <typename S>
void Big_digit_3x3<S>::print(Screen& lcd, uint8_t i)
{
    uint8_t x = lcd.cursor_pos_x();
    uint8_t y = lcd.cursor_pos_y();

    print_3bricks(lcd, 0, i);

    lcd.cursor_pos(x, y + 1);
    print_3bricks(lcd, 3, i);

    lcd.cursor_pos(x, y + 2);
    print_3bricks(lcd, 6, i);

    lcd.cursor_pos(x+3, y);
}



} // namespace dev
#endif

