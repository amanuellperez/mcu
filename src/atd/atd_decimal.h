// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
#ifndef __ATD_DECIMAL_H__
#define __ATD_DECIMAL_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Clase para manejar números decimales con un número fijo de
 *	cifras.
 *
 *  - TODO:
 *	+ que se puedan sumar números decimales con diferentes cifras.
 *	+ añadir multiplicar y dividir números decimales.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    04/03/2020 v0.0. Versión mínima (muy limitada, para probarla).
 *
 ****************************************************************************/

#include <utility>
#include "atd_math.h"
#include <iostream>

namespace atd{


/*!
 *  \brief  Número decimal con ndigits cifras deccimales.
 *
 */
template <typename Int, int ndigits>
class Decimal{
public:
// Types
    using rep = Int; // representación. Copio la notación de chrono.

// Construction
    Decimal() = default;

    /// Definimos el número "integer_part'fractional_part". 
    Decimal(rep integer_part, rep fractional_part)
    {
	if constexpr (ndigits == 0)
	    construct_0digits(integer_part, fractional_part);

	else
	    construct(integer_part, fractional_part);
    }

    /// Construimos un número decimal usando su representación interna.
    /// (RRR) Podríamos usar el constructor: 
    ///
    ///		    Decimal<int, 2> x{314};
    ///
    ///	      pero por defecto yo entiendo que estoy definiendo x == 314'00.
    ///	      Para evitar ello defino from_internal_value(314); Queda clara
    ///	      la intención:
    /// 
    ///		    auto x = Decimal<int, 2>::from_internal_value(314);
    ///
    ///	      definimos x == 3'14.
    ///
    static Decimal from_internal_value(rep x);

    Decimal(const Decimal&) = default;
    Decimal& operator=(const Decimal&) = default;


    ~Decimal() = default;

// Info
    static constexpr int num_digits() {return ndigits;}
    
    /// Devuelve el número como {integer_part, fractional_part}
    std::pair<rep, rep> value() const 
    { return atd::div(x_, ten_to_the_n); }

// Observer.
    rep internal_value() const {return x_;}

// Arithmetic
    Decimal& operator+=(const Decimal& a);
    Decimal& operator-=(const Decimal& a);

private:
    // Ejemplo: si num_cifras == 2 y value_ = 314, entonces el número decimal
    // vale 3.14
    rep x_;

    static constexpr rep ten_to_the_n = ten_to_the<ndigits, rep>();


// construcción
    void construct_0digits(rep integer_part, rep fractional_part);
    void construct(rep integer_part, rep fractional_part);
};



template <typename I, int n>
inline 
Decimal<I,n> Decimal<I,n>::from_internal_value(rep x)
{
    Decimal dec;
    dec.x_ = x;
    return dec;
}


template <typename I, int n>
inline 
void Decimal<I, n>::construct_0digits(rep integer_part, rep fractional_part)
{ x_ = integer_part; }


template <typename I, int n>
void Decimal<I,n>::construct(rep integer_part, rep fractional_part)
{
    fractional_part = most_significant_digits<rep, n>(fractional_part);

    x_ = integer_part * ten_to_the_n;

    if (x_ > 0)
	x_ += fractional_part;

    else
	x_ -= fractional_part;
}

template <typename I, int n>
inline Decimal<I,n>& Decimal<I,n>::operator+=(const Decimal& a)
{
    x_ += a.x_;

    return *this;
}


template <typename I, int n>
inline Decimal<I,n>& Decimal<I,n>::operator-=(const Decimal& a)
{
    x_ -= a.x_;

    return *this;
}

// TODO: que se puedan sumar dos decimales con números de cifras diferentes, y
// tipos rep diferentes.
template <typename I, int n>
inline Decimal<I, n> operator+(Decimal<I,n> a, 
			       const Decimal<I,n>& b)
{
    a += b;
    return a;
}


template <typename I, int n>
inline Decimal<I, n> operator-(Decimal<I,n> a, 
			       const Decimal<I,n>& b)
{
    a -= b;
    return a;
}


/***************************************************************************
 *			    FUNCIONES AYUDA
 ***************************************************************************/
template <typename Int, int ndigits>
inline std::ostream& operator<<(std::ostream& out,
                                const atd::Decimal<Int, ndigits>& d)
{
    auto [n, f] = d.value();

    return out << n << '.' << f;
}


}// namespace

#endif

