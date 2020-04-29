// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ATD_BCD_H__
#define __ATD_BCD_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Interfaz para manejo de tipo BCD.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    29/04/2020 v0.0. Versión mínima. Experimental.
 *
 ****************************************************************************/

namespace atd{

/// Convierte un número 'x' de 2 cifras escrito en BCD a binario.
/// El orden del BCD tiene que ser el humano: 'decenas|unidades'
template <typename Int>
inline Int BCD2int(const Int& x)
{
    Int u = (x & 0x0F);		// unidades
    Int d = (x & 0xF0) >> 4;	// decenas

    return Int{10}*d + u;
}

/// Convierte un número 'x' de 2 cifras escrito en binario a BCD.
/// El orden del BCD tiene que ser el humano: 'decenas|unidades'
template <typename Int>
inline Int int2BCD(const Int& x)
{
    Int u = x % Int{10};    // unidades
    Int d = (x-u)/Int{10};  // decenas

    return (d << 4) | u;
}


}// namespace

#endif



