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
#include <atd_names.h>	// Width

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
    static bool print(Screen& lcd, uint8_t i);

    /// Imprime el número 'n'.
    /// return: el número de cifras escritas en la Screen.
    // La diferencia con print es que print imprime números de 1 cifra,
    // mientras que esta función imprime todas las cifras de n.
    // Si width es distinto de cero, indica el número mínimo de cifras que se
    // imprimen. En caso de ser menos de width se llenan con '0'.
    template <typename Screen>
    static uint8_t print_number(Screen& lcd, uint8_t n, 
			    const nm::Width<int>& width = 0);

private:
    // Imprime los dígitos [j0, j0 + 3) del digit i
    // return: true si todo va bien.
    //	       false si hay algún error.
    template <typename Screen>
    static bool print_3bricks(Screen& lcd, uint8_t j0, uint8_t i);
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
//
// DUDA: el código se lee bastante peor por culpa de la gestión de errores.
// ¿Cómo mejorarla? Si defino un state del Screen, si el state está mal no se
// imprimiría nada, y bastaría con hacerle un check al final al state.
template <typename F>
template <typename Screen>
bool Big_digit<F>::print_3bricks(Screen& lcd, uint8_t j0, uint8_t i)
{
    using symbol = dev::HD44780_charset_A00;

    for (uint8_t j = j0; j < j0 + Font::cols; ++j)
    {
	if ( Font::is_digit_char_full(i, j)){
	    if (!lcd.print(symbol::of("█")))
		return false;
	}

	else if ( Font::is_digit_char_space(i, j)){
	    if (!lcd.print(' '))
		return false;
	}

	else{
	    if (!lcd.print_extended(Font::digit(i,j)))
		return false;
	}
    }

    return true;
}

template <typename F>
template <typename Screen>
bool Big_digit<F>::print(Screen& lcd, uint8_t i)
{
    uint8_t x = lcd.cursor_pos_x();
    uint8_t y = lcd.cursor_pos_y();

    for (uint8_t k = 0; k < Font::rows; ++k){
        if (!print_3bricks(lcd, k * Font::cols, i))
	    return false;

        lcd.cursor_pos(x, y + k + 1);
    }

    lcd.cursor_pos(x + Font::cols, y);

    return true;

}

template <typename F>
template <typename Screen>
uint8_t Big_digit<F>::print_number(Screen& lcd,
                                uint8_t x,
                                const nm::Width<int>& w)
{
    atd::Digits_from_left_to_right d{x, w};

    uint8_t n = 0;
    for (auto p = d.begin(); p != d.end(); ++p, ++n)
    {
	if (!print(lcd, *p))
	    return n;
    }

    return n;
}


// Caracteres genéricos que usamos en todas las fuentes
namespace big_digits{
constexpr const uint8_t char_full = 255;
constexpr const uint8_t char_space = 254;
}// namespace

// (RRR) Quiero que las funciones de Screen se puedan parametrizar por el tipo 
//       de fuente que se quiere usar: `print<Font>(34);` para ello es
//       necesario tener la fuente por defecto, en cuyo caso se llamará a la
//       función normal `print(34);`
struct Font_default{
    static constexpr uint8_t rows = 2;
    static constexpr uint8_t cols = 1;
};

using Big_digit_default = Big_digit<Font_default>;

} // namespace dev
#endif

