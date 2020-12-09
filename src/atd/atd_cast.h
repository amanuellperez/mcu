// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ATD_CAST_H__
#define __ATD_CAST_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Cast que encajarían bien en std.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	27/08/2019 safe_static_cast
 *	10/11/2019 bounded_cast
 *
 ****************************************************************************/
#include <limits>
#include <cstddef>

namespace atd{
/*!
 *  \brief  Convierte el tipo r en IntReturn.
 *
 *  En caso de que el valor de r exceda los límites de IntReturn devuelve el
 *  valor mayor (si r > numeric_limits::max()) o menor (si r <
 *  numeric_limits::min()) que puede tomar ese tipo.
 *
 */

template <typename IntReturn, typename Int>
IntReturn bounded_cast(const Int& r)
{
    if (r < std::numeric_limits<IntReturn>::min())
	return std::numeric_limits<IntReturn>::min();

    else if (r > std::numeric_limits<IntReturn>::max())
	return std::numeric_limits<IntReturn>::max();

    else
	return static_cast<IntReturn>(r);
}




/// Hace un static cast convirtiendo T x en R x. Básicamente hace:
///	T x;
///	R ret = x;
/// Garantiza que no hay overflow y que se hace el cast en tiempo de
/// compilación.
template<typename R, typename T, T x>
inline constexpr R safe_static_cast()
{
    if constexpr (static_cast<T>(static_cast<R>(x)) == x)
	return static_cast<R>(x);

    else  // No funciona: static_assert(0,...) de ahi lo enrevesado
        static_assert(static_cast<T>(static_cast<R>(x)) == x,
                      "\nsafe_static_cast: overflow!\nEl static_cast que se "
                      "intenta hacer genera overflow.");

    // aqui nunca llega, pero para que no de warning
    return static_cast<R>(x);
}


// Conversión de bool -> std::byte (necesaria para usarla en las máscaras de
// bits 
inline std::byte to_byte(bool x)
{
    if (x)  return std::byte{1};
    else    return std::byte{0};
}

}// namespace


#endif


