// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __AVR_TYPES_H__
#define __AVR_TYPES_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Tipos comunes a usar.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    31/01/2021 v0.0
 *
 ****************************************************************************/
#include <atd_magnitude.h>
#include <atd_decimal.h>

// (RRR) ¿Podrían ser todos estos tipos genéricos?
//       Creo que no. Hay micros que no usan double como el atmega32, pero
//       otros que puede que si los usen. En ese caso sería mejor usar double
//       en lugar de Decimal (???)
namespace avr{

// Las frecuencias y los periodos tienen que tener el mismo tipo rep 
// (mismo número número de decimales). 
// Con 2 decimales en uint32_t nos entra: 4*10^9 = 4*10^7*10^2, podemos 
// representar números con 7 cifras en la parte entera (hasta 40*10^6 Hz).
// TODO: cambiarle el nombre. Scalar? Scalar_type? Scalar_t?
using __Freq_Rep = atd::Decimal<uint32_t, 2>;

// Frequencies types
using Hertz     = atd::Hertz<__Freq_Rep>;
using KiloHertz = atd::KiloHertz<__Freq_Rep>;
using MegaHertz = atd::MegaHertz<__Freq_Rep>;

// Period types
using Second      = atd::Second<__Freq_Rep>;
using Millisecond = atd::Millisecond<__Freq_Rep>;
using Microsecond = atd::Microsecond<__Freq_Rep>;
using Nanosecond  = atd::Nanosecond<__Freq_Rep>;

// syntactic sugar
namespace literals{
constexpr Hertz operator"" _Hz (unsigned long long int freq)
{return Hertz{freq};}

constexpr KiloHertz operator"" _KHz (unsigned long long int freq)
{return KiloHertz{freq};}

constexpr MegaHertz operator"" _MHz (unsigned long long int freq)
{return MegaHertz{freq};}

constexpr Second operator"" _s (unsigned long long int t)
{return Second{t};}

constexpr Millisecond operator"" _ms (unsigned long long int t)
{return Millisecond{t};}

constexpr Microsecond operator"" _us (unsigned long long int t)
{return Microsecond{t};}

}

}// namespace


#endif


