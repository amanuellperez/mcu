// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#ifndef __ATD_ALGORITHM_H__
#define __ATD_ALGORITHM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de algorithm
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel Lopez
 *	02/11/2019 find_c
 *
 ****************************************************************************/

namespace atd{

// find_c
// ------
/// Busca en la cadena de c = [p, '\0') el valor de x.
/// Returns: puntero a la primera posición de 'x' en caso de encontrarlo, 
/// o en caso de no encontarlo un puntero a '\0', el end de la cadena de C.
inline constexpr const char* find_c(const char* p, char x)
{
    while(*p != '\0' and *p != x)
	++p;

    return p;
}



/// Busca la n-ésima posición del caracter 'x' dentro de la cadena [p, '\0).
/// Returns: la posición, en caso de encontrarla.
///	     Si falla devuelve un puntero a '\0'. (al end de la cadena)
// Precondicion: n > 0 and p cadena de c.
template <typename Int>
inline constexpr const char* find_c(const char* p, char x, Int n)
{
    p = find_c(p, x);
    --n;

    for (; n != Int{0} and *p != '\0'; --n){
	++p;
	p = find_c(p, x);
    }
    
    return p;
}



}// namespace

#endif

