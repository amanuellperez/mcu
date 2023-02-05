// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __STD_CSTDIO_H__
#define __STD_CSTDIO_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Ampliación de cstdio.
 *
 *   - HISTORIA:
 *       Manuel Perez - 13/10/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"

namespace STD{

/// Transforma el entero x en una cadena tipo c, almacenándola en el buffer 
/// [p0, pe). Devuelve la posición pc en donde EMPIEZA el entero como cadena
/// El número x como cadena estará almacenado en [pc, pe).
/// Precondición: x > 0
/// Es el equivalente a sprintf.
template <typename It, typename Int>
It __posint_to_cstring(It p0, It pe, Int x)
{
    It pc = pe;

    while (x != 0 and pc != p0){
	Int r = x % 10;
	x = (x -  r) / 10;
	
	--pc;
	*pc = '0' + r;	// to_ascii(r)
    }

    return pc;
}


/// Transforma el entero x en una cadena tipo c, almacenándola en el buffer 
/// [p0, pe). Devuelve la posición pc en donde EMPIEZA el entero como cadena
/// El número x como cadena estará almacenado en [pc, pe).
///
/// Si Int es signed esta función falla para el caso en que valga
/// std::numeric_limits::min() ya que falla el operador -x para este caso.
/// Es el equivalente a sprintf.
template <typename It, typename Int>
It __int_to_cstring(It p0, It pe, Int x)
{
    if (p0 == pe)
	return p0;

    It pc = pe; // = ultima posición donde insertamos una cifra
    
    if (x == 0){
	--pc;
	*pc = '0';
	return pc;
    }

    bool negativo  = false;
    if (x < 0){
	negativo = true;
	x = -x;	// Esta operación falla si x == std::numeric_limits::min()!!!
    }

    pc = __posint_to_cstring(p0, pe, x);

    if (negativo and pc != p0){
	--pc;
	*pc = '-';
    }

    return pc;
}


} // namespace


#endif


