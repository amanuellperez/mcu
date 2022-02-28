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
#include <atd_math.h>	// Digits_of

namespace dev{

template <typename Font_t>
class Big_digit{
public:
    using Font   = Font_t;

    /// Carga en memoria los bricks para poder dibujar los big_digits.
    template <typename Screen>
    static void load(Screen& lcd);

    /// Imprime el digit i (de 0 a 9). 
    // CUIDADO: no imprime números, sino digits!!!
    template <typename Screen>
    static void print(Screen& lcd, uint8_t i);

    /// Imprime el número 'n'.
    // La diferencia con print es que print imprime números de 1 cifra,
    // mientras que esta función imprime todas las cifras de n.
    // Si width es distinto de cero, indica el número mínimo de cifras que se
    // imprimen. En caso de ser menos de width se llenan con '0'.
    template <typename Screen>
    static void print_number(Screen& lcd, uint8_t n, uint8_t width = 0);

private:
    // Imprime los dígitos [j0, j0 + 3) del digit i
    template <typename Screen>
    static void print_3bricks(Screen& lcd, uint8_t j0, uint8_t i);
};

template <typename F>
template <typename Screen>
void Big_digit<F>::load(Screen& lcd)
{
    for (uint8_t i = 0; i < Font::nbricks; ++i)
	lcd.new_extended_char(i, Font::brick(i));
}


// (RRR) ¿Por qué usar symbol y no el código correspondiente? 
//       Lo bueno de symbol es que esta función no hay que tocarla aunque
//       cambiemos de LCD. symbol se encarga de saber qué código corresponde
//       al símbolo correspondiente. De esta forma es código genérico.
template <typename F>
template <typename Screen>
void Big_digit<F>::print_3bricks(Screen& lcd, uint8_t j0, uint8_t i)
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

template <typename F>
template <typename Screen>
void Big_digit<F>::print(Screen& lcd, uint8_t i)
{
    uint8_t x = lcd.cursor_pos_x();
    uint8_t y = lcd.cursor_pos_y();

    for (uint8_t k = 0; k < Font::rows; ++k){
        print_3bricks(lcd, k * Font::cols, i);
        lcd.cursor_pos(x, y + k + 1);
    }

    lcd.cursor_pos(x + Font::cols, y);
}


template <typename F>
template <typename Screen>
void Big_digit<F>::print_number(Screen& lcd, uint8_t n, uint8_t width)
{
    atd::Digits_from_left_to_right d{n, width};

    for (auto p = d.begin(); p != d.end(); ++p)
	print(lcd, *p);
}


// Caracteres genéricos que usamos en todas las fuentes
namespace big_digits{
constexpr const uint8_t char_full = 255;
constexpr const uint8_t char_space = 254;
}// namespace

} // namespace dev
#endif

