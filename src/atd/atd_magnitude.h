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
#ifndef __ATD_MAGNITUDE_H__
#define __ATD_MAGNITUDE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Magnitudes definidas en el S.I.
 *
 *  - SEE: ver std::chrono, ver atd::decimal.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    31/03/2020 v0.0 - versión mínima basada en Stroustrup (The C++
 *    Programming Language, section 28.7). De momento solo necesito
 *    Temperature and Pressure pero quiero dejar una estructura genérica para
 *    poder irla ampliando poco a poco.
 *
 ****************************************************************************/
#include <ratio>
#include <type_traits>
#include <numeric>

#include <iostream>

#include "atd_ratio.h"

namespace atd{

// Unit
// ----
/// Unidades en sistema MKS
template <int M, int K, int S>
struct Unit{
    enum {m = M, kg = K, s = S};
};

// Operaciones
// Suma: Unit_plus<U1, U2>
template <typename U1, typename U2>
struct __Unit_plus{
    using type = Unit<U1::m + U2::m,
		      U1::kg + U2::kg,
		      U1::s + U2::s>;
};

template <typename U1, typename U2>
using Unit_plus = typename __Unit_plus<U1, U2>::type;

// Resta: Unit_minus<U1, U2>
template <typename U1, typename U2>
struct __Unit_minus{
    using type = Unit<U1::m - U2::m,
		      U1::kg - U2::kg,
		      U1::s - U2::s>;
};

template <typename U1, typename U2>
using Unit_minus = typename __Unit_minus<U1, U2>::type;


// Different types of units
using Units_meter    = Unit<1, 0, 0>;
using Units_kilogram = Unit<0, 1, 0>;
using Units_pascal   = Unit<-1, 1, -2>;



template <typename Unit0, typename Rep0, typename Multiplier0>
struct Magnitude;

// --------------
// magnitude_cast
// --------------
// El número de units medidos por una magnitud viene dado por:
//
//		num_units = value() * multiplier;
//
// Si queremos convertir una magnitud en otra tiene que cumplirse que:
//	num_units = value1() * multiplier1 = value2() * multiplier2;
//
// despejando:
//	value2() = value1() * multiplier1 / multiplier2;
//
//  Lo que hacemos es calcular estáticamente CF = multiplier1 / multiplier2 
//  y operar.
template <typename To_magnitude, 
	  typename Unit, typename Rep, typename Multiplier>
constexpr inline To_magnitude
		    magnitude_cast(const Magnitude<Unit, Rep, Multiplier>& m)
{
    constexpr std::ratio_divide<Multiplier, typename To_magnitude::Multiplier> CF;
    using CR = std::common_type_t<typename To_magnitude::Rep, Rep>;

    if constexpr (CF.num == 1 and CF.den == 1)	
        return To_magnitude(static_cast<typename To_magnitude::Rep>(m.value()));
    
    else if (CF.num != 1 and CF.den == 1) 
	return To_magnitude(
	    static_cast<typename To_magnitude::Rep>(
		    static_cast<CR>(m.value()) * static_cast<CR>(CF.num))
			  );

    else if (CF.num == 1 and CF.den != 1)
	return To_magnitude(static_cast<typename To_magnitude::Rep>(
		    static_cast<CR>(m.value()) / static_cast<CR>(CF.den)));

    else
	return To_magnitude(static_cast<typename To_magnitude::Rep>(
	    static_cast<CR>(
	    m.value()) * static_cast<CR>(CF.num) / static_cast<CR>(CF.den)));
}



// Magnitude
// ---------
/*!
 *  \brief  Magnitud.
 *
 *  Esta clase es una generalización de std::chrono::duration, basada en la
 *  que define Stroustrup para manejar magnitudes del sistema internacional.
 *
 *  Unit: indica el tipo de magnitud. Nos da las dimensiones. 
 *	  Ejemplo:  longitud = <1,0,0>	// m
 *		    velocidad= <1,0,-1> // m*s^(-1)
 *
 *  Rep: tipo usado para almacenar los datos. En un ordenador será típicamente
 *	 double pero en un microcontrolador será atd::Decimal.
 *
 *  Multiplier: múltiplo (o submúltiplo) de la unidad que usamos. Es de tipo
 *	      std::ratio.
 *
 *  Ejemplos
 *  --------
 *	Ejemplo		    |	unit	 | múltiplo
 *  ------------------------+------------+-----------
 *  2.4 km = 2.4 * 1000*m	longitud    1000
 *  2.4  m = 2.4 *    1*m	long.	    1
 *  2.4 mm = 2.4 * 0.001*m	long.	    1:1000
 *
 *
 */
template <typename Unit0, typename Rep0, typename Multiplier0>
class Magnitude{
    // preconditions. TODO: con concepts esto se puede simplificar.
    static_assert(is_ratio<Multiplier0>::value,
                  "Multiplier must be a specialization of ratio");
    static_assert(Multiplier0::num > 0, "Multiplier must be positive");

public:
// Types
    using Unit     = Unit0;
    using Rep      = Rep0;
    using Multiplier = Multiplier0;

// Construction
    constexpr Magnitude() = default;

    // TODO: poner como requirement que Rep2 se pueda convertir en Rep
    template <typename Rep2>
    constexpr explicit Magnitude(const Rep2& value0)
	: value_{static_cast<Rep>(value0)} {}


    template <typename Rep2, typename Multiplier2>
    constexpr explicit Magnitude(const Magnitude<Unit, Rep2, Multiplier2>& m)
	: value_{magnitude_cast<Magnitude>(m).value()} {}

    template <typename Rep2, typename Multiplier2>
    Magnitude& operator=(const Magnitude<Unit, Rep2, Multiplier2>& m)
    {
	value_ = magnitude_cast<Magnitude>(m).value();
	return *this;
    }

// Observer
    constexpr Rep value() const {return value_;}

// Estructura algebráica de espacio vectorial
    // Suma de vectores
    constexpr Magnitude& operator+=(const Magnitude& x);
    constexpr Magnitude& operator-=(const Magnitude& x);
    
    // Producto por escalares
    constexpr Magnitude& operator*=(const Rep& a);
    constexpr Magnitude& operator/=(const Rep& a);


private:
    Rep value_;
};

template <typename U, typename R, typename M>
inline constexpr Magnitude<U, R, M>& 
		    Magnitude<U, R, M>::operator+=(const Magnitude<U, R, M>& x)
{
    value_ += x.value();
    return *this;
}


template <typename U, typename R, typename M>
inline constexpr Magnitude<U, R, M>& 
		    Magnitude<U,R,M>::operator-=(const Magnitude<U, R, M>& x)
{
    value_ -= x.value();
    return *this;
}


template <typename U, typename Rep, typename M>
inline constexpr Magnitude<U, Rep, M>& 
				Magnitude<U, Rep, M>::operator*=(const Rep& a)
{
    value_ *= a;
    return *this;
}

template <typename U, typename Rep, typename M>
inline constexpr Magnitude<U, Rep, M>& 
				Magnitude<U, Rep, M>::operator/=(const Rep& a)
{
    value_ /= a;
    return *this;
}

}// namespace atd

template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
struct std::common_type<atd::Magnitude<Unit, Rep1, Multiplier1>,
		   atd::Magnitude<Unit, Rep2, Multiplier2>>{

    using Rep = std::common_type_t<Rep1, Rep2>;
    
    static constexpr auto num = std::gcd(Multiplier1::num, Multiplier2::num);
    static constexpr auto den = std::lcm(Multiplier1::den, Multiplier2::den);

    using Multiplier = std::ratio<num, den>;

    using type = atd::Magnitude<Unit, Rep, Multiplier>;
};


namespace atd{

// ---------------------
// non-member arithmetic
// ---------------------
template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline std::common_type_t<
	    Magnitude<Unit, Rep1, Multiplier1>,
	    Magnitude<Unit, Rep2, Multiplier2>>
    operator+(const Magnitude<Unit, Rep1, Multiplier1>& a,
	      const Magnitude<Unit, Rep2, Multiplier2>& b)
{
    using CM = std::common_type_t<Magnitude<Unit, Rep1, Multiplier1>,
				  Magnitude<Unit, Rep2, Multiplier2>>;

std::cout << ">> a = " << a.value() << '\n';
std::cout << ">> b = " << b.value() << '\n';
    return CM{CM{a}.value() + CM{b}.value()};
}


template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline std::common_type_t<
	    Magnitude<Unit, Rep1, Multiplier1>,
	    Magnitude<Unit, Rep2, Multiplier2>>
    operator-(const Magnitude<Unit, Rep1, Multiplier1>& a,
	      const Magnitude<Unit, Rep2, Multiplier2>& b)
{
    using CM = std::common_type_t<Magnitude<Unit, Rep1, Multiplier1>,
				  Magnitude<Unit, Rep2, Multiplier2>>;

    return CM{CM{a}.value() - CM{b}.value()};
}



template <typename Unit, typename Rep1, typename Multiplier,
			 typename Rep2>
constexpr inline Magnitude<Unit, Rep1, Multiplier>
    operator*(const Rep2& a, Magnitude<Unit, Rep1, Multiplier> v)
{
    v *= a;
    return v;
}

template <typename Unit, typename Rep1, typename Multiplier,
			 typename Rep2>
constexpr inline Magnitude<Unit, Rep1, Multiplier>
    operator*(Magnitude<Unit, Rep1, Multiplier> v, const Rep2& a)
{
    return a*v;
}



// -----------
// Comparisons
// -----------
template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline bool operator==(
	    const Magnitude<Unit, Rep1, Multiplier1>& a,
	    const Magnitude<Unit, Rep2, Multiplier2>& b)
{
    using CT = std::common_type_t<Magnitude<Unit, Rep1, Multiplier1>,
                                  Magnitude<Unit, Rep2, Multiplier2>>;

    return CT{a}.value() == CT{b}.value();
}

template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline bool operator!=(
	    const Magnitude<Unit, Rep1, Multiplier1>& a,
	    const Magnitude<Unit, Rep2, Multiplier2>& b)
{ return !(a == b); }

template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline bool operator<(
	    const Magnitude<Unit, Rep1, Multiplier1>& a,
	    const Magnitude<Unit, Rep2, Multiplier2>& b)
{
    using CT = std::common_type_t<Magnitude<Unit, Rep1, Multiplier1>,
                                  Magnitude<Unit, Rep2, Multiplier2>>;

    return CT{a}.value() < CT{b}.value();
}


template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline bool operator<=(
	    const Magnitude<Unit, Rep1, Multiplier1>& a,
	    const Magnitude<Unit, Rep2, Multiplier2>& b)
{ return !(b < a);}



template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline bool operator>(
	    const Magnitude<Unit, Rep1, Multiplier1>& a,
	    const Magnitude<Unit, Rep2, Multiplier2>& b)
{ return b < a;}



template <typename Unit, typename Rep1, typename Multiplier1,
			 typename Rep2, typename Multiplier2>
constexpr inline bool operator>=(
	    const Magnitude<Unit, Rep1, Multiplier1>& a,
	    const Magnitude<Unit, Rep2, Multiplier2>& b)
{ return !(a < b);}


// operator << 
template <typename U, typename R, typename M>
std::ostream& operator<<(std::ostream& out, const Magnitude<U, R, M>& m)
{ return out << m.value(); }
}// namespace atd


#endif

