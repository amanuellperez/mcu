// Copyright (C) 2021 Manuel Perez 
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

#ifndef __AVR_TYPES_H__
#define __AVR_TYPES_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Tipos comunes a usar.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    31/01/2021 v0.0 Frequency and Time.
 *    30/03/2021      Potential.
 *
 ****************************************************************************/
#include <atd_magnitude.h>

// (RRR) ¿Podrían ser todos estos tipos genéricos?
//       Creo que no. Hay micros que no usan double como el atmega32, pero
//       otros que puede que si los usen. En ese caso sería mejor usar double
//       en lugar de Decimal (???)
namespace avr_{
// (???) ¿Usar o no usar decimales con las frecuencias?
//       Si se usan decimales no entran en uint16_t así que hay que usar un
//       uint32_t. ¿O será mejor no usar Decimal y directamente uint16_t?
//using Frequency = atd::ENG_frequency<atd::Decimal<uint32_t, 3>>;

// Frequency representa una frequencia en hercios. Es lo que en atd::Magnitude
// llamamos `atd::Hertz`. Usar el tipo base como uint32_t permite representar
// frecuenias no decimales desde 1Hz hasta 4GHz que para los avrs es
// demasiado. Lo que no permite es usar decimales. ¿Mejor un tipo con
// decimales como atd::Decimal<>?
// using Frequency = atd::ENG_frequency<uFloat16>; 
// DUDA: usar uFloat16? A día de hoy uFloat16 creo que mete 1kB más de código.
//       Mejorar esa clase antes de usarla de forma masiva.
//						      
//using Frequency = atd::ENG_frequency<uint32_t>;
using Frequency = atd::Hertz<uint32_t>;
using Hertz     = atd::Hertz<uint32_t>;
using KiloHertz = atd::KiloHertz<uint32_t>;
using MegaHertz = atd::MegaHertz<uint32_t>;

inline constexpr Frequency frequency_in_Hz(const Frequency::Rep& x)
//{ return Frequency{x, 0}; }
{ return Hertz{x}; }

inline constexpr Frequency frequency_in_kHz(const Frequency::Rep& x)
//{ return Frequency{x, 3}; }
{ return KiloHertz{x}; }

inline constexpr Frequency frequency_in_MHz(const Frequency::Rep& x)
//{ return Frequency{x, 6}; }
{ return MegaHertz{x}; }

// Time
//using Time      = atd::ENG_time<atd::Decimal<uint32_t, 3>>;

// almaceno en us el tiempo, dando un rango de:
//    0, 2^32 = 4000 segundos = 1h 11 minutos
//using Time      = atd::ENG_time<uint32_t>;
using Time      = atd::Second<uint32_t>;
using Second	= atd::Second<uint32_t>;
using Millisecond = atd::Millisecond<uint32_t>;
using Microsecond = atd::Microsecond<uint32_t>;

inline constexpr Time time_in_s(const Time::Rep& x) 
//{return Time{x, 0};}
{return Second{x};}

inline constexpr Time time_in_ms(const Time::Rep& x) 
//{return Time{x, -3};}
{return Millisecond{x};}
inline constexpr Time time_in_us(const Time::Rep& x) 
//{return Time{x, -6};}
{return Microsecond{x};}

// Electric potential
// Almacenamos el potencial en milivoltios, esa será la máxima resolución que
// podemos dar.
//using Potential = atd::ENG_electric_potential<atd::Decimal<uint32_t, 3>>;
//using Potential = atd::ENG_electric_potential<uint32_t>;
using Potential = atd::Millivolt<uint32_t>; // TODO: borrar Potential
using Volt      = atd::Volt<uint32_t>;
using Millivolt = atd::Millivolt<uint32_t>;
inline constexpr Potential potential_in_V(const Potential::Rep& x)
//{ return Potential{x, 0}; }
{ return Volt{x}; }

inline constexpr Potential potential_in_mV(const Potential::Rep& x)
//{ return Potential{x, -3}; }
{ return Millivolt{x}; }

inline constexpr Potential::Rep in_mV(const Millivolt& x)
//{ return x.millivalue(); }
{ return x.value(); }


// syntactic sugar
namespace literals{
// frequency
constexpr Frequency operator"" _Hz (unsigned long long int freq)
{return frequency_in_Hz(freq);}

constexpr Frequency operator"" _kHz (unsigned long long int freq)
{return frequency_in_kHz(freq);}

constexpr Frequency operator"" _MHz (unsigned long long int freq)
{return frequency_in_MHz(freq);}

// time
constexpr Time operator"" _s (unsigned long long int t)
{return time_in_s(t);}

constexpr Time operator"" _ms (unsigned long long int t)
{return time_in_ms(t);}

constexpr Time operator"" _us (unsigned long long int t)
{return time_in_us(t);}

// potential
constexpr Potential operator"" _V (unsigned long long int v)
{return potential_in_V(v);}

constexpr Potential operator"" _mV (unsigned long long int v)
{return potential_in_mV(v);}

}// namespace literals

}// namespace


#endif


