// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __ATD_DOUBLE_H__
#define __ATD_DOUBLE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Soporte para 'double' para los micros. Algunas funciones
 *  útiles.
 *
 *    Aunque es posible que sea más eficiente usar atd::Decimal (???) para
 *    hacer la calculadora voy a experimentar usando 'double'.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    23/07/2021 modf, print, 
 *		 mantissa_to_str_with_zeros, mantissa_to_str,
 *		 to_string
 *
 ****************************************************************************/
#include <utility>
#include <array>
#include <tuple>    // std::tie

#include "atd_algorithm.h"
#include "atd_string.h"

namespace atd{

constexpr char double_decimal_point = '.'; // TODO: ¿dónde guardarlo? locale?

/// Descompone un double en parte entera y fraccionaria.
/// Si x = 123.456 devuelve {123, 0.456}. Observar que la parte fraccionaria es
/// menor que 1.
// Lo dejo igual que std::modf, solo cambia la forma de devolver los datos.
// Esto es, si x = -1.23, devuelve {-1, -0.23}. Observar el signo negativo en
// la parte fraccionaria (así es como funciona std::modf)
std::pair<double, double> modf(double x);

inline std::pair<double, double> to_integer_and_decimal(double x)
{
    auto [i, f] = modf(x);
    if (f < 0)
	return {i, -f};

    return {i, f};
}


// No se trata de una función genérica. Por eso la hago de implementación.
// Sabé que [p0, pe) es un número, y lo que
// hace es eliminar todos los ceros a la izquierda. Si el número fuera todo
// ceros devuelve "0" y no "" que sería lo que haría una función genérica.
// Devuelve [p0, qe) = cadena que contiene el número sin los ceros al final.
std::pair<char*, char*> __remove_trailing_zeros(char* p0, char* pe);


// Convierte el double 'x' en cadena almacenándolo en [m0, me).
// Mantiene los ceros a la izquierda.
// Ejemplo: si x = 0.123 y [m0, me) son 5 caracteres, devuelve
//	      [m0, me) = "12300".
// Precondition: x0 = 0.yyyy (parte entera(x0)  < 1)
char* mantissa_to_str_with_zeros(const double& x0, char* m0, char* me);


// Convierte el double 'x' en la cadena almacenándolo en [m0, me).
//  Ejemplo: si x = 0.123 y str es de 5 caracteres devuelve str = 12300
//  No añade '\0' al final, cuidado si se imprime.
// Precondition: x0 = 0.yyyy (parte entera(x0)  < 1)
// El número de cifras que se usa es N = size[m0, me) = me - m0
// Devuelve [p0, pe) = 'x' escrito como cadena de N cifras. En general 
//	m0 <= p0 <= pe <= me
std::pair<char*, char*> mantissa_to_str(const double& x0, char* m0, char* me);



// Precondition: x = +0.yyyy (parte entera = 0)
// ndigits = número máximo de cifras decimales
template <size_t ndigits = 8>
void __print_mantissa(std::ostream& out, const double& x)
{
    std::array<char, ndigits> str; 

    auto [p0, pe] = mantissa_to_str(x, str.begin(), str.end());

    print(out, p0, pe);
}


// ¿cómo imprimir un double en un LCD? Esta función se encarga de ello.
template <size_t ndigits = 8>
void print(std::ostream& out, double x)
{

    auto [i, f] = to_integer_and_decimal(x);
    
    out << static_cast<int>(i);
    
    if (f != 0){
	out << double_decimal_point;
	__print_mantissa<ndigits>(out, f);
    }
}

/// Convierte el double x en una cadena, con ndigits cifras decimales como
/// máximo, guardándola en [m0, me).
/// Devuelve el número como cadena [p0, pe). En general:
///		m0 <= p0 <= pe <= me
/// En caso de error devuelve {m0, m0}.
/// ndigits = número de cifras decimales máximo a usar.
//// Equivalente a: x.to_cstring(m0, me) [= x --> [m0, me)]
template <int ndigits = 8>
std::pair<char*, char*> to_string(const double& x, char* m0, char* me)
{
    if (m0 == me)
	return {m0, m0};

    auto [i, f] = to_integer_and_decimal(x);

    char* p0 = me; 
    char* pe = me;

    if (f != 0){
	if (me - m0 < ndigits)
	    return {m0, m0};	    // error: no hay espacio!!!

	std::tie(p0, pe) = mantissa_to_str(f, me - ndigits, me);

	if (p0 == m0)
	    return {m0, m0};	    // error: no hay espacio!!!

	--p0;
	*p0 = double_decimal_point;
    }

    if (p0 == m0)
	return {m0, m0};	    // error: no hay espacio!!!


    p0 = int_to_string(static_cast<int>(i), m0, p0);

    return {p0, pe};
}


}// namespace

#endif


