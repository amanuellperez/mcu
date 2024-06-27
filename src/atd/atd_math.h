// Copyright (C) 2019-2024 Manuel Perez 
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

#ifndef __ATD_MATH_H__
#define __ATD_MATH_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Funciones matemáticas.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    07/03/2020 ten_to_the
 *    05/04/2020 number_of_digits
 *    05/03/2021 is_power_of_ten, exponent_of_power_of_ten
 *    23/07/2021 remove_trailing_zeros
 *    19/02/2022 Digits_of, Digits_from_left_to_right
 *    23/04/2023 is_even/is_odd
 *    12/10/2023 Integer_and_decimal_part
 *    24/10/2023 sign_of
 *    06/06/2024 divide_rounding<int>
 *    10/06/2024 Sign
 *    23/06/2024 overflow
 *    27/06/2024 multiply(x).by_ten_to_the(n);
 *               divide  (x).by_ten_to_the(n);
 *
 *
 ****************************************************************************/
#include <cstdlib>
#include <utility>
#include <type_traits>
#include <limits>
#include <algorithm> 
#include "atd_cmath.h"
#include "atd_concepts.h"
#include "atd_type_traits.h"

namespace atd{

/// Devuelve 10^n
template <typename Int>
inline constexpr Int ten_to_the(int n)
{
    if (n == 0)
	return Int{1};

    else 
	return Int{10} * ten_to_the<Int>(n - 1);
}


// if (x == 10^n) return true;
// else           return false;
//
// El 1 == 10^0, lo considero una potencia de 10 (lo normal en matemáticas)
// Las potencias de 10 son números positivos.
// TODO: de momento solo funciona con n >= 0. Implementar el caso n < 0
template <typename Int>
inline constexpr bool is_power_of_ten(Int x)
{
    if (x == 1)
	return true;

    if (!(x % Int{10})){
	    return is_power_of_ten(x / Int{10});
    }
    
    else
	return false;

}

/// Devuelve el exponente de una potencia de 10.
/// precondicion: x == 10^n.
/// En caso de que x no sea una potencia de 10, devuelve 0.
template <typename Int>
inline constexpr int exponent_of_power_of_ten(Int x, int exp = 0)
{
    if (x == 1)
	return exp;

    if (!(x % Int{10})){
	    return exponent_of_power_of_ten(x / Int{10}, exp + 1);
    }
    
    else
	return 0;

}





/// Devuelve los n most significant digits de x.
///
/// Ejemplo:
///	    int x = most_significant_digits<int, 2>(1234);
///
/// Devuelve x == 12;
template <Type::Integer Int, int n>
inline constexpr Int most_significant_digits(Int x)
{
    static_assert(n > 0, "n must be greater than 0");

    Int x0 = x;

    while (x >= ten_to_the<Int>(n-1)){
	x0 = x;
	x /= Int{10};
    }

    return x0;
}

template <Type::Integer Int>
inline constexpr Int most_n_significant_digits(int n, Int x)
{
    if (n <= 0)
	return 0; // throw logic_error? (en micros no puedo lanzar excepciones)

    Int x0 = x;

    while (x >= ten_to_the<Int>(n-1)){
	x0 = x;
	x /= Int{10};
    }

    return x0;
}



// Devuelve el número de cifras que tiene x.
// Ejemplo: number_of_digits(324) == 3;
// TODO: restringir a Type::Integer. No puedo ponerlo de momento
// ya que eng_magnitude está calculando number_of_digits de un double!!!
// Al eliminar eng_magnitude imponer esta reestricción
//template <Type::Integer Int>
template <typename Int>
inline constexpr int number_of_digits(Int x)
{
    int n = 1;

    while (x >= Int{10}){
	++n;
	x /= 10;
    }

    return n;
}






/// Devuelve x pero sin los 0 a la derecha.
/// Ejemplo: si x = 12300, devuelve 123.
template <typename Int>
inline constexpr Int remove_trailing_zeros(Int x)
{
    while (x != 0 and (x % 10) == 0){
	x /= Int{10};
    }

    return x;
}


// Digits_of 
// ---------
// Devuelve los digitos de un número leyéndolo de dcha a izda (unidades,
// decenas, ...).
// Ejemplo: el número 1234 devuelve al iterar 4, 3, 2, 1.
// Voy a implementar un iterador mínimo. Quizás sería interesante traer
// alp_iterator.h y usar esa clase.
// Es un forward iterator.
template <typename Int>
class Iterator_digits_of{
public:
//    // Es regular
//    Iterator_digits_of(): x_{0}, divisor_{0} {}

    Iterator_digits_of(Int x)
        : x_{x}
    { }

    static Iterator_digits_of end()
    {return Iterator_digits_of{std::numeric_limits<Int>::max()};}

    Int operator*() {return digit();}

    Iterator_digits_of& operator++()
    {
        next();
        return *this;
    }


    Iterator_digits_of operator++(int)
    {
        Iterator_digits_of tmp = *this;
        ++*this;
        return tmp;
    }

    friend bool operator==(const Iterator_digits_of& a,
                           const Iterator_digits_of& b)
    {return a.x_ == b.x_;} 

    friend bool operator!=(const Iterator_digits_of& a,
                           const Iterator_digits_of& b)
    {return !(a == b);}


private:
    Int x_;

    void next() 
    { 
	// x_ = (x_ - digit()) / Int{10}; 
	x_  /= Int{10}; 
	if (x_ == 0)
	    x_ = std::numeric_limits<Int>::max(); // = end
    }

    Int digit() { return x_ % Int{10}; }

};


// Si el número es negativo, no devuelve el signo.
template <typename Int>
class Digits_of{
public:
    Digits_of(Int x) : x_{abs(x)} { }
    
    Iterator_digits_of<Int> begin() const
    { return Iterator_digits_of<Int>{x_}; }

    Iterator_digits_of<Int> end() const
    { return Iterator_digits_of<Int>::end(); }

private:
    Int x_;
};



// Digits_from_left_to_right
// --------------------------
// Devuelve los digitos de un número leyéndolo de izda a dcha.
// Ejemplo: el número 1234 devuelve al iterar 1, 2, 3, 4.
// Voy a implementar un iterador mínimo. Quizás sería interesante traer
// alp_iterator.h y usar esa clase.
// Es un forward iterator.
template <typename Int>
class Iterator_digits_in_reverse_order{
public:
//    // Es regular
//    Iterator_digits_in_reverse_order(): x_{0}, divisor_{0} {}

    Iterator_digits_in_reverse_order(Int x, Int divisor)
        : x_{x}, divisor_{divisor}
    { }

    static Iterator_digits_in_reverse_order end()
    {return Iterator_digits_in_reverse_order{0, 0};}

    Int operator*() {return x_ / divisor_;}

    Iterator_digits_in_reverse_order& operator++()
    {
        next();
        return *this;
    }


    Iterator_digits_in_reverse_order operator++(int)
    {
        Iterator_digits_in_reverse_order tmp = *this;
        ++*this;
        return tmp;
    }

    friend bool operator==(const Iterator_digits_in_reverse_order& a,
                           const Iterator_digits_in_reverse_order& b)
    {return a.x_ == b.x_ and a.divisor_ == b.divisor_;} 

    friend bool operator!=(const Iterator_digits_in_reverse_order& a,
                           const Iterator_digits_in_reverse_order& b)
    {return !(a == b);}


private:
    Int x_;
    Int divisor_;

    void next() 
    {
	bool last = (divisor_ == 1? true: false);

	Int digit = x_ / divisor_;

	x_ -= digit * divisor_;
	divisor_ /= Int{10};

	if (last)
	    divisor_ = 0;
    }

};


// Si el número es negativo, no devuelve el signo.
template <typename Int>
class Digits_from_left_to_right{
public:
    // ndigits = número mínimo de cifras a devolver.
    // Si el número tiene menos cifras añade ceros a la izda.
    // Si se pasa ndigits = 0, no añade ceros.
    Digits_from_left_to_right(Int x, int ndigits = 0) 
	: x_{abs(x)}, ndigits_{ndigits} { }
    
    Iterator_digits_in_reverse_order<Int> begin() const
    {
	Int divisor = 1;
	// while (x_ >= divisor * Int{10}) OJO: posible overflow al multiplicar x 10
	while (x_ / Int{10} >= divisor)
	    divisor *= Int{10};
	
	if (ndigits_ > 0)
	    divisor = std::max(divisor, ten_to_the<Int>(ndigits_ - 1));

	return Iterator_digits_in_reverse_order<Int>{x_, divisor};
    }

    Iterator_digits_in_reverse_order<Int> end() const
    { return Iterator_digits_in_reverse_order<Int>::end(); }

private:
    Int x_;
    int ndigits_;
};


// is_even/is_odd
// --------------
template <Type::Integer Int>
inline 
constexpr bool is_even(const Int& x) {return !(x % 2); }

template <Type::Integer Int>
inline 
constexpr bool is_odd(const Int& x) {return (x % 2); }


// sign_of
// -------
template <Type::Integer Int>
inline 
constexpr int sign_of(const Int& x)
{
    if (x > 0)
	return +1;

    if constexpr (std::is_signed_v<Int>){
	if (x < 0)
	    return -1;
    }

    return 0;
}


// divide_rounding
// ---------------
// La división de dos enteros trunca. A veces es más interesante
// que redonde. La siguiente función redondea el resultado.
template <std::integral Int>
inline Int divide_rounding(const Int& x, const Int& y)
{
    auto [q, r] = div(x, y);

    if (2 * r >= y)
	return q + 1;

    return q;

}


// Sign
// ----
// Clase que sirve para nombrar el signo

enum class Sign{
    positive,
    negative,
    plus = positive,	// ¿mejor positive o plus?
    minus = negative
};

inline Sign opposite(Sign s)
{
    if (s == Sign::positive)
	return Sign::negative;

    else 
	return Sign::positive;
}

inline std::ostream& operator<<(std::ostream& out, Sign sign)
{
    switch (sign){
	break;	case Sign::positive: out << '+';

	break;	case Sign::negative: out << '-';
    }

    return out;
}


// overflow
// --------
template <Type::Integer Int>
    requires (std::is_unsigned_v<Int>)
inline 
constexpr bool overflow(const same_type_with_double_bits_t<Int>& x)
{
    if (x <= std::numeric_limits<Int>::max())
	return false;

    return true;
}


template <Type::Integer Int>
    requires (std::is_signed_v<Int>)
inline 
constexpr bool overflow(const same_type_with_double_bits_t<Int>& x)
{
    if (std::numeric_limits<Int>::min() <= x and 
				x <= std::numeric_limits<Int>::max())
	return false;

    return true;
}


// multiply
// --------
template <Type::Integer Int>
struct multiply{
    constexpr multiply(const Int& x) : x_{x} { }

    // DUDA: qué nombre? by_ten_to_the_power_of vs by_ten_to_the
    template <Type::Integer Int2>
    constexpr Int by_ten_to_the_power_of(Int2 n);

    template <Type::Integer Int2>
    constexpr Int by_ten_to_the(Int2 n)
	{return by_ten_to_the_power_of<Int2>(n);}

    const Int& x_;
};

// TODO: cómo gestionar el overflow?
template <Type::Integer Int>
    template <Type::Integer Int2>
constexpr 
Int multiply<Int>::by_ten_to_the_power_of(Int2 n)
{
    Int x = x_;
    for (; n > 0; --n){
	x *= Int{10};
//	if (x < x_) 
//	    throw overflow!!!
    }

    return x;
}


// divide
// ------
template <Type::Integer Int>
struct divide{
    constexpr divide(const Int& x) : x_{x} { }

    // DUDA: qué nombre? by_ten_to_the_power_of vs by_ten_to_the
    template <Type::Integer Int2>
    constexpr Int by_ten_to_the_power_of(Int2 n);

    template <Type::Integer Int2>
    constexpr Int by_ten_to_the(Int2 n)
	{return by_ten_to_the_power_of<Int2>(n);}

    const Int& x_;
};

// TODO: cómo gestionar el overflow?
template <Type::Integer Int>
    template <Type::Integer Int2>
constexpr 
Int divide<Int>::by_ten_to_the_power_of(Int2 n)
{
    Int x = x_;

    for (; n > 0; --n){
	x /= Int{10};

	if (x == 0)
	    return 0;
    }

    return x;
}


} // namespace


#endif

