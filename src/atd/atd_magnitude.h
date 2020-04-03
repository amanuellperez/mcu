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
template <int M, int Kg, int S, int Kelvin>
struct Unit{
    enum {m = M, kg = Kg, s = S, K = Kelvin};
};

// Operaciones
// Suma: Unit_plus<U1, U2>
template <typename U1, typename U2>
struct __Unit_plus{
    using type = Unit<U1::m + U2::m,
		      U1::kg + U2::kg,
		      U1::s + U2::s,
		      U1::K + U2::K
		     >;
};

template <typename U1, typename U2>
using Unit_plus = typename __Unit_plus<U1, U2>::type;

// Resta: Unit_minus<U1, U2>
template <typename U1, typename U2>
struct __Unit_minus{
    using type = Unit<U1::m - U2::m,
		      U1::kg - U2::kg,
		      U1::s - U2::s,
		      U1::K - U2::K
		     >;
};

template <typename U1, typename U2>
using Unit_minus = typename __Unit_minus<U1, U2>::type;


// Different types of units
using Units_meter    = Unit< 1, 0,  0,  0>;
using Units_kilogram = Unit< 0, 1,  0,  0>;
using Units_pascal   = Unit<-1, 1, -2,  0>;
using Units_kelvin   = Unit< 0, 0,  0,  1>;

template <typename Unit0,
          typename Rep0,
          typename Multiplier0, typename Different = std::ratio<0>>
class Magnitude;

// --------------
// magnitude_cast
// --------------
// La medida en el sistema internacional será:
//
//		SI = value() * multiplier + displacement;
//
// Si queremos convertir una magnitud en otra tiene que cumplirse que:
//	SI = value1() * m1 + d1 = value2() * m2 + d2;
//
// despejando:
//
//	value2() = value1() * m1 / m2 + (d1 - d2) * m1 / m2;
//
// Llamemos q = m1 / m2, y d = (d1 - d2) * m1 / m2;
//
//  Lo que hacemos es calcular estáticamente q = m1 / m2 y operar.
template <typename To_magnitude, 
	  typename Unit, typename Rep, typename Multiplier, typename Displacement>
constexpr inline To_magnitude
	magnitude_cast(const Magnitude<Unit, Rep, Multiplier, Displacement>& m)
{
    using CR = std::common_type_t<typename To_magnitude::Rep, Rep>;

    using Q = std::ratio_divide<Multiplier, typename To_magnitude::Multiplier>;
    using DIFF = std::ratio_subtract<Displacement, typename To_magnitude::Displacement>;
    using Q_D = std::ratio_multiply<Q, DIFF>; // = ratio Q_D = (d1 - d2) * m1 / m2;
    constexpr typename To_magnitude::Rep D{Q_D::num/Q_D::den};// Rep D = (d1 - d2) * m1 / m2;

    if constexpr (Q::num == 1 and Q::den == 1)	
        return To_magnitude(static_cast<typename To_magnitude::Rep>(m.value()) + D);
    
    else if (Q::num != 1 and Q::den == 1) {
        return To_magnitude(
	    static_cast<typename To_magnitude::Rep>(
		    static_cast<CR>(m.value()) * static_cast<CR>(Q::num) + D)
			  );
    }

    else if (Q::num == 1 and Q::den != 1)
	return To_magnitude(static_cast<typename To_magnitude::Rep>(
		    static_cast<CR>(m.value()) / static_cast<CR>(Q::den)) + D);

    else
	return To_magnitude(static_cast<typename To_magnitude::Rep>(
	    static_cast<CR>(
	    m.value()) * static_cast<CR>(Q::num) / static_cast<CR>(Q::den)) + D);
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
 *  Multiplier: factor de conversión = SI/unidades. Es de tipo std::ratio.
 *		Ejemplo: para definir cm ==> multiplier = 1m/100cm = 1:100.
 *
 *  Displacement: La mayoría de las unidades que usamos son múltiplos unas de
 *  otras (mm, m, km se diferencian solo en un factor de escala). Sin embargo
 *  las temperaturas son de la forma K = m*T + d, donde K sería la temperatura
 *  en Kelvins y T puede ser Celsius, Fahrenheit.... Para poder manejar
 *  temperaturas necesito este 'displacement'.
 *
 *
 *  Ejemplos
 *  --------
 *	Ejemplo			|   unit    | múltiplo	| displacement
 *  ----------------------------+-----------+-------------------------------
 *  2.4 km = 2.4 * 1000  m	| longitud  | 1000	|   0
 *  2.4  m = 2.4 *    1  m	| long.	    | 1		|   0
 *  2.4 mm = 2.4 * 0.001 m	| long.	    | 1:100	|   0
 *
 *  20 ºC  = 20 * 1 + 273'15 K	| temper.   | 1		|   27315:100
 *  52 ºF  = 52*5/9 + 45967/180K| temper.   | 5:9	|   45967:180
 *
 */
template <typename Unit0,
          typename Rep0,
          typename Multiplier0, typename Displacement0>
class Magnitude {
    // preconditions. TODO: con concepts esto se puede simplificar.
    static_assert(is_ratio<Multiplier0>::value,
                  "Multiplier must be a specialization of ratio");
    static_assert(Multiplier0::num > 0, "Multiplier must be positive");

    static_assert(is_ratio<Displacement0>::value,
                  "Displacement must be a specialization of ratio");
    static_assert(Displacement0::num >= 0, "Displacement must be positive");

public:
// Types
    using Unit         = Unit0;
    using Rep          = Rep0;
    using Multiplier   = Multiplier0;
    using Displacement = Displacement0;

    // Construction
    constexpr Magnitude() = default;

    // TODO: poner como requirement que Rep2 se pueda convertir en Rep
    template <typename Rep2>
    constexpr explicit Magnitude(const Rep2& value0)
	: value_{static_cast<Rep>(value0)} {}

    template <typename Rep2, typename M2, typename D2>
    constexpr Magnitude(const Magnitude<Unit, Rep2, M2, D2>& m)
	: value_{magnitude_cast<Magnitude>(m).value()} {}

    template <typename Rep2, typename M2, typename D2>
    Magnitude& operator=(const Magnitude<Unit, Rep2, M2, D2>& m)
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

template <typename U, typename R, typename M, typename D>
inline constexpr Magnitude<U, R, M, D>& 
	    Magnitude<U, R, M, D>::operator+=(const Magnitude<U, R, M, D>& x)
{
    value_ += x.value();
    return *this;
}


template <typename U, typename R, typename M, typename D>
inline constexpr Magnitude<U, R, M, D>& 
		Magnitude<U,R,M, D>::operator-=(const Magnitude<U, R, M, D>& x)
{
    value_ -= x.value();
    return *this;
}


template <typename U, typename Rep, typename M, typename D>
inline constexpr Magnitude<U, Rep, M, D>& 
			Magnitude<U, Rep, M, D>::operator*=(const Rep& a)
{
    value_ *= a;
    return *this;
}

template <typename U, typename Rep, typename M, typename D>
inline constexpr Magnitude<U, Rep, M, D>& 
			    Magnitude<U, Rep, M, D>::operator/=(const Rep& a)
{
    value_ /= a;
    return *this;
}

}// namespace atd


// if (Magnitude1::Displacement == Magnitude2::Displacement)
//	type = common_type_equal_displacement(Magnitude1, Magnitude2);
//  else
//	type = common_type_different_displacement(Magnitude1, Magnitude2);
template <typename Mangitude1, typename Mangitude2, 
	  bool equal_displacement = true>
struct __Magnitud_common_type{
    using Unit = typename Mangitude1::Unit;
    using R1   = typename Mangitude1::Rep;
    using M1   = typename Mangitude1::Multiplier;
    using D1   = typename Mangitude1::Displacement;

    using Unit2 = typename Mangitude2::Unit;
    using R2    = typename Mangitude2::Rep;
    using M2    = typename Mangitude2::Multiplier;
    using D2    = typename Mangitude2::Displacement;

    static_assert(std::is_same_v<Unit, Unit2>);

    using Rep = std::common_type_t<R1, R2>;

    // Multiplier = gcd(Multiplier1, Multiplier2);
    static constexpr auto num = std::gcd(M1::num, M2::num);
    static constexpr auto den = std::lcm(M1::den, M2::den);

    using Multiplier = std::ratio<num, den>;
    using type = atd::Magnitude<Unit, Rep, Multiplier, D1>;
};

// caso raro (para temperaturas que se conviertan de ºC a ºF)
// En este caso no me rompo los cuernos y hago que el tipo común sea la
// unidad del sistema internacional
template <typename Mangitude1, typename Mangitude2>
struct __Magnitud_common_type<Mangitude1, Mangitude2, false>{
    using Unit = typename Mangitude1::Unit;
    using Unit2 = typename Mangitude2::Unit;
    static_assert(std::is_same_v<Unit, Unit2>);

    using R1 = typename Mangitude1::Rep;
    using R2 = typename Mangitude2::Rep;
    using Rep = std::common_type_t<R1, R2>;

    using type = atd::Magnitude<Unit, Rep, std::ratio<1>, std::ratio<0>>;
};


template <typename Unit, typename R1, typename M1, typename D1,
			 typename R2, typename M2, typename D2>
struct std::common_type<atd::Magnitude<Unit, R1, M1, D1>,
		   atd::Magnitude<Unit, R2, M2, D2>>
{
    using type = typename 
        __Magnitud_common_type<atd::Magnitude<Unit, R1, M1, D1>,
                               atd::Magnitude<Unit, R2, M2, D2>>::type;
};


namespace atd{

// ---------------------
// non-member arithmetic
// ---------------------
template <typename Unit, typename Rep1, typename Multiplier1, typename D1,
			 typename Rep2, typename Multiplier2, typename D2>
constexpr inline std::common_type_t<
	    Magnitude<Unit, Rep1, Multiplier1, D1>,
	    Magnitude<Unit, Rep2, Multiplier2, D2>>
    operator+(const Magnitude<Unit, Rep1, Multiplier1, D1>& a,
	      const Magnitude<Unit, Rep2, Multiplier2, D2>& b)
{
    using CM = std::common_type_t<Magnitude<Unit, Rep1, Multiplier1, D1>,
				  Magnitude<Unit, Rep2, Multiplier2, D2>>;

    return CM{CM{a}.value() + CM{b}.value()};
}


template <typename Unit, typename Rep1, typename Multiplier1, typename D1,
			 typename Rep2, typename Multiplier2, typename D2>
constexpr inline std::common_type_t <
				    Magnitude<Unit, Rep1, Multiplier1, D1>,
				    Magnitude<Unit, Rep2, Multiplier2, D2>
				    >
    operator-(const Magnitude<Unit, Rep1, Multiplier1, D1>& a,
	      const Magnitude<Unit, Rep2, Multiplier2, D2>& b)
{
    using CM = std::common_type_t<Magnitude<Unit, Rep1, Multiplier1, D1>,
				  Magnitude<Unit, Rep2, Multiplier2, D2>>;

    return CM{CM{a}.value() - CM{b}.value()};
}



template <typename Unit, typename Rep1, typename M, typename D,
			 typename Rep2>
constexpr inline Magnitude<Unit, Rep1, M, D>
    operator*(const Rep2& a, Magnitude<Unit, Rep1, M, D> v)
{
    v *= a;
    return v;
}

template <typename Unit, typename Rep1, typename M, typename D,
			 typename Rep2>
constexpr inline Magnitude<Unit, Rep1, M, D>
    operator*(Magnitude<Unit, Rep1, M, D> v, const Rep2& a)
{
    return a*v;
}



// -----------
// Comparisons
// -----------
template <typename Unit, typename Rep1, typename M1, typename D1,
			 typename Rep2, typename M2, typename D2>
constexpr inline bool operator==(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{
    using CT = std::common_type_t<Magnitude<Unit, Rep1, M1, D1>,
                                  Magnitude<Unit, Rep2, M2, D2>>;

    return CT{a}.value() == CT{b}.value();
}

template <typename Unit, typename Rep1, typename M1, typename D1,
			 typename Rep2, typename M2, typename D2>
constexpr inline bool operator!=(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return !(a == b); }

template <typename Unit, typename Rep1, typename M1, typename D1,
			 typename Rep2, typename M2, typename D2>
constexpr inline bool operator<(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{
    using CT = std::common_type_t<Magnitude<Unit, Rep1, M1, D1>,
                                  Magnitude<Unit, Rep2, M2, D2>>;

    return CT{a}.value() < CT{b}.value();
}


template <typename Unit, typename Rep1, typename M1, typename D1,
			 typename Rep2, typename M2, typename D2>
constexpr inline bool operator<=(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return !(b < a);}



template <typename Unit, typename Rep1, typename M1, typename D1,
			 typename Rep2, typename M2, typename D2>
constexpr inline bool operator>(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return b < a;}



template <typename Unit, typename Rep1, typename M1, typename D1,
			 typename Rep2, typename M2, typename D2>
constexpr inline bool operator>=(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return !(a < b);}


// operator << 
template <typename U, typename R, typename M, typename D>
std::ostream& operator<<(std::ostream& out, const Magnitude<U, R, M, D>& m)
{ return out << m.value(); }
}// namespace atd


#endif


