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
#include <atd_eng_magnitude.h>
#include <atd_decimal.h>

// (RRR) ¿Podrían ser todos estos tipos genéricos?
//       Creo que no. Hay micros que no usan double como el atmega32, pero
//       otros que puede que si los usen. En ese caso sería mejor usar double
//       en lugar de Decimal (???)
namespace avr{

// (???) ¿Usar o no usar decimales con las frecuencias?
//       Si se usan decimales no entran en uint16_t así que hay que usar un
//       uint32_t. ¿O será mejor no usar Decimal y directamente uint16_t?
using Frequency = atd::ENG_frequency<atd::Decimal<uint32_t, 3>>;
using Time      = atd::ENG_time<atd::Decimal<uint32_t, 3>>;

// syntactic sugar
namespace literals{
constexpr Frequency operator"" _Hz (unsigned long long int freq)
{return Frequency{freq, 0};}

constexpr Frequency operator"" _KHz (unsigned long long int freq)
{return Frequency{freq, 3};}

constexpr Frequency operator"" _MHz (unsigned long long int freq)
{return Frequency{freq, 6};}

constexpr Time operator"" _s (unsigned long long int t)
{return Time{t, 0};}

constexpr Time operator"" _ms (unsigned long long int t)
{return Time{t, -3};}

constexpr Time operator"" _us (unsigned long long int t)
{return Time {t, -6};}

}

}// namespace


#endif


