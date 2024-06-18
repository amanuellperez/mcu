// Copyright (C) 2022 Manuel Perez 
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

#pragma once

#ifndef __LCD_BIG_DIGITS_H__
#define __LCD_BIG_DIGITS_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Escribamos cifras grandes en el LCD
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    05/02/2022 Escrito
 *
 ****************************************************************************/
#include "dev_HD44780_charset.h" // symbol TODO: not_generic!!!
#include <atd_math.h>	// Digits_of
#include <atd_names.h>	// Width

namespace dev{

template <typename Font_t>
class Big_digit{
public:
    using Font   = Font_t;

    static constexpr uint8_t rows = Font::rows;
    static constexpr uint8_t cols = Font::cols;

    /// Carga en memoria los bricks para poder dibujar los big_digits.
    template <typename Screen>
    static void load(Screen& scr);

    /// Imprime el digit i (de 0 a 9). 
    // CUIDADO: no imprime números, sino digits!!!
    // DUDA: llamar a esta función `print_digit` y a la función `print_number`
    // simplemente `print`?
    template <typename Screen>
    static bool print(Screen& scr, uint8_t i);

    /// Imprime el número 'n'.
    /// return: el número de cifras escritas en la Screen.
    // La diferencia con print es que print imprime números de 1 cifra,
    // mientras que esta función imprime todas las cifras de n.
    // Si width es distinto de cero, indica el número mínimo de cifras que se
    // imprimen. En caso de ser menos de width se llenan con '0'.
    template <typename Screen, typename Int>
    static uint8_t print_number(Screen& scr, Int n, 
			    const nm::Width<int>& w = 0);

    /// Imprime ':'
    template <typename Screen>
    static void print_colon(Screen& scr);

private:
    // Imprime los dígitos [j0, j0 + 3) del digit i
    // return: true si todo va bien.
    //	       false si hay algún error.
    template <typename Screen>
    static bool print_3bricks(Screen& scr, uint8_t j0, uint8_t i);
};

template <typename F>
template <typename Screen>
void Big_digit<F>::load(Screen& scr)
{
    for (uint8_t i = 0; i < Font::nbricks; ++i)
	scr.new_extended_char(i, Font::brick(i));
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
bool Big_digit<F>::print_3bricks(Screen& scr, uint8_t j0, uint8_t i)
{
    using symbol = dev::HD44780_charset_A00;

    for (uint8_t j = j0; j < j0 + Font::cols; ++j)
    {
	if ( Font::is_digit_char_full(i, j)){
	    if (!scr.print(symbol::of("█")))
		return false;
	}

	else if ( Font::is_digit_char_space(i, j)){
	    if (!scr.print(' '))
		return false;
	}

	else{
	    if (!scr.print_extended(Font::digit(i,j)))
		return false;
	}
    }

    return true;
}

template <typename F>
template <typename Screen>
bool Big_digit<F>::print(Screen& scr, uint8_t i)
{
    uint8_t x = scr.cursor_pos_x();
    uint8_t y = scr.cursor_pos_y();

    for (uint8_t k = 0; k < Font::rows; ++k){
        if (!print_3bricks(scr, k * Font::cols, i))
	    return false;

        scr.cursor_pos(x, y + k + 1);
    }

    scr.cursor_pos(x + Font::cols, y);

    return true;

}

template <typename F>
template <typename Screen, typename Int>
uint8_t Big_digit<F>::print_number(Screen& scr,
                                Int x,
                                const nm::Width<int>& w)
{
    atd::Digits_from_left_to_right d{x, w};

    uint8_t n = 0;
    for (auto p = d.begin(); p != d.end(); ++p, ++n)
    {
	if (!print(scr, *p))
	    return n;
    }

    return n;
}


// Esta implementación es la implementación por defecto. 
// MEJORA: que detecte si Font::print_colon está definido en cuyo caso se
// llamaría a esa implementación permitiendo a cada font definir su propio
// ':'
template <typename F>
template <typename Screen>
void Big_digit<F>::print_colon(Screen& scr)
{
    using symbol = dev::HD44780_charset_A00;

    if constexpr (Font::rows== 1){
	scr.print(':');
    }
    else{
	uint8_t x = scr.cursor_pos_x();
	uint8_t y = scr.cursor_pos_y();

	if constexpr (Font::rows== 2){
	    scr.print(symbol::of("･"));
	    scr.cursor_pos(x, y + 1);
	    scr.print(symbol::of("･"));
	}

	else if constexpr (Font::rows == 3){
	    scr.print(symbol::of("｡"));
	    scr.cursor_pos(x, y + 1);
	    scr.print(symbol::of("｡"));
	    //scr.print(symbol::of("º"));
	}
	else if constexpr (Font::rows == 4){
	    scr.cursor_pos(x, y + 1);
	    scr.print('.');
	    scr.cursor_pos(x, y + 2);
	    scr.print('.');
	}
	else{
	    static_assert(atd::always_false_v<F>, "Case not implemented");
	}

	scr.cursor_pos(x+1, y);
    }
	

}

// Caracteres genéricos que usamos en todas las fuentes
namespace big_digits{
constexpr const uint8_t char_full = 255;
constexpr const uint8_t char_space = 254;
}// namespace

/***************************************************************************
 *			    DEFAULT FONT
 ***************************************************************************/
// (RRR) Quiero que las funciones de Screen se puedan parametrizar por el tipo 
//       de fuente que se quiere usar: `print<Font>(34);` para ello es
//       necesario tener la fuente por defecto, en cuyo caso se llamará a la
//       función normal `print(34);`
struct Font_default{
    static constexpr uint8_t rows = 1;
    static constexpr uint8_t cols = 1;
};

template <>
class Big_digit<Font_default>{
public:
    using Font = Font_default;

    static constexpr uint8_t rows = 1;
    static constexpr uint8_t cols = 1;

    template <typename Screen>
    static void load(Screen& scr) {}

    /// Imprime el digit i (de 0 a 9). 
    // La precondición de esta función dice que `i` es un digit. Por eso basta
    // con llamar a print.
    template <typename Screen>
    static bool print(Screen& scr, uint8_t i)
    {return scr.print(i);}

    /// Imprime el número 'n'.
    /// return: el número de cifras escritas en la Screen.
    template <typename Screen>
    static uint8_t print_number(Screen& scr, uint8_t n, 
			    const nm::Width<int>& w = 0)
    { return scr.print(n, w);}

    template <typename Screen>
    static void print_colon(Screen& scr)
    { scr.print(':');}
};


using Font_digit_default = Big_digit<Font_default>;

} // namespace dev
#endif

