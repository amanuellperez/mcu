// Copyright (C) 2019-2023 Manuel Perez 
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

#ifndef __ATD_CAST_H__
#define __ATD_CAST_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Cast que encajarían bien en std.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	27/08/2019 safe_static_cast
 *	10/11/2019 bounded_cast
 *	12/03/2021 to_integer
 *	25/11/2022 convert(a).into(b);
 *	13/03/2023 little_endian_to<>/big_endian_to<>
 *	01/11/2023 bool_to
 *	04/10/2024 int_to_cstring
 *
 ****************************************************************************/
#include <limits>
#include <cstddef>
#include <span>
#include <std/std_atd.h>

#include "atd_decimal.h"
#include "atd_minifloat.h"

namespace atd{
// bounded_cast
// ------------
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



// safe_static_cast
// ----------------
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


// to_byte
// -------
// Conversión de bool -> std::byte (necesaria para usarla en las máscaras de
// bits 
inline std::byte to_byte(bool x)
{
    if (x)  return std::byte{1};
    else    return std::byte{0};
}

template <Type::Integer Int>
inline Int bool_to(bool x)
{
    if (x)  return Int{1};
    else    return Int{0};
}

// to_integer
// ----------
// (RRR) ¿Cómo convertir double en int? ¿y atd::Decimal en int? Necesito un
// interfaz común. to_integer lo suministra.
template <Type::Arithmetic T>
constexpr inline T to_integer(const T& x) {return x;}


template <typename Rep2, int N, typename Rep = Rep2>
    requires (!private_::is_class_decimal_v<Rep2>)
inline constexpr Rep2 to_integer(const Decimal<Rep, N>& d)
{
    auto [i, f] = d.value();
    return static_cast<Rep2>(i);
}

template <typename To_decimal, typename Rep, int N>
    requires (private_::is_class_decimal_v<To_decimal>)
inline constexpr To_decimal to_integer(const Decimal<Rep, N>& d)
{
    return decimal_cast<To_decimal>(d);
}

// floating_cast
// -------------
// Es `static_cast` para `floats`. 
// CUIDADO: esta función puede truncar cifras significativas por 
// eso hay que llamarla explícitamente para hacer el casting.
//
// Define un interfaz común para convertir de un decimal point en otro.
// Antes de llamar a esta función hay que confirmar que se puede hacer
// realmente el casting.
// (RRR) Surge en atd::Magnitude cuando se intenta pasar de Meter a Centimeter,
//       por ejemplo, 4'00 m = 400 cm, se pueden usar diferentes números
//       decimales en la representación: 2 decimales para los metros, ninguno
//       para los centímetros. 
//       Sin embargo si se usa atd::Decimal pasar de atd::Decimal<int,2> a
//       atd::Decimal<int,0> supone una pérdida de cifras decimales. Ese
//       cambio no se puede hacer de forma implícita estando prohibido en el
//       constructor de atd::Decimal. ¿Cómo hacerlo? Llamando a
//       `decimal_cast`. El problema es que si quiero que atd::Magnitude
//       funcione también con floats no podemos llamar directamente a
//       decimal_cast. Necesitamos un interfaz genérico: `floating_cast`
//       suministra dicho interfaz.
//
// (RRR) ¿Por qué el orden To, From? Para que se pueda deducir automáticamente
//       el tipo From del parámetro pasado. Si se pone al revés el compilador
//       indica que no es capaz de deducirlo.
template <Type::Arithmetic To, Type::Arithmetic From>
inline constexpr
To floating_cast(const From& x) 
{
    if constexpr (private_::is_class_decimal_v<From> and
		  private_::is_class_decimal_v<To>)
	return decimal_cast<To>(x);

    else if constexpr (is_minifloat_v<From> and
	               is_minifloat_v<To>)
	return minifloat_cast<To>(x);

    else
	return static_cast<To>(x);
}



// convert(a).into(b)
// ------------------
// Uno de los problemas cuando se pasan dos parámetros es que no queda claro
// quién es quién. Ejemplo:
//
//		copy(a, b): esto es `a->b` or `b = a`???
//
// Es más claro escribir `copy(a).to(b)`, aqui no hay lugar a dudas.
//
// Sin embargo C++ no da soporte directo para escribir estas cosas, pero es
// fácil de emularlo con estructuras. 
// Esto es un experimento. Puede (hay que medir) que introduzca una ligera
// ineficiencia, pero gano mucho en claridad.
//
// Para usarlo es necesario implementar convert_x_into_y, que devolverá true
// en caso de que la conversión se haga con éxito, y false si no.
template <typename X>
struct convert{
    convert(const X& x): x_{x} {}

    template <typename Y>
    bool into(Y& y)
    { return convert_x_into_y(x_, y); }

    const X& x_;
};


// little_endian_to
// ----------------
// Convierte el array de bytes data[0], data[1], ... en el tipo Int indicado
// La codificación es little endian: LSByte == data[0]
template <typename Int>
inline Int little_endian_to(std::span<uint8_t> data)
{
    Int res{0};

    for (uint8_t i = 0; i < data.size(); ++i){
	res |= (Int{data[i]} << i * 8);
    }

    return res;
}


// big_endian_to
// ----------------
// Convierte el array de bytes data[0], data[1], ... en el tipo Int indicado
// La codificación es little endian: MSByte == data[0]
template <typename Int>
inline Int big_endian_to(std::span<uint8_t> data)
{
    Int res{0};

    auto n = data.size();
    for (uint8_t i = 0; i < n; ++i){
	res |= (Int{data[n - 1 - i]} << i * 8);
    }

    return res;
}

// int_to_cstring
// --------------
template <typename It, typename Int>
It int_to_cstring(It p0, It pe, Int x)
{ return STD::atd_::int_to_cstring(p0, pe, x); }


}// namespace


#endif


