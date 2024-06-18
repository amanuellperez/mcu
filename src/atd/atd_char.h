// Copyright (C) 2023 Manuel Perez 
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

#ifndef __ATD_CHAR_H__
#define __ATD_CHAR_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Concebimoos `char` como objeto para poder escribir de forma más
 *	legible algunas funciones.
 *
 *	Ejemplo:
 *	La función 
 *		is_one_of(c, "abc");
 *	frente a
 *		Char{c}.is_one_of("abc");
 *
 *	Es más legible la segunda forma.
 *
 *	De hecho es una pena que C++ no tenga "argumentos movibles" (<-- qué
 *	nombre le damos?). Esto es, que yo pudiera definir la función:
 *	    char_is_one_of(char c, const char* str);
 *
 *	pero a la hora de llamarla pueda poner los argumentos en otras
 *	posiciones:
 *	    char(c)_is_one_of(str); <--- más legible!!!
 *
 * HISTORIA
 *    Manuel Perez
 *    08/10/2023 is_one_of
 *
 ****************************************************************************/

namespace atd{

struct Char{
    explicit Char(char c0) : c{c0} { }

    // chars[] = cadena de C con los caracteres que se buscan
    bool is_one_of(const char chars[]);

    const char c;
};

}// namespace atd
#endif



