// Copyright (C) 2020-2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *    Hay que tener cuidado al restar temperaturas de diferentes unidades ya
 *    que no está claro en qué consiste eso de restar:
 *
 *	Observar que la resta en diferenets unidades no tiene mucho sentido:
 *	Si resto 0ºC - 10ºC obtienes -10ºC, pero ¿y si resto 0ºC - 0ºK?
 *	Si paso K a C:
 *	0 C - 0 K = 0 C - (-273.15) C = +273.15 C !!! <-- ABSURDO, 
 *	ya que la "idea" de restar 0 K no es restar un valor absoluto sino un
 *	incremento. Lo que uno esperaría es que 0C - 0K = 0C.
 *
 *  - SEE: ver std::chrono, ver atd::decimal.
 *  
 *  - TODO: Al usarlo se ve que hay un claro error de diseño. Una magnitud es
 *	    la longitud, otra la frecuencia, otra el periodo y otra la
 *	    velocidad. Cada una de ellas se puede representar usando
 *	    diferentes unidades. Yo quiero hacer:
 *		    Speed v = 2_km_per_second = Kilometer{2}/Second{1};
 *		    Frequency f1 = 2_Hz;
 *		    Length l1 = 3_m;
 *		    Length l2 = 2_km;
 *		    Length l3 = l1 + l2;
 *		    Time t = 3_s; <--- este es std::duration
 *
 *  
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    31/03/2020 v0.0 - versión mínima basada en Stroustrup (The C++
 *    Programming Language, section 28.7). De momento solo necesito
 *    Temperature and Pressure pero quiero dejar una estructura genérica para
 *    poder irla ampliando poco a poco.
 *
 *    30/01/2021 0.0: añado hertz, period y inverse.
 *    08/02/2021      operator / (entre magnitudes del mismo tipo).
 *    06/03/2021      Unit_symbol, numeric_limits, operator++/--
 *
 ****************************************************************************/

#include <ratio>
#include <type_traits>
#include <numeric>
#include <limits>

#include <iostream>

#include "atd_units.h"
#include "atd_ratio.h"

namespace atd{

/***************************************************************************
 *				MAGNITUDE
 ***************************************************************************/
template <typename Unit0,
          typename Rep0,
          typename Multiplier0, typename Displacement = std::ratio<0>>
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
//	value2() = value1() * m1 / m2 + (d1 - d2) / m2;
//
// Llamemos q = m1 / m2, y d = (d1 - d2) / m2;
//
//  Lo que hacemos es calcular estáticamente q = m1 / m2 y operar.
template <typename To_magnitude, 
	  typename Unit, typename Rep, typename Multiplier, typename Displacement>
constexpr inline To_magnitude
	magnitude_cast(const Magnitude<Unit, Rep, Multiplier, Displacement>& m)
{
    using CR = std::common_type_t<typename To_magnitude::Rep, Rep>;

// Q = m1 / m2;
    using Q = std::ratio_divide<Multiplier, typename To_magnitude::Multiplier>;

// Rep D = (d1 - d2) / m2:
    using DIFF =
        std::ratio_subtract<Displacement, typename To_magnitude::Displacement>;
    using Q_D = std::ratio_divide<DIFF, typename To_magnitude::Multiplier>; 
    constexpr typename To_magnitude::Rep D{CR{Q_D::num}/CR{Q_D::den}};

//std::cout << "DIFF = " << DIFF::num << "/" << DIFF::den << '\n';
//std::cout << "Q_D = " << Q_D::num << "/" << Q_D::den << '\n';
//std::cout << "Q = " << Q::num << "/" << Q::den << '\n';
//std::cout << "D = " << D << '\n';

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
 *  2.4 mm = 2.4 * 0.001 m	| long.	    | 1:1000	|   0
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

    using Scalar       = Rep;  // las magnitudes las multiplicamos/dividimos 
			       // por escalares.

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

// Operaciones con escalares
    constexpr Magnitude& operator*=(const Scalar& a);
    constexpr Magnitude& operator/=(const Scalar& a);

// Operaciones con magnitudes
    constexpr Magnitude& operator++();
    constexpr Magnitude operator++(int);

    constexpr Magnitude& operator--();
    constexpr Magnitude operator--(int);

    constexpr Magnitude& operator+=(const Magnitude& x);
    constexpr Magnitude& operator-=(const Magnitude& x);

private:
    Rep value_;
};



// operaciones con magnitudes
// --------------------------
template <typename U, typename R, typename M, typename D>
inline constexpr Magnitude<U,R,M,D>& Magnitude<U,R,M,D>::operator++()
{
    ++value_;
    return *this;
}

template <typename U, typename R, typename M, typename D>
inline constexpr Magnitude<U,R,M,D> Magnitude<U,R,M,D>::operator++(int)
{
    Magnitude old{*this};
    ++(*this);
    return old;
}


template <typename U, typename R, typename M, typename D>
inline constexpr Magnitude<U,R,M,D>& Magnitude<U,R,M,D>::operator--()
{
    --value_;
    return *this;
}


template <typename U, typename R, typename M, typename D>
inline constexpr Magnitude<U,R,M,D> Magnitude<U,R,M,D>::operator--(int)
{
    Magnitude old{*this};
    --(*this);
    return old;
}

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



// operaciones con escalares
// -------------------------
template <typename U, typename Rep, typename M, typename D>
inline constexpr Magnitude<U, Rep, M, D>& 
			Magnitude<U, Rep, M, D>::operator*=(const Scalar& a)
{
    value_ = value_ * a;
    return *this;
}

template <typename U, typename Rep, typename M, typename D>
inline constexpr Magnitude<U, Rep, M, D>& 
			    Magnitude<U, Rep, M, D>::operator/=(const Scalar& a)
{
    value_ = value_ / a;
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


// TODO: de momento me olvido del desplazamiento D. En principio D solo lo uso
// en temperaturas así que no creo que sea necesario tenerlo en cuenta a la
// hora de multiplicar ya que ¿qué sentido tiene multiplicar dos temperaturas?
template <typename U1, typename R1, typename M1, typename D,
	  typename U2, typename R2, typename M2>
inline constexpr Magnitude<Unit_multiply<U1, U2>,
			  std::common_type<R1, R2>,
			  std::ratio_multiply<M1, M2>,
			  D>
operator*(const Magnitude<U1,R1,M1,D>& m1, const Magnitude<U2,R2,M2,D>& m2)
{
    using Rep = std::common_type<R1, R2>;
    using CT = Magnitude<Unit_multiply<U1, U2>,
			  Rep,
			  std::ratio_multiply<M1, M2>,
			  D>;

    Rep x1 = m1.value();
    Rep x2 = m2.value();

    return CT{x1*x2};

}


// Razón de dos magnitudes.
// Observar que no es la división de 2 magnitudes (eso sería espacio/tiempo =
// velocidad), sino la división de 2 magnitudes del mismo tipo, que da como
// resultado el número de veces que es mayor una que otra.
template <typename U, typename Rep1, typename Rep2, typename M1, typename M2, typename D>
constexpr inline std::common_type_t<Rep1, Rep2> 
	    operator/(const Magnitude<U, Rep1, M1, D>& a, 
		      const Magnitude<U, Rep2, M2, D>& b)
{
    using CR = std::common_type_t<Rep1, Rep2>;
    using Q  = std::ratio_divide<M1, M2>;

    CR res = (static_cast<CR>(a.value()) * static_cast<CR>(Q::num)) 
		/ 
	     (static_cast<CR>(b.value()) * static_cast<CR>(Q::den));

    return res;
}

// operaciones con escalares
// -------------------------
template <typename U, typename R, typename M, typename D>
constexpr inline Magnitude<U, R, M, D>
operator*(const typename Magnitude<U, R, M, D>::Scalar& a,
          Magnitude<U, R, M, D> v)
{
    v *= a;
    return v;
}

template <typename U, typename R, typename M, typename D>
constexpr inline Magnitude<U, R, M, D>
operator*(Magnitude<U, R, M, D> v,
          const typename Magnitude<U, R, M, D>::Scalar& a)
{
    return a*v;
}

template <typename U, typename R, typename M, typename D>
constexpr inline Magnitude<U, R, M, D>
operator/(Magnitude<U, R, M, D> v,
          const typename Magnitude<U, R, M, D>::Scalar& a)
{
    v /= a;
    return v;
}

// Magnitud_inverse(Magnitud)
template <typename Mag>
struct __Magnitude_inverse{
    using type = Magnitude<Unit_inverse<typename Mag::Unit>, 
			   typename Mag::Rep, 
			   ratio_inverse<typename Mag::Multiplier>, 
			   typename Mag::Displacement>;
};

template <typename Mag>
using Magnitude_inverse_t = __Magnitude_inverse<Mag>::type;

/// Hace 1/magnitud.
// (RRR) Introduzco esta función para poder convertir frecuencias en periodos
//	  y viceversa:
//        f = 2 Mhz ==> T = 1/f = .5 us
// (RRR) La función inverse habilita el poder dividir magnitudes:
//	 velocity = space / time 
//	 De momento no está implementado pero ya queda preparado para el
//	 futuro.
template <typename U, typename R, typename M, typename D>
constexpr inline Magnitude_inverse_t<Magnitude<U, R, M, D>>
inverse(const Magnitude<U, R, M, D>& a)
{
    R res = R{1} / a.value();

    return Magnitude_inverse_t<Magnitude<U, R, M, D>> {res};
}

template <typename U, typename R, typename M, typename D>
constexpr inline Magnitude_inverse_t<Magnitude<U, R, M, D>>
operator/(const R& a, const Magnitude<U, R, M, D>& b)
{
    return a * inverse(b);
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



// Magnitudes por defecto
// ----------------------
// Length
// ------
template <typename Int, typename Multiplier>
using Length = atd::Magnitude<atd::Units_length, Int, Multiplier>;

template <typename Int>
using Kilometer = Length<Int, std::kilo>;

template <typename Int>
using Hectometer = Length<Int, std::hecto>;

template <typename Int>
using Decameter = Length<Int, std::deca>;

template <typename Int>
using Meter = Length<Int, std::ratio<1>>;

template <typename Int>
using Decimeter = Length<Int, std::deci>;

template <typename Int>
using Centimeter = Length<Int, std::centi>;

template <typename Int>
using Millimeter = Length<Int, std::milli>;


// Frequency
// ---------
template <typename Int, typename Multiplier>
using Frequency = atd::Magnitude<atd::Units_frequency, Int, Multiplier>;

template <typename Int>
using Hertz = Frequency<Int, std::ratio<1,1>>;

template <typename Int>
using KiloHertz = Frequency<Int, std::kilo>;

template <typename Int>
using MegaHertz = Frequency<Int, std::mega>;

template <typename Int>
using GigaHertz = Frequency<Int, std::giga>;

template <typename Int>
using TeraHertz = Frequency<Int, std::tera>;


// Period
// ------
// (???) Observar que std::chrono define todo esto. 
//       ¿se podría integrar magnitud con chrono?
template <typename Int, typename Multiplier>
using Time = atd::Magnitude<atd::Units_time, Int, Multiplier>;

template <typename Int>
using Second = Time<Int, std::ratio<1,1>>;

template <typename Int>
using Millisecond = Time<Int, std::milli>;

template <typename Int>
using Microsecond = Time<Int, std::micro>;

template <typename Int>
using Nanosecond = Time<Int, std::nano>;

template <typename Int>
using Picosecond = Time<Int, std::pico>;


// Temperature
// -----------
// Kelvin
template <typename Int, typename Multiplier>
using __Kelvin = Magnitude<atd::Units_temperature, Int, Multiplier>;

template <typename Int>
using Kelvin = __Kelvin<Int, std::ratio<1>>;


// Celsius
template <typename Int, typename Multiplier>
using __Celsius = Magnitude<atd::Units_temperature, Int, 
			    Multiplier, std::ratio<27315, 100>>;

template <typename Int>
using Celsius = __Celsius<Int, std::ratio<1>>;

template <typename Int>
using Decicelsius = __Celsius<Int, std::deci>;

template <typename Int>
using Centicelsius = __Celsius<Int, std::centi>;

template <typename Int>
using Millicelsius = __Celsius<Int, std::milli>;


// Fahrenheit
template <typename Int>
using Fahrenheit = atd::Magnitude<atd::Units_temperature,
				  Int,
				  std::ratio<5, 9>, std::ratio<45967, 180>>;


// Pressure
// --------
template <typename Int, typename Multiplier>
using Pressure = atd::Magnitude<atd::Units_pressure, Int, Multiplier>;

template <typename Int>
using Kilopascal = Pressure<Int, std::kilo>;

template <typename Int>
using Hectopascal = Pressure<Int, std::hecto>;

template <typename Int>
using Decapascal = Pressure<Int, std::deca>;

template <typename Int>
using Pascal = Pressure<Int, std::ratio<1>>;

template <typename Int>
using Decipascal = Pressure<Int, std::deci>;
    
template <typename Int>
using Centipascal = Pressure<Int, std::centi>;

template <typename Int>
using Millipascal = Pressure<Int, std::milli>;


}// namespace atd



// numeric_limits
// --------------
template <typename U, typename Rep, typename M, typename D>
struct std::numeric_limits<atd::Magnitude<U,Rep,M, D>>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = std::numeric_limits<Rep>::is_signed;
    static constexpr bool is_integer     = std::numeric_limits<Rep>::is_integer;
    static constexpr bool is_exact       = std::numeric_limits<Rep>::is_exact;

    static constexpr atd::Magnitude<U, Rep, M, D> min() noexcept 
    {return atd::Magnitude<U, Rep, M, D>{std::numeric_limits<Rep>::min()};}

    static constexpr atd::Magnitude<U, Rep, M, D> max() noexcept 
    {return atd::Magnitude<U, Rep, M, D>{std::numeric_limits<Rep>::max()};}
};


#endif


