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

#ifndef __ATD_FLOAT_H__
#define __ATD_FLOAT_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para manejar floating points (genéricos, incluye atd::Decimal)
 *
 * HISTORIA
 *    Manuel Perez
 *    12/10/2023 Integer_and_decimal_part
 *
 ****************************************************************************/
#include "atd_concepts.h"
#include "atd_decimal.h"

namespace atd{



// Integer_and_decimal_part
// ------------------------
// (RRR) Cuando se quiere leer una temperatura, hay sensores que devuelven 2
//       números: la parte entera y la parte decimal.
//
//       Un programa, en cambio, quiere operar con un tipo float: `float`,
//       `double`, `atd::Decimal`...
//
//       Los devices NO deben de conocer el tipo de float que va a usar el
//       programa, luego se necesita una forma genérica que permita
//       transformar la parte {entera, decimal} en un tipo cualquier `Float`
//
//       Esta clase es un primer intento de conseguir eso.
template <Type::Integer Int>
struct Integer_and_decimal_part{
// Construction
    Integer_and_decimal_part( Int integer_part0 = Int{0}
			    , Int decimal_part0 = Int{0})
	    : integer_part{integer_part0}, decimal_part{decimal_part0} {}

// Conversion to floating point type
// TODO: quedaría mejor as<float>()??? Pero ¿cómo implementarlo?
//	float as_float() const;
//	double as_double() const;
    
    // Este es incómodo de usar:
//    template <typename Rep, int ndecimals>
//    Decimal<Rep, ndecimals> as_decimal()
//    { return {integer_part, decimal_part}; }

    // Escribe el número {integer_part, decimal_part} en `res`
    template <typename Rep, int ndecimals>
    void as(Decimal<Rep, ndecimals>& res)
    { res = Decimal<Rep, ndecimals>{integer_part, decimal_part}; }

// Data
    Int integer_part;
    Int decimal_part;
};



}// namespace atd

#endif
