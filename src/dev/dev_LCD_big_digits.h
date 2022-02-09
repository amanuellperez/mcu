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
 *    05/02/2022 Escrito
 *
 ****************************************************************************/
#include "dev_LCD_HD44780_charset.h" // symbol

namespace dev{

template <typename LCD_screen, typename Font_t>
class Big_digit{
public:
    using Screen = LCD_screen;
    using Font   = Font_t;

    static void init(Screen& lcd);

    /// Imprime el digit i (de 0 a 9). 
    // CUIDADO: no imprime números, sino digits!!!
    static void print(Screen& lcd, uint8_t i);

private:
    // Imprime los dígitos [j0, j0 + 3) del digit i
    static void print_3bricks(Screen& lcd, uint8_t j0, uint8_t i);
};

template <typename S, typename F>
void Big_digit<S, F>::init(Screen& lcd)
{
    for (uint8_t i = 0; i < Font::nbricks; ++i)
	lcd.new_extended_char(i, Font::brick(i));
}


// (RRR) ¿Por qué usar symbol y no el código correspondiente? 
//       Lo bueno de symbol es que esta función no hay que tocarla aunque
//       cambiemos de LCD. symbol se encarga de saber qué código corresponde
//       al símbolo correspondiente. De esta forma es código genérico.
template <typename S, typename F>
void Big_digit<S, F>::print_3bricks(Screen& lcd, uint8_t j0, uint8_t i)
{
    using symbol = dev::HD44780_charset_A00;

    for (uint8_t j = j0; j < j0 + Font::cols; ++j)
    {
	if ( Font::is_digit_char_full(i, j))
	    lcd.print(symbol::of("█"));

	else if ( Font::is_digit_char_space(i, j))
	    lcd.print(' ');

	else
	    lcd.print_extended(Font::digit(i,j));
    }
}

template <typename S, typename F>
void Big_digit<S, F>::print(Screen& lcd, uint8_t i)
{
    uint8_t x = lcd.cursor_pos_x();
    uint8_t y = lcd.cursor_pos_y();

    for (uint8_t k = 0; k < Font::rows; ++k){
        print_3bricks(lcd, k * Font::cols, i);
        lcd.cursor_pos(x, y + k + 1);
    }

    lcd.cursor_pos(x + Font::cols, y);
}


} // namespace dev
#endif

