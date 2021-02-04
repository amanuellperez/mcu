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
 *  - SEE: ver std::chrono, ver atd::magnitude, tienen la misma estructura.
 *
 *  - TODO:
 *    ¿Cómo construir el Decimal<3> x con valor 3.001?
 *    Si escribo Decimal<3> x{3,1}, este es "3.1", para escribir "3.001"
 *    tendría que poder escribir Decimal<3> x{3, 001} lo cual significa
 *    "3.(octal 01)" diferente en general de "3.001". 
 *    Como esto es algo que no creo que use, y siempre se puede definir
 *    Decimal<3>::from_internal_value(3001); de momento me olvido de este
 *    problema. 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    04/03/2020 v0.0. Versión mínima (muy limitada, para probarla).
 *
 ****************************************************************************/

#include <utility>
#include "atd_math.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

namespace atd{

template <typename Rep, int ndecimals>
class Decimal;

// Un número decimal lo estoy almacenando como x*10^(-n).
// Queremos escribir x1*10^(-n1) como x2*10^(-n2):
//
//	    x2*10^(-n2) = x1*10^(-n1) ==> x2 = x1*10^(n2-n1)
//
// Fórmula que nos dice cómo calcular x2 a partir de x1.
//
template <typename To_decimal, typename Rep1, int n1>
constexpr inline To_decimal decimal_cast(const Decimal<Rep1, n1>& d)
{
    constexpr int n2 = To_decimal::num_decimals;

    if constexpr (n2 == n1)
	return To_decimal{d.internal_value()};

    else if constexpr (n2 > n1)
        return To_decimal::from_internal_value(d.internal_value() *
	    ten_to_the<typename To_decimal::Rep>(n2 - n1));

    else
        return To_decimal::from_internal_value(d.internal_value() /
			    ten_to_the<typename To_decimal::Rep>(n1 - n2));
}



/*!
 *  \brief  Número decimal con num_decimals cifras decimales.
 *
 *  Lo que vamos a hacer es operar con números con un número fijo de
 *  decimales. Esto es, si multiplicamos/dividimos números con 2 cifras
 *  decimales el resultado tiene también 2 cifras decimales!!! (= FIX)
 *
 *  Internamente estamos escribiendo el número como potencia de 10:
 *
 *		    x*10^(-n)
 *
 *  siendo x = el valor que almacenamos, y 'n' el número de cifras decimales.
 *
 *  Ejemplo:
 *	2.78 = 278*10^(-2)  ==> almacenamos 278, usamos n = 2.
 *
 */
template <typename Rep0, int ndecimals0>
class Decimal{
public:
// Types
    using Rep = Rep0; // representación. Copio la notación de chrono.
    static constexpr int num_decimals = ndecimals0;

// Construction
    constexpr Decimal() = default;

    /// Definimos el número con la parte fraccionaria (si la tiene) igual a
    /// cero.
    constexpr explicit Decimal(Rep integer_part)
	:x_{construct(integer_part, 0)}
    { }

    /// Definimos el número "integer_part'fractional_part". 
    constexpr Decimal(Rep integer_part, Rep fractional_part);

    template <typename Rep2, int n2>
    constexpr explicit Decimal(const Decimal<Rep2, n2>& d)
	:x_{decimal_cast<Decimal>(d).internal_value()} {}


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
    constexpr static Decimal from_internal_value(Rep x);


// Info
    
    /// Devuelve el número como {integer_part, fractional_part}
    constexpr std::pair<Rep, Rep> value() const
    {
        auto [q, r] = atd::div(atd::abs(x_), ten_to_the_n);

	if (x_ < 0) 
	    q = -q;

        return std::pair<Rep, Rep>{q, r};
    }

// Observer
    constexpr Rep internal_value() const {return x_;}

// Estructura algebraica de espacio vectorial
    // Suma
    constexpr Decimal& operator+=(const Decimal& a);
    constexpr Decimal& operator-=(const Decimal& a);

    // Producto por escalares
    constexpr Decimal& operator*=(const Rep& s);
    constexpr Decimal& operator/=(const Rep& s);

private:
    // Ejemplo: si num_cifras == 2 y value_ = 314, entonces el número decimal
    // vale 3.14
    Rep x_;

    static constexpr Rep ten_to_the_n = ten_to_the<Rep>(num_decimals);


// construcción
    constexpr Rep construct(Rep integer_part, Rep fractional_part);

};


template <typename I, int n>
inline constexpr 
Decimal<I,n>::Decimal(Rep integer_part, Rep fractional_part)
    : x_{construct(integer_part, fractional_part)}
{ }



template <typename I, int n>
inline constexpr 
Decimal<I,n> Decimal<I,n>::from_internal_value(Rep x)
{
    Decimal dec{};  // Tengo que escribirlo como dec{} para evitar un warning
    dec.x_ = x;
    return dec;
}



template <typename I, int n>
constexpr Decimal<I,n>::Rep 
	    Decimal<I,n>::construct(Rep integer_part, Rep fractional_part)
{
    if constexpr (n != 0){
	int ndigits = number_of_digits(fractional_part);

	if (ndigits >= n) 
	    fractional_part = most_significant_digits<Rep, n>(fractional_part);
	else{
	    fractional_part *= ten_to_the<Rep>(n - ndigits);
	}
    }
    else
	fractional_part = 0;


    x_ = integer_part * ten_to_the_n;

    if (x_ >= 0)
	x_ += fractional_part;

    else
	x_ -= fractional_part;

    return x_;
}


template <typename I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator+=(const Decimal& a)
{
    x_ += a.x_;

    return *this;
}


template <typename I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator-=(const Decimal& a)
{
    x_ -= a.x_;

    return *this;
}


template <typename I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator*=(const Rep& a)
{
    x_ *= a;

    return *this;
}


template <typename I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator/=(const Rep& a)
{
    x_ /= a;

    return *this;
}

}// namespace atd


// El common_type es el número con mayor cifras decimales.
// Como suele ocurrir con este tipo de funciones, hay un problema con el
// overflow. De momento voy a dar por supuesto que Rep es suficientemente
// grande para que no haya overflow (que es lo que suponemos en la práctica).
template <typename Rep1, int n1, typename Rep2, int n2>
struct std::common_type<atd::Decimal<Rep1, n1>, atd::Decimal<Rep2, n2>>{

    using Rep = std::common_type_t<Rep1, Rep2>;
    static constexpr int n = std::max(n1, n2);

    using type = atd::Decimal<Rep, n>;
};



namespace atd{

// Estructura algebráica de espacio vectorial
// ------------------------------------------
template <typename R1, int n1, typename R2, int n2>
constexpr inline std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>
    operator+(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    return CT::from_internal_value(CT{a}.internal_value() +
                                   CT{b}.internal_value());
}


template <typename R1, int n1, typename R2, int n2>
constexpr inline std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>
    operator-(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    return CT::from_internal_value(CT{a}.internal_value() -
                                   CT{b}.internal_value());
}


// Multiplicación:  x1*10^(-n1) * x2*10^(-n2) = (x1*x2)*10^-(n1+n2)
// Problema: ¿qué pasa con overflow?
//	Si multiplico 1'000 * 1000000'000 obtengo:
//	1000 * 1000000000 = 10^3*10^9 = 10^12 número que no entra en un
//	int32_t. Sin embargo 1'000*1000000'000 = 1000000'000 que sí debería de
//	entrar.
//
// * Solución 1:
//	Sean los números i1'f1 y i2'f2 (integer'fractional part).
//	Entonces:
//	i1'f1 * i2'f2 = (i1 + f1*10^(-n))*(i2 + f2*10^(-n))
//	              = i1*i2 + (i1*f2 + i2*f1 + (f1*f2)*10^(-n))*10^(-n)
//
//	De esta forma si multiplico 1'000 * 1000000'000, hacemos
//	1*10^6 + (0*0 + 0*0 + 0*0) = 10^6
//	añadiendo la parte decimal quedaría almacenado internamente como 10^9,
//	que entra en uint32_t.
template <typename R1, int n1, typename R2, int n2>
//constexpr inline Decimal<std::common_type_t<R1, R2>, n1 + n2>
constexpr inline Decimal<std::common_type_t<R1, R2>, std::max(n1,n2)>
    operator*(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    // El resultado de la operación  tendrá n1 + n2 cifras decimales
    constexpr int n = std::max(n1, n2);
    using Res = Decimal<std::common_type_t<R1, R2>, n>;
    using Rep = typename Res::Rep;

    constexpr typename Res::Rep ten_to_the_n = ten_to_the<Rep>(n);

    auto [i1, f1] = Res{a}.value();
    auto [i2, f2] = Res{b}.value();

    Rep res = i1*i2*ten_to_the_n + (i1*f2 + i2*f1 + (f1*f2)/ten_to_the_n);

    return Res::from_internal_value(res);
}


// TODO: ¿qué pasa con overflow? Me ha dado overflow!!! ¿cómo poder avisar?
// Naive implementation:
//	x1*10^(-n1) / x2*10^(-n2) = (x1*10^n2/x2)*10^-n1
//  Problema: overflow en 12345'000/100'000
//	al multiplicar 12345*1000 = 12345000 = overflow
//	pero el resultado sí que se puede almacenar en un decimal de 3 cifras:
//	sería 123'450!!!
//
//  Mejora:
//	Escribimos x1/x2 como número mixto x1/x2 = q + r/x2 y operamos:
//	x1*10^(-n1) / x2*10^(-n2) = (q + r/x2)*10^n2*10^(-n1)
//				  = (q*10^n2 + r*10^n2/x2)*10^(-n1)
//	Así evitamos el overflow en del ejemplo anterior.
//
template <typename R1, int n1, typename R2, int n2>
constexpr inline Decimal<R1, n1>
    operator/(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    using Rep = std::common_type_t<R1, R2>;

    Rep x1 = a.internal_value();
    Rep x2 = b.internal_value();

    auto [q, r] = atd::div(x1, x2);

    Rep xr = q*ten_to_the<Rep>(n2) + (r * ten_to_the<Rep>(n2))/x2;

    return Decimal<R1, n1>::from_internal_value(xr);

}


template <typename R, int n>
constexpr inline Decimal<R, n>
    operator/(const R& a, const Decimal<R,n>& b)
{ return Decimal<R,n>::from_internal_value(a * ten_to_the<R>(n)) / b; }



template <typename R1, int n1, typename R2>
constexpr inline Decimal<R1, n1> operator*(const R2& a, Decimal<R1, n1> v)
{
    v *= a;
    return v;
}


template <typename R1, int n1, typename R2>
constexpr inline Decimal<R1, n1> operator*(Decimal<R1, n1> v, const R2& a)
{
    return a*v;
}

template <typename R1, int n1, typename R2>
constexpr inline Decimal<R1, n1> operator/(Decimal<R1, n1> v, const R2& a)
{
    v /= a;
    return v;
}



// Comparisons
// -----------
template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator==(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    return CT{a}.internal_value() == CT{b}.internal_value();
}

template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator!=(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return !(a == b); }


template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator<(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    return CT{a}.internal_value() < CT{b}.internal_value();
}

template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator<=(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return !(b < a); }


template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator>(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return (b < a); }


template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator>=(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return !(a < b); }


// Serialización
// -------------
template <typename Rep, int ndecimals>
std::ostream& operator<<(std::ostream& out,
                                const atd::Decimal<Rep, ndecimals>& d)
{
    auto [n, f] = d.value();
    out << n;

    if constexpr (ndecimals > 0){
	out << '.';

	out.width(ndecimals);
	out.fill('0');
	out << std::right << f;
    }
    return out;
}


}// namespace

#endif

