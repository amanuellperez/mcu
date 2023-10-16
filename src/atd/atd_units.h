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

#ifndef __ATD_UNITS_H__
#define __ATD_UNITS_H__


/****************************************************************************
 *
 *  - DESCRIPCION: Unidades en el S.I.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    13/03/2021 v0.0 Reestructurado. Las 'Units' las necesitan tanto
 *		      atd::Magnitude como atd::ENG_magnitude
 *    30/03/2021      Volt
 *    16/10/2023      Unit como struct. Como enum daba warnings.
 *
 ****************************************************************************/

namespace atd{

/// Unidades en sistema MKS
template <int M, int Kg, int S, int Kelvin, int Ampere>
struct Unit{
    static constexpr int m = M;
    static constexpr int kg = Kg;
    static constexpr int s = S;
    static constexpr int K = Kelvin;
    static constexpr int A = Ampere;
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
		      U1::A + U2::A
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
		      U1::A + U2::A
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
    using type = Unit<-U::m, -U::kg, -U::s, -U::K, -U::A>;
};
}// impl_of

template <typename U>
using Unit_inverse = typename impl_of::Unit_inverse<U>::type;


// Different types of units
using Units_scalar      = Unit<0, 0, 0, 0, 0>;
using Units_length      = Unit<1, 0, 0, 0, 0>;
using Units_mass	= Unit<0, 1, 0, 0, 0>;
using Units_time        = Unit<0, 0, 1, 0, 0>;
using Units_temperature = Unit<0, 0, 0, 1, 0>;
using Units_pressure    = Unit<-1, 1, -2, 0, 0>;
using Units_frequency   = Unit<0, 0, -1, 0, 0>;
using Units_velocity    = Unit<1, 0, -1, 0, 0>;
using Units_electric_potential = Unit<2, 1, -3, 0, -1>;


// Symbols
// --------
namespace impl_of{
template <typename U>
struct Unit_symbol;

template <>
struct Unit_symbol<Units_length>
{ static constexpr const char* value = "m"; };

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

}// namespace

#endif
