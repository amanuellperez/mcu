// Copyright (C) 2020 Manuel Perez 
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

#ifndef __MCU_STD_CMATH_H__
#define __MCU_STD_CMATH_H__

/****************************************************************************
 *
 * - DESCRIPCION: El equivalente a <cmath>
 *
 * - HISTORIA:
 *  Manuel Perez 
 *	31/03/2020 abs
 *
 *
 ****************************************************************************/
#include "std_config.h"

#include <math.h>

// Eliminamos macros
#undef abs
#undef div
#undef acos
#undef asin
#undef atan
#undef atan2
#undef ceil
#undef cos
#undef cosh
#undef exp
#undef fabs
#undef floor
#undef fmod
#undef frexp
#undef ldexp
#undef log
#undef log10
#undef pow
#undef sin
#undef sinh
#undef sqrt
#undef tan
#undef tanh

namespace STD{

inline int abs(int n) { return n >= 0? n: -n; }
inline long abs(long n) { return n >= 0? n: -n; }
inline long long abs(long long n) { return n >= 0? n: -n; }


// El standard no define abs como constexpr y parece ser que prohibe a las
// implementaciones añadir constexpr cuando no está indicado así. Defino estas
// internas para poder usarlas.
constexpr inline int __abs(int n) { return n >= 0? n: -n; }
constexpr inline long __abs(long n) { return n >= 0? n: -n; }
constexpr inline long long __abs(long long n) { return n >= 0? n: -n; }


// No tengo claro si estas funciones están definidas como macros. De hecho,
// g++ internamente usa una función especial para implementarlas. 
// Como quiero que el código quede independiente de la implementación de g++
// doy este rodeo para definir todas estas funciones.
// ----
// modf
// ----
// Parece ser que avr-gcc no suministra modf para float y long double
//inline float __modf(float x, float* integral_part) 
//{ return modf(x, integral_part); }

inline double __modf(double x, double* integral_part)
{ return modf(x, integral_part); }

//inline long double __modf(long double x, long double* integral_part)
//{ return modf(x, integral_part); }

#undef modf

//inline float modf(float x, float* integral_part) 
//{ return __modf(x, integral_part); }

inline double modf(double x, double* integral_part)
{ return __modf(x, integral_part); }

//inline long double modf(long double x, long double* integral_part)
//{ return __modf(x, integral_part); }


}// namespace


#endif

