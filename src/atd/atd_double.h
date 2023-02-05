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
 *    Manuel Perez
 *    23/07/2021 modf, print, 
 *		 mantissa_to_str_with_zeros, mantissa_to_str,
 *		 to_string
 *
 ****************************************************************************/
#include <utility>
#include <array>
#include <tuple>    // std::tie
#include <stdio.h>  // snprintf

#include "atd_algorithm.h"
#include "atd_string.h"
#include "atd_math.h"

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


//// No se trata de una función genérica. Por eso la hago de implementación.
//// Sabé que [p0, pe) es un número, y lo que
//// hace es eliminar todos los ceros a la izquierda. Si el número fuera todo
//// ceros devuelve "0" y no "" que sería lo que haría una función genérica.
//// Devuelve [p0, qe) = cadena que contiene el número sin los ceros al final.
//std::pair<char*, char*> __remove_trailing_zeros(char* p0, char* pe);
//
//
//// Convierte el double 'x' en cadena almacenándolo en [m0, me).
//// Mantiene los ceros a la izquierda.
//// Ejemplo: si x = 0.123 y [m0, me) son 5 caracteres, devuelve
////	      [m0, me) = "12300".
//// Precondition: x0 = 0.yyyy (parte entera(x0)  < 1)
//char* mantissa_to_str_with_zeros(const double& x0, char* m0, char* me);
//
//
//// Convierte el double 'x' en la cadena almacenándolo en [m0, me).
////  Ejemplo: si x = 0.123 y str es de 5 caracteres devuelve str = 12300
////  No añade '\0' al final, cuidado si se imprime.
//// Precondition: x0 = 0.yyyy (parte entera(x0)  < 1)
//// El número de cifras que se usa es N = size[m0, me) = me - m0
//// Devuelve [p0, pe) = 'x' escrito como cadena de N cifras. En general 
////	m0 <= p0 <= pe <= me
//std::pair<char*, char*> mantissa_to_str(const double& x0, char* m0, char* me);




/// Convierte un double en cadena. 
/// Ejemplo: 
///    1) x0 = 23.59, y ndecimals = 5, devuelve [p0, me) = "23.59000"
///	  Mantiene los "000" del final.
///
///    2) x0 = 23.99 y ndecimals = 1, devuelve  [p0, me) = "24.0" 
///       Redondea.
///
/// Devuelve el puntero p0 donde EMPIEZA el double.
template <size_t ndecimals>
char* to_string_with_zeros(const double& x0, char* m0, char* me)
{
    int32_t x = static_cast<int32_t>(x0 * ten_to_the<double>(ndecimals + 1));
    int32_t r{};
    if ((x % 10) < 5)
	r = 0;
    else
	r = 1;
    x = x/10 + r; // redondeamos


    auto p = std::reverse_iterator{me};
    size_t i = 0;
    for (; p != std::reverse_iterator{m0} and x >= 1; ++p, ++i){
	if (i == ndecimals){
	    *p = double_decimal_point;
	}
	else {
	    int y = x % 10;
	    *p = digit_to_char(y);
	    x /= 10;
	}
    }
    
    return p.base();
}



// Devuelve p0, siendo [p0, me) = el double x0 convetido a cadena.
// Deja siempre libre el primer caracter m0, para poder escribir el signo +/-
// si se quiere. Esto es p0 != m0 siempre.
// En caso de error devuelve me ([me, me) = double x0!!!)
// precondition: me != m0	==> std::prev(me) válido
template <size_t ndecimals>
char* __pos_to_string_without_zeros(const double& x0, char* m0, char* me)
{
// Convierto a int y redondeo
// TODO: revisar que no haya overflow!!!
    uint64_t x = static_cast<uint64_t>(x0 * ten_to_the<double>(ndecimals + 1));

    if ((x % 10) >= 5)
	x = x/10 + 1; 
    else
	x = x/10;


// Eliminamos los ceros a la izquierda en la parte decimal
    size_t i = 0;
    while ((x % 10) == 0 and i < ndecimals){
	x /= 10;
	++i;
    }

// Convertimos la parte decimal en str
    auto p = std::prev(me);
    if (i < ndecimals){
	for (; p != m0 and i < ndecimals; --p, ++i){
	    int y = x % 10;
	    *p = digit_to_char(y);
	    x /= 10;
	}

	if (p == m0) return me; // error

	*p = double_decimal_point;

	--p;
	if (p == m0) return me; // error
    }
    if (x < 1){
	*p = '0';
	return p;
    }

    for (; p != m0 and x >= 1; --p, ++i){
	int y = x % 10;
	*p = digit_to_char(y);
	x /= 10;
    }

    if (x >= 1) return me; // error

    return std::next(p);
}

template <size_t ndecimals>
char* to_string_without_zeros(const double& x0, char* m0, char* me)
{
    double x = x0;

    if (x0 < 0){
	x = -x0;
    }

    auto p0 = __pos_to_string_without_zeros<ndecimals>(x, m0, me);

    if (x0 < 0){
	if (p0 != me and p0 != m0){
	    --p0;
	    *p0 = '-';
	}
	else return me;  // error
    }

    return p0;
}



/// Convierte el double x en una cadena, con ndecimals cifras decimales como
/// máximo, guardándola en [m0, me).
/// Devuelve el número como cadena [p0, pe). En general:
///		m0 <= p0 <= pe <= me
/// En caso de error devuelve {m0, m0}.
/// ndecimals = número de cifras decimales máximo a usar.
//// Equivalente a: x.to_cstring(m0, me) [= x --> [m0, me)]
template <int ndecimals>
std::pair<char*, char*> to_string(const double& x, char* m0, char* me)
{
    if (m0 == me)
	return {m0, m0};

    auto p0 = to_string_without_zeros<ndecimals>(x, m0, me);

    return {p0, me};
}



//// Precondition: x = +0.yyyy (parte entera = 0)
//// ndecimals = número máximo de cifras decimales
//template <size_t ndecimals = 8>
//void __print_mantissa(std::ostream& out, const double& x)
//{
//    std::array<char, ndecimals> str; 
//
//    auto [p0, pe] = mantissa_to_str(x, str.begin(), str.end());
//
//    print(out, p0, pe);
//}


// ¿cómo imprimir un double en un LCD? Esta función se encarga de ello.
template <size_t ndecimals = 8>
void print(std::ostream& out, double x)
//inline void print(std::ostream& out, double x)
{
    std::array<char, 20> str;
    auto [p0, pe] = to_string<ndecimals>(x, str.begin(), str.end());
    for (; p0 != pe; ++p0)
	out << *p0;
//    constexpr int N = 20;
//    char str[N];
//
//    int n = snprintf(str, N, "%f", x);
//    if (n < N){
//	    out << str;
//    }
//    else {
//	out << "error print double";
//    }
}




}// namespace

#endif


