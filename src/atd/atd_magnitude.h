// Copyright (C) 2020-2024 Manuel Perez 
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
#ifndef __ATD_MAGNITUDE_H__
#define __ATD_MAGNITUDE_H__
/****************************************************************************
 *
 *  DESCRIPCION: Magnitudes definidas en el S.I.
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
 *  SEE: ver std::chrono, ver atd::decimal.
 *  
 *  USO:    No me gusta la forma de usarlo. Una magnitud es
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
 *	    En cambio, de momento, hay que indicar las unidades:
 *		    Meter l1 = 2_m;
 *	    en lugar de 
 *		    Length l1 = 2_m;
 *
 *	    26/06/2024 El "problema" comentado no es tal. Se puede definir 
 *	    Frequency = Hertz, y el código anterior compilará y funcionará
 *	    correctamente.
 *  
 *  HISTORIA:
 *    Manuel Perez
 *    31/03/2020 v0.0 - versión mínima basada en Stroustrup (The C++
 *    Programming Language, section 28.7). De momento solo necesito
 *    Temperature and Pressure pero quiero dejar una estructura genérica para
 *    poder irla ampliando poco a poco.
 *
 *    30/01/2021 Añado hertz, period y inverse.
 *    08/02/2021 operator / (entre magnitudes del mismo tipo).
 *    06/03/2021 Unit_symbol, numeric_limits, operator++/--
 *    10/03/2022 print
 *    23/10/2023 print: imprime las unidades
 *    26/06/2024 Radian/Degree, Angular_speed
 *
 ****************************************************************************/
#include <ratio>
#include <type_traits>
#include <numeric>
#include <limits>

#include <iostream>

#include "atd_units.h"
#include "atd_ratio.h"
#include "atd_ostream.h"    // print
#include "atd_cast.h"	    // floating_cast
			    
namespace atd{

/***************************************************************************
 *				MAGNITUDE
 ***************************************************************************/
template <typename Unit0,
          Type::Arithmetic Rep0,
          Type::Static_ratio Multiplier0, Type::Static_ratio Displacement = std::ratio<0>>
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
// Llamemos q = m1 / m2, y q_d = (d1 - d2) / m2;
//
//  Lo que hacemos es calcular estáticamente q = m1 / m2 y operar.
template <typename To_magnitude, 
	  typename Unit, 
	  Type::Arithmetic Rep, 
	  Type::Static_ratio Multiplier, Type::Static_ratio Displacement>
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
    using Rep2 = typename To_magnitude::Rep;
    constexpr Rep2 D = floating_cast<Rep2>(CR{Q_D::num}/CR{Q_D::den});

//    using Rep2 = To_magnitude::Rep;
//    Rep2 D{};
//    if constexpr (Q_D::num == 0)
//	D = Rep2{0};
//    else 
//	D = Rep2{CR{Q_D::num}/CR{Q_D::den}};

//std::cout << "DIFF = " << DIFF::num << "/" << DIFF::den << '\n';
//std::cout << "Q_D = " << Q_D::num << "/" << Q_D::den << '\n';
//std::cout << "Q = " << Q::num << "/" << Q::den << '\n';
//std::cout << "D = " << D << '\n';

    if constexpr (Q::num == 1 and Q::den == 1)	
        return To_magnitude(
		    floating_cast<Rep2>(m.value()) + D
			   );
    
    else if (Q::num != 1 and Q::den == 1) {
        return To_magnitude(
	    floating_cast<Rep2>(
		    floating_cast<CR>(m.value()) * floating_cast<CR>(Q::num) + D)
			  );
    }

    else if (Q::num == 1 and Q::den != 1){
	return To_magnitude(floating_cast<Rep2>(
		    floating_cast<CR>(m.value()) / floating_cast<CR>(Q::den)) + D);
    }
    else
	return To_magnitude(floating_cast<Rep2>(
	    floating_cast<CR>(
	    m.value()) * floating_cast<CR>(Q::num) / floating_cast<CR>(Q::den)) + D);
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
          Type::Arithmetic Rep0,
          Type::Static_ratio Multiplier0, Type::Static_ratio Displacement0>
class Magnitude {

    static_assert(Multiplier0::num > 0, "Multiplier must be positive");
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

    template <Type::Arithmetic Rep2>
    constexpr 
    explicit (!std::convertible_to<Rep2, Rep>)
    Magnitude(const Rep2& value0)
	: value_{static_cast<Rep>(value0)} {}

    template <Type::Arithmetic Rep2, typename M2, typename D2>
    constexpr 
    explicit (!std::convertible_to<Rep2, Rep>)
    Magnitude(const Magnitude<Unit, Rep2, M2, D2>& m)
	: value_{magnitude_cast<Magnitude>(m).value()} {}

    template <Type::Arithmetic Rep2, typename M2, typename D2>
    constexpr
    Magnitude& operator=(const Magnitude<Unit, Rep2, M2, D2>& m)
    requires (std::convertible_to<Rep2, Rep>)
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
template <typename U, Type::Arithmetic R, Type::Static_ratio M, Type::Static_ratio D>
inline constexpr Magnitude<U,R,M,D>& Magnitude<U,R,M,D>::operator++()
{
    ++value_;
    return *this;
}

template <typename U, Type::Arithmetic R, Type::Static_ratio M, Type::Static_ratio D>
inline constexpr Magnitude<U,R,M,D> Magnitude<U,R,M,D>::operator++(int)
{
    Magnitude old{*this};
    ++(*this);
    return old;
}


template <typename U, Type::Arithmetic R, Type::Static_ratio M, Type::Static_ratio D>
inline constexpr Magnitude<U,R,M,D>& Magnitude<U,R,M,D>::operator--()
{
    --value_;
    return *this;
}


template <typename U, Type::Arithmetic R, Type::Static_ratio M, Type::Static_ratio D>
inline constexpr Magnitude<U,R,M,D> Magnitude<U,R,M,D>::operator--(int)
{
    Magnitude old{*this};
    --(*this);
    return old;
}

template <typename U, Type::Arithmetic R, Type::Static_ratio M, Type::Static_ratio D>
inline constexpr Magnitude<U, R, M, D>& 
	    Magnitude<U, R, M, D>::operator+=(const Magnitude<U, R, M, D>& x)
{
    value_ += x.value();
    return *this;
}


template <typename U, Type::Arithmetic R, Type::Static_ratio M, Type::Static_ratio D>
inline constexpr Magnitude<U, R, M, D>& 
		Magnitude<U,R,M, D>::operator-=(const Magnitude<U, R, M, D>& x)
{
    value_ -= x.value();
    return *this;
}



// operaciones con escalares
// -------------------------
template <typename U, Type::Arithmetic Rep, Type::Static_ratio M, Type::Static_ratio D>
inline constexpr Magnitude<U, Rep, M, D>& 
			Magnitude<U, Rep, M, D>::operator*=(const Scalar& a)
{
    value_ = value_ * a;
    return *this;
}

template <typename U, Type::Arithmetic Rep, Type::Static_ratio M, Type::Static_ratio D>
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

//template <typename Unit, typename R1, typename M1, typename D1,
//			 typename R2, typename M2, typename D2>
template <typename Unit, Type::Arithmetic R1, Type::Static_ratio M1, 
					      Type::Static_ratio D1,
			 Type::Arithmetic R2, Type::Static_ratio M2, 
					      Type::Static_ratio D2>
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
template <typename Unit, Type::Arithmetic Rep1, Type::Static_ratio Multiplier1, typename D1,
			 Type::Arithmetic Rep2, Type::Static_ratio Multiplier2, typename D2>
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


template <typename Unit, Type::Arithmetic Rep1, Type::Static_ratio Multiplier1, typename D1,
			 Type::Arithmetic Rep2, Type::Static_ratio Multiplier2, typename D2>
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
//template <typename U1, typename R1, typename M1, typename D,
//	  typename U2, typename R2, typename M2>
template <typename U1, Type::Arithmetic R1, Type::Static_ratio M1, Type::Static_ratio D,
	  typename U2, Type::Arithmetic R2, Type::Static_ratio M2>
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
template <typename U, Type::Arithmetic Rep1,
		      Type::Arithmetic Rep2,
		      Type::Static_ratio M1, Type::Static_ratio M2, 
		      Type::Static_ratio D>
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
// template <typename U, typename R, typename M, typename D>
template <typename U, Type::Arithmetic R, Type::Static_ratio M, 
					  Type::Static_ratio D>
constexpr inline Magnitude<U, R, M, D>
operator*(const typename Magnitude<U, R, M, D>::Scalar& a,
          Magnitude<U, R, M, D> v)
{
    v *= a;
    return v;
}

//template <typename U, typename R, typename M, typename D>
template <typename U, Type::Arithmetic R, Type::Static_ratio M, 
					  Type::Static_ratio D>
constexpr inline Magnitude<U, R, M, D>
operator*(Magnitude<U, R, M, D> v,
          const typename Magnitude<U, R, M, D>::Scalar& a)
{
    return a*v;
}

//template <typename U, typename R, typename M, typename D>
template <typename U, Type::Arithmetic R, Type::Static_ratio M, 
					  Type::Static_ratio D>
constexpr inline Magnitude<U, R, M, D>
operator/(Magnitude<U, R, M, D> v,
          const typename Magnitude<U, R, M, D>::Scalar& a)
{
    v /= a;
    return v;
}

// Magnitud_inverse(Magnitud)
// -------------------------
template <typename Mag>
struct Magnitude_inverse{
    using type = Magnitude<Unit_inverse<typename Mag::Unit>, 
			   typename Mag::Rep, 
			   ratio_inverse<typename Mag::Multiplier>, 
			   typename Mag::Displacement>;
};

template <typename Mag>
using Magnitude_inverse_t = Magnitude_inverse<Mag>::type;

/// Hace 1/magnitud.
// (RRR) Introduzco esta función para poder convertir frecuencias en periodos
//	  y viceversa:
//        f = 2 Mhz ==> T = 1/f = .5 us
// (RRR) La función inverse habilita el poder dividir magnitudes:
//	 velocity = space / time 
//	 De momento no está implementado pero ya queda preparado para el
//	 futuro.
template <typename U, Type::Arithmetic Rep, Type::Static_ratio M, 
					  Type::Static_ratio D>
constexpr inline Magnitude_inverse_t<Magnitude<U, Rep, M, D>>
inverse(const Magnitude<U, Rep, M, D>& a)
{
    Rep res = Rep{1} / a.value();

    return Magnitude_inverse_t<Magnitude<U, Rep, M, D>> {res};
}

// versión para enteros: más complicada!
//  TODO: primera implementación rápida que se me ha ocurrido.
//	  Con tiempo pensarlo.
template <typename U, Type::Arithmetic Rep, Type::Static_ratio M, 
					  Type::Static_ratio D>
    requires (std::integral<Rep>)
constexpr inline auto
inverse(const Magnitude<U, Rep, M, D>& a)
{
    static_assert(std::numeric_limits<Rep>::max() > 1'000'000'000);
    Rep r9 = Rep{1'000'000'000} / a.value();

    using Mres = std::ratio_multiply<M, std::giga>;
    return Magnitude_inverse_t<Magnitude<U, Rep, Mres, D>> {r9};

}


template <typename U, Type::Arithmetic R, Type::Static_ratio M, 
					  Type::Static_ratio D>
constexpr inline Magnitude_inverse_t<Magnitude<U, R, M, D>>
operator/(const R& a, const Magnitude<U, R, M, D>& b)
{
    return a * inverse(b);
}







// -----------
// Comparisons
// -----------
template <typename Unit, Type::Arithmetic Rep1, typename M1, typename D1,
			 Type::Arithmetic Rep2, typename M2, typename D2>
constexpr inline bool operator==(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{
    using CT = std::common_type_t<Magnitude<Unit, Rep1, M1, D1>,
                                  Magnitude<Unit, Rep2, M2, D2>>;

    return CT{a}.value() == CT{b}.value();
}

template <typename Unit, Type::Arithmetic Rep1, typename M1, typename D1,
			 Type::Arithmetic Rep2, typename M2, typename D2>
constexpr inline bool operator!=(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return !(a == b); }

template <typename Unit, Type::Arithmetic Rep1, typename M1, typename D1,
			 Type::Arithmetic Rep2, typename M2, typename D2>
constexpr inline bool operator<(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{
    using CT = std::common_type_t<Magnitude<Unit, Rep1, M1, D1>,
                                  Magnitude<Unit, Rep2, M2, D2>>;

    return CT{a}.value() < CT{b}.value();
}


template <typename Unit, Type::Arithmetic Rep1, typename M1, typename D1,
			 Type::Arithmetic Rep2, typename M2, typename D2>
constexpr inline bool operator<=(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return !(b < a);}



template <typename Unit, Type::Arithmetic Rep1, typename M1, typename D1,
			 Type::Arithmetic Rep2, typename M2, typename D2>
constexpr inline bool operator>(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return b < a;}



template <typename Unit, Type::Arithmetic Rep1, typename M1, typename D1,
			 Type::Arithmetic Rep2, typename M2, typename D2>
constexpr inline bool operator>=(
	    const Magnitude<Unit, Rep1, M1, D1>& a,
	    const Magnitude<Unit, Rep2, M2, D2>& b)
{ return !(a < b);}



// Magnitudes por defecto
// ----------------------
// Length
// ------
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Length = atd::Magnitude<atd::Units_length, Int, Multiplier>;

template <Type::Arithmetic Int>
using Kilometer = Length<Int, std::kilo>;

template <Type::Arithmetic Int>
using Hectometer = Length<Int, std::hecto>;

template <Type::Arithmetic Int>
using Decameter = Length<Int, std::deca>;

template <Type::Arithmetic Int>
using Meter = Length<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using Decimeter = Length<Int, std::deci>;

template <Type::Arithmetic Int>
using Centimeter = Length<Int, std::centi>;

template <Type::Arithmetic Int>
using Millimeter = Length<Int, std::milli>;

// Angle
// -----
// Radian
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Angle = Magnitude<atd::Units_angle, Int, Multiplier>;

template <Type::Arithmetic Int>
using Radian = Angle<Int, std::ratio<1>>;

namespace impl_of{
using pi = std::ratio<355, 113>;
//using pi = std::ratio<103'993, 33'102>; // más preciso

using radians_per_degree = std::ratio_divide<pi, std::ratio<180,1>>;

template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Degree = Angle<Int, std::ratio_multiply<radians_per_degree, Multiplier>>;
}

template <Type::Arithmetic Int>
using Degree = impl_of::Degree<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using Decidegree = impl_of::Degree<Int, std::deci>;

template <Type::Arithmetic Int>
using Centidegree = impl_of::Degree<Int, std::centi>;

template <Type::Arithmetic Int>
using Millidegree = impl_of::Degree<Int, std::milli>;

// Angular_speed
// -------------
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Angular_speed = Magnitude<atd::Units_angular_speed, Int, Multiplier>;

// in rad/second
template <Type::Arithmetic Int>
using Radians_per_second = Angular_speed<Int, std::ratio<1>>;

namespace impl_of{
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Degrees_per_second = 
	Angular_speed<Int, std::ratio_multiply<radians_per_degree, Multiplier>>;
}

template <Type::Arithmetic Int>
using Degrees_per_second = impl_of::Degrees_per_second<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using Decidegrees_per_second = impl_of::Degrees_per_second<Int, std::deci>;

template <Type::Arithmetic Int>
using Millidegrees_per_second = impl_of::Degrees_per_second<Int, std::milli>;


// Frequency
// ---------
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Frequency = atd::Magnitude<atd::Units_frequency, Int, Multiplier>;

template <Type::Arithmetic Int>
using Hertz = Frequency<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using KiloHertz = Frequency<Int, std::kilo>;

template <Type::Arithmetic Int>
using MegaHertz = Frequency<Int, std::mega>;

template <Type::Arithmetic Int>
using GigaHertz = Frequency<Int, std::giga>;

template <Type::Arithmetic Int>
using TeraHertz = Frequency<Int, std::tera>;


// Period
// ------
// (???) Observar que std::chrono define todo esto. 
//       ¿se podría integrar magnitud con chrono?
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Time = atd::Magnitude<atd::Units_time, Int, Multiplier>;

template <Type::Arithmetic Int>
using Second = Time<Int, std::ratio<1,1>>;

template <Type::Arithmetic Int>
using Millisecond = Time<Int, std::milli>;

template <Type::Arithmetic Int>
using Microsecond = Time<Int, std::micro>;

template <Type::Arithmetic Int>
using Nanosecond = Time<Int, std::nano>;

template <Type::Arithmetic Int>
using Picosecond = Time<Int, std::pico>;


// Temperature
// -----------
// Kelvin
namespace impl_of{
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Kelvin = Magnitude<atd::Units_temperature, Int, Multiplier>;
}// impl_of

template <Type::Arithmetic Int>
using Kelvin = impl_of::Kelvin<Int, std::ratio<1>>;


// Celsius
namespace impl_of{
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Celsius = Magnitude<atd::Units_temperature, Int, 
			    Multiplier, std::ratio<27315, 100>>;
}// impl_of
 
template <Type::Arithmetic Int>
using Celsius = impl_of::Celsius<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using Decicelsius = impl_of::Celsius<Int, std::deci>;

template <Type::Arithmetic Int>
using Centicelsius = impl_of::Celsius<Int, std::centi>;

template <Type::Arithmetic Int>
using Millicelsius = impl_of::Celsius<Int, std::milli>;


// Fahrenheit
template <Type::Arithmetic Int>
using Fahrenheit = atd::Magnitude<atd::Units_temperature,
				  Int,
				  std::ratio<5, 9>, std::ratio<45967, 180>>;


// Pressure
// --------
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Pressure = atd::Magnitude<atd::Units_pressure, Int, Multiplier>;

template <Type::Arithmetic Int>
using Kilopascal = Pressure<Int, std::kilo>;

template <Type::Arithmetic Int>
using Hectopascal = Pressure<Int, std::hecto>;

template <Type::Arithmetic Int>
using Decapascal = Pressure<Int, std::deca>;

template <Type::Arithmetic Int>
using Pascal = Pressure<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using Decipascal = Pressure<Int, std::deci>;
    
template <Type::Arithmetic Int>
using Centipascal = Pressure<Int, std::centi>;

template <Type::Arithmetic Int>
using Millipascal = Pressure<Int, std::milli>;


// Mass 
// ----
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Mass = atd::Magnitude<atd::Units_mass, Int, Multiplier>;

template <Type::Arithmetic Int>
using Kilogram = Mass<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using Hectogram = Mass<Int, std::ratio<1,10>>;

template <Type::Arithmetic Int>
using Decagram = Mass<Int, std::ratio<1, 100>>;

template <Type::Arithmetic Int>
using Gram = Mass<Int, std::ratio<1, 1000>>;

template <Type::Arithmetic Int>
using Decigram = Mass<Int, std::ratio<1, 10'000>>;
    
template <Type::Arithmetic Int>
using Centigram = Mass<Int, std::ratio<1, 100'000>>;

template <Type::Arithmetic Int>
using Milligram = Mass<Int, std::ratio<1, 1'000'000>>;


// Electrical potential
// --------------------
template <Type::Arithmetic Int, Type::Static_ratio Multiplier>
using Electrical_potential 
		= atd::Magnitude<atd::Units_electric_potential, Int, Multiplier>;

template <Type::Arithmetic Int>
using Kilovolt = Electrical_potential<Int, std::kilo>;

template <Type::Arithmetic Int>
using Hectovolt = Electrical_potential<Int, std::hecto>;

template <Type::Arithmetic Int>
using Decavolt = Electrical_potential<Int, std::deca>;

template <Type::Arithmetic Int>
using Volt = Electrical_potential<Int, std::ratio<1>>;

template <Type::Arithmetic Int>
using Decivolt = Electrical_potential<Int, std::deci>;
    
template <Type::Arithmetic Int>
using Centivolt = Electrical_potential<Int, std::centi>;

template <Type::Arithmetic Int>
using Millivolt = Electrical_potential<Int, std::milli>;


// is_magnitude_unit_basic
// -----------------------
template <typename Unit>
inline constexpr 
bool can_unit_be_composed_by_prefix_and_symbol()
{
    if constexpr (std::is_same_v<Unit, Units_mass>)
	return false;

    else if constexpr (std::is_same_v<Unit, Units_temperature>)
	return false;

    else if constexpr (std::is_same_v<Unit, Units_angle>)
	return false;

    else if constexpr (std::is_same_v<Unit, Units_angular_speed>)
	return false;

    return true;
}

// Magnitude_symbol
// ----------------
template <typename U>
struct Magnitude_symbol;

//// Magnitudes básicas
//// ------------------
//// Las magnitudes componibles son las que puedo componer como "prefix + symbol".
//// Ejemplo: kilo-meter, milli-meter, ...
//// Podemos definir por separado el prefijo del símbolo.
//template <Type::Arithmetic Int>
//struct Magnitude_symbol<Meter<Int>>
//{ static constexpr const char* short_name = "m"; };
//
//template <Type::Arithmetic Int>
//struct Magnitude_symbol<Hertz<Int>>
//{ static constexpr const char* short_name = "Hz"; };
//
//template <Type::Arithmetic Int>
//struct Magnitude_symbol<Second<Int>>
//{ static constexpr const char* short_name = "s"; };
//
//template <Type::Arithmetic Int>
//struct Magnitude_symbol<Pascal<Int>>
//{ static constexpr const char* short_name = "Pa"; };

// Mass
// ----
// La masa es rara. La magnitud básica es el kilogramo: kg. No podemos
// componerla como "prefix + symbol" ya que el symbol básico es `kg`. 
// Por ello me veo obligado a enumerar todas las posibilidades. 
// ¿Otra forma más sencilla de hacerlo?
template <Type::Arithmetic Int>
struct Magnitude_symbol<Kilogram<Int>>
{ static constexpr const char* short_name = "kg"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Hectogram<Int>>
{ static constexpr const char* short_name = "hg"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Decagram<Int>>
{ static constexpr const char* short_name = "dag"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Gram<Int>>
{ static constexpr const char* short_name = "g"; };


template <Type::Arithmetic Int>
struct Magnitude_symbol<Decigram<Int>>
{ static constexpr const char* short_name = "dg"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Centigram<Int>>
{ static constexpr const char* short_name = "cg"; };


template <Type::Arithmetic Int>
struct Magnitude_symbol<Milligram<Int>>
{ static constexpr const char* short_name = "mg"; };


// Temperature
// -----------
// Magnitud que no se puede componer como "prefix + unit". 
// Enumeramos todas las posibilidades.
// NOTA: se podía averiguar si es Kelvin, Celsius o Fahrenheit mirando que la
// unidad sea la Temperatura y el desplazamiento D. Pero esto no serviría para
// la masa. ¿Alguna forma genérica de incluir todos los casos particulares de
// forma más sencilla?
template <Type::Arithmetic Int>
struct Magnitude_symbol<Kelvin<Int>>
{ static constexpr const char* short_name = "K"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Celsius<Int>>
{ static constexpr const char* short_name = "ºC"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Decicelsius<Int>>
{ static constexpr const char* short_name = "ºdC"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Millicelsius<Int>>
{ static constexpr const char* short_name = "ºmC"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Fahrenheit<Int>>
{ static constexpr const char* short_name = "ºF"; };


// Angle
// -----
// Magnitud que no se puede componer como "prefix + unit". 
// Es el mismo problema que con la temperatura: allí tenemos 3 unidades, ºC, K
// y ºF, y en ángulos tenemos dos: º y rad. El radian se compone de forma
// genérica pero el Degree no.
template <Type::Arithmetic Int>
struct Magnitude_symbol<Radian<Int>>
{ static constexpr const char* short_name = "rad"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Degree<Int>>
{ static constexpr const char* short_name = "º"; };

// Aunque se ve raro por la falta de uso, el simbolo del decigrado
// tiene que ser "dº"
template <Type::Arithmetic Int>
struct Magnitude_symbol<Decidegree<Int>>
{ static constexpr const char* short_name = "dº"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Millidegree<Int>>
{ static constexpr const char* short_name = "mº"; };


// Angular_speed
// -------------
// Magnitud que no se puede componer como "prefix + unit". 
// Mismo comentario que en Angle, Temperature, Mass
template <Type::Arithmetic Int>
struct Magnitude_symbol<Radians_per_second<Int>>
{ static constexpr const char* short_name = "rad/s"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Degrees_per_second<Int>>
{ static constexpr const char* short_name = "º/s"; };

// Aunque se ve raro por la falta de uso, el simbolo del decigrado
// tiene que ser "dº"
template <Type::Arithmetic Int>
struct Magnitude_symbol<Decidegrees_per_second<Int>>
{ static constexpr const char* short_name = "dº/s"; };

template <Type::Arithmetic Int>
struct Magnitude_symbol<Millidegrees_per_second<Int>>
{ static constexpr const char* short_name = "mº/s"; };


// operator << 
// -----------
template <Type::Ostream Out, typename Unit, 
			     Type::Arithmetic R, Type::Static_ratio M, 
					         Type::Static_ratio D>
inline Out& operator<<(Out& out, const Magnitude<Unit, R, M, D>& m)
{

    print(out, m.value());

    if constexpr (can_unit_be_composed_by_prefix_and_symbol<Unit>()){
//    if constexpr (requires { Unit_symbol<Unit>;
//			     Unit_prefix_symbol<M>;}){
	out << ' ' 
	    << Unit_prefix_symbol<M>
	    << Unit_symbol<Unit>;
    } else{
	using Mag = Magnitude<Unit, R, M, D>;
	out << ' ' << Magnitude_symbol<Mag>::short_name;
    }


    return out;
}




}// namespace atd



// numeric_limits
// --------------
template <typename U, Type::Arithmetic Rep, 
				    Type::Static_ratio M, Type::Static_ratio D>
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


