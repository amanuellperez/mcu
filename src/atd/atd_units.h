// Copyright (C) 2021-2024 Manuel Perez 
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

#ifndef __ATD_UNITS_H__
#define __ATD_UNITS_H__


/****************************************************************************
 *
 * DESCRIPCION: Unidades en el S.I.
 *
 * COMENTARIOS
 *    Ángulos
 *    -------
 *    Un ángulo ES una magnitud; en geometría euclidea los segmentos y los
 *    ángulos se conciben de forma similar porque tienen realidad física. 
 *    Si pensamos en radianes son adimensionales porque están definidos por
 *    una razón de longitudes, pero si pienso en grados uso una unidad: 
 *    cuando digo que un ángulo mide 90º me refiero a que voy a tener 90
 *    unidades que llamo grado. Es la misma idea que cuando digo que una
 *    longitud son 90 m. 
 *    Esto sugiere la idea de que puedo concebir el ángulo como unidad básica
 *    y lo puedo añadir a Unit. 
 *
 *    Las unidades están vinculadas a magnitudes: si existe una magnitud,
 *    tiene que existir su unidad correspondiente. Otra cosa es que las
 *    unidades sean derivables o no de otras magnitudes básicas. 
 *    
 *    Concebirlo como unidad básica tiene la ventaja de que se definen de
 *    forma automática la velocidad angular: angulo/tiempo. Si no se define
 *    como unidad básica la velocidad angular tendría unidades de tiempo^{-1}
 *    que son las mismas unidades que Herzt lo cual no permitiría definir una
 *    Magnitud Angular_speed.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    13/03/2021 Reestructurado. Las 'Units' las necesitan tanto
 *		 atd::Magnitude como atd::ENG_magnitude
 *    30/03/2021 Volt
 *    16/10/2023 Unit como struct. Como enum daba warnings.
 *    23/10/2023 Unit_prefix_symbol
 *    26/06/2024 Ángulos
 *
 ****************************************************************************/
#include <ratio>
#include "atd_ratio.h"

namespace atd{

/// Unidades en sistema MKS
//
// Dimensionless == 0: es unidad
//		 == 1: es adimensional
template <int M, int Kg, int S, int Kelvin, int Ampere, int Angle>
struct Unit{
    static constexpr int m = M;
    static constexpr int kg = Kg;
    static constexpr int s = S;
    static constexpr int K = Kelvin;
    static constexpr int A = Ampere;
    static constexpr int angle = Angle; 
};

// ----------
// Operations
// ----------

// Addition: Unit_multiply<U1, U2>
// -------------------------------
namespace impl_of{
template <typename U1, typename U2>
struct Unit_multiply{
    using type = Unit<U1::m + U2::m,
		      U1::kg + U2::kg,
		      U1::s + U2::s,
		      U1::K + U2::K,
		      U1::A + U2::A,
		      U1::angle + U2::angle
		     >;
};
}// impl_of
 
template <typename U1, typename U2>
using Unit_multiply = typename impl_of::Unit_multiply<U1, U2>::type;



// Substraction: Unit_divide<U1, U2>
// ---------------------------------
namespace impl_of{
template <typename U1, typename U2>
struct Unit_divide{
    using type = Unit<U1::m - U2::m,
		      U1::kg - U2::kg,
		      U1::s - U2::s,
		      U1::K - U2::K,
		      U1::A - U2::A,
		      U1::angle - U2::angle
		     >;
};
}// impl_of
 
template <typename U1, typename U2>
using Unit_divide = typename impl_of::Unit_divide<U1, U2>::type;


// Inverse: Unit_inverse<U>
// ------------------------
namespace impl_of{
template <typename U>
struct Unit_inverse{
    using type = Unit<-U::m, -U::kg, -U::s, -U::K, -U::A, -U::angle>;
};
}// impl_of

template <typename U>
using Unit_inverse = typename impl_of::Unit_inverse<U>::type;


// Different types of units
using Units_scalar      = Unit<0, 0, 0, 0, 0, 0>;
using Units_angle	= Unit<0, 0, 0, 0, 0, 1>; 
using Units_length      = Unit<1, 0, 0, 0, 0, 0>;
using Units_mass	= Unit<0, 1, 0, 0, 0, 0>;
using Units_time        = Unit<0, 0, 1, 0, 0, 0>;
using Units_temperature = Unit<0, 0, 0, 1, 0, 0>;
using Units_pressure    = Unit<-1, 1, -2, 0, 0, 0>;
using Units_frequency   = Unit<0, 0, -1, 0, 0, 0>;
// (???) Units_velocity vs Units_speed 
using Units_velocity    = Unit<1, 0, -1, 0, 0, 0>;
using Units_angular_speed      = Unit<0, 0, -1, 0, 0, 1>;
using Units_electric_potential = Unit<2, 1, -3, 0, -1, 0>;


// Symbols
// --------
namespace impl_of{
template <typename U>
struct Unit_symbol
{ static constexpr const char* value = " (Unknown unit. Please, write Unit_symbol) "; };

template <>
struct Unit_symbol<Units_length>
{ static constexpr const char* value = "m"; };

template <>
struct Unit_symbol<Units_angle>
{ static constexpr const char* value = "rad"; };

template <>
struct Unit_symbol<Units_mass>
{ static constexpr const char* value = "g"; };

template <>
struct Unit_symbol<Units_time>
{ static constexpr const char* value = "s"; };

template <>
struct Unit_symbol<Units_temperature>
{ static constexpr const char* value = "K"; };

template <>
struct Unit_symbol<Units_pressure>
{ static constexpr const char* value = "Pa"; };

template <>
struct Unit_symbol<Units_frequency>
{ static constexpr const char* value = "Hz"; };

template <>
struct Unit_symbol<Units_electric_potential>
{ static constexpr const char* value = "V"; };

} // impl_of


template <typename U>
inline constexpr const char* Unit_symbol = impl_of::Unit_symbol<U>::value;



// Unit_prefix_symbol
// ------------------
namespace impl_of{
template <typename U>
struct Unit_prefix_symbol
{ static constexpr const char* short_name = "(Unknown prefix. Please write Unit_prefix_symbol)";};


// (RRR) ¿por qué definir `short_name` como `const char*`y no `char`?
//       El culpable es el deca-
template<>
struct Unit_prefix_symbol<std::exa>{
    static constexpr const char* short_name = "E";
//    static constexpr const char* long_name = "exa";
};

template<>
struct Unit_prefix_symbol<std::peta>{
    static constexpr const char* short_name = "P";
//    static constexpr const char* long_name = "peta";
};

template<>
struct Unit_prefix_symbol<std::tera>{
    static constexpr const char* short_name = "T";
//    static constexpr const char* long_name = "tera";
};

template<>
struct Unit_prefix_symbol<std::giga>{
    static constexpr const char* short_name = "G";
//    static constexpr const char* long_name = "giga";
};


template<>
struct Unit_prefix_symbol<std::mega>{
    static constexpr const char* short_name = "M";
//    static constexpr const char* long_name = "mega";
};

template<>
struct Unit_prefix_symbol<std::kilo>{
    static constexpr const char* short_name = "k";
//    static constexpr const char* long_name = "kilo";
};

template<>
struct Unit_prefix_symbol<std::hecto>{
    static constexpr const char* short_name = "h";
//    static constexpr const char* long_name = "hecto";
};


template<>
struct Unit_prefix_symbol<std::deca>{
    static constexpr const char* short_name = "da"; // NO entra en un char!!!
//    static constexpr const char* long_name = "deka";
};

// Defino este en blanco para que el caso de "10 m" se pueda escribir
// usando el formato: numero prefijo-unidad
template<>
struct Unit_prefix_symbol<std::ratio<1>>{
    static constexpr const char* short_name = "";
//    static constexpr const char* long_name = "";
};

template<>
struct Unit_prefix_symbol<std::deci>{
    static constexpr const char* short_name = "d";
//    static constexpr const char* long_name = "deci";
};


template<>
struct Unit_prefix_symbol<std::centi>{
    static constexpr const char* short_name = "c";
//    static constexpr const char* long_name = "centi";
};

template<>
struct Unit_prefix_symbol<std::milli>{
    static constexpr const char* short_name = "m";
//    static constexpr const char* long_name = "milli";
};

template<>
struct Unit_prefix_symbol<std::micro>{
    static constexpr const char* short_name = "u";
//    static constexpr const char* long_name = "micro";
};

template<>
struct Unit_prefix_symbol<std::nano>{
    static constexpr const char* short_name = "n";
//    static constexpr const char* long_name = "nano";
};


template<>
struct Unit_prefix_symbol<std::pico>{
    static constexpr const char* short_name = "p";
//    static constexpr const char* long_name = "pico";
};

template<>
struct Unit_prefix_symbol<std::femto>{
    static constexpr const char* short_name = "f";
//    static constexpr const char* long_name = "femto";
};


template<>
struct Unit_prefix_symbol<std::atto>{
    static constexpr const char* short_name = "a";
//    static constexpr const char* long_name = "atto";
};
}// namespace


template <Type::Static_ratio Q>
inline constexpr 
const char* Unit_prefix_symbol = impl_of::Unit_prefix_symbol<Q>::short_name;

}// namespace atd
 
#endif
