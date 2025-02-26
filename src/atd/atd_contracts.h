// Copyright (C) 2025 Manuel Perez 
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

#ifndef __ATD_CONTRACTS_H__
#define __ATD_CONTRACTS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	A falta del estandar C++26 creo mis propias pre y postcondiciones.
 *	Cuando esté el estandar C++26 esto sobrará.
 *
 * HISTORIA
 *    Manuel Perez
 *    26/02/2025 precondition
 *
 ****************************************************************************/

#include <string_view>
#include "atd_trace.h"

namespace atd{

// Si no se satisface la condición se muestra un mensaje de error, warning...
// Solo si TRACE_LEVEL >= level.
template <uint8_t level>
inline void precondition(bool condition, std::string_view msg)
{
    if constexpr (trace_level<level>()){
	if (!condition)
	    ctrace<level>() << msg << '\n';
    }
}

} // atd


#endif


