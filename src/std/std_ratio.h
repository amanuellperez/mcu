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

#ifndef __MCU_STD_RATIO_H__
#define __MCU_STD_RATIO_H__

/****************************************************************************
 *
 *  - DESCRIPCION: El equivalente a <ratio>
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	04/12/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_cstdint.h"
#include "std_type_traits.h"

namespace STD{

// ------------------
// Funciones de ayuda
// ------------------
constexpr inline intmax_t __static_sign(intmax_t x)
{ return x > 0? 1: -1;}

// C++17 no tiene constexpr abs!!!
constexpr inline intmax_t __static_abs(intmax_t x)
{ return x > 0? x: -x;}

constexpr inline intmax_t __static_gcd(intmax_t a, intmax_t b)
{
    while (true) {
        if (b == 0) return a;

        a = a % b;

        if (a == 0) return b;

        b = b % a;
    }

}

// least common multiple
// TODO: esta es una versión naive. ¿Qué pasa si a * b overflow pero al
// dividir por el gcd(a, b) realmente no hubiese overflow? Tendría que poderse
// calcular el valor.
constexpr inline intmax_t __static_lcm(intmax_t a, intmax_t b)
{ return __static_abs(a * b) / __static_gcd(a, b); }

/*!
 *  \brief  std::ratio
 */
template <intmax_t Num, intmax_t Den = 1>
struct ratio {
    static_assert(Den !=  0, "denominator cannot be zero");

    static constexpr intmax_t num =
	    Num * __static_sign(Den) / __static_gcd(Num, Den);

    static constexpr intmax_t den =
	    __static_abs(Den) / __static_gcd(Num, Den);

    using type = ratio<num, den>;
};

// -----------
// Operaciones
// -----------
// Naive implementation: de momento implemento lo más sencillo. gcc tiene una
// implementación más complicada, supongo que para evitar overflows o dar un
// error en caso de que se produzca alguno (???)
// FUTURO: Una forma de saber que hay overflows (y dar error) y operar es usar
// static_bigint (ver nota 43 de mi cuaderno). Recordar: solo implementarlo si
// realmente es necesario.
//
// TODO: cuando haya concepts cambiar typename por 'ratio'.
template <typename q1, typename q2>
using ratio_add =
    ratio<q1::num * q2::den + q1::den * q2::num, q1::den * q2::den>;

template <typename q1, typename q2>
using ratio_subtract =
    ratio<q1::num * q2::den - q1::den * q2::num, q1::den * q2::den>;

template <typename q1, typename q2>
using ratio_multiply = ratio<q1::num * q2::num, q1::den * q2::den>;

template <typename q1, typename q2>
using ratio_divide = ratio<q1::num * q2::den, q1::den * q2::num>;


// -----------
// Comparación
// -----------
// Observar que al no haber sobrecarga de templates (no haber concepts) no
// podemos sobrecargar equal(ratio q1, ratio q2)!!! teniendo que usar
// ratio_equal en vez de equal.
template <typename q1, typename q2>
struct ratio_equal : 
    public bool_constant<q1::num == q2::num and q1::den == q2::den> { };

template <typename q1, typename q2>
inline constexpr bool ratio_equal_v = ratio_equal<q1, q2>::value;

template <typename q1, typename q2>
struct ratio_not_equal : 
    public bool_constant<!ratio_equal_v<q1, q2>> { };

template <typename q1, typename q2>
inline constexpr bool ratio_not_equal_v = ratio_not_equal<q1, q2>::value;


// ------------------
// Tipos predefinidos
// ------------------
using atto  = ratio<1, 1000000000000000000>;
using femto = ratio<1,    1000000000000000>;
using pico  = ratio<1,       1000000000000>;
using nano  = ratio<1,          1000000000>;
using micro = ratio<1,             1000000>;
using milli = ratio<1,                1000>;
using centi = ratio<1,                 100>;
using deci  = ratio<1,                  10>;
using deca  = ratio<                 10, 1>;
using hecto = ratio<                100, 1>;
using kilo  = ratio<               1000, 1>;
using mega  = ratio<            1000000, 1>;
using giga  = ratio<         1000000000, 1>;
using tera  = ratio<      1000000000000, 1>;
using peta  = ratio<   1000000000000000, 1>;
using exa   = ratio<1000000000000000000, 1>;

}// namespace

#endif


